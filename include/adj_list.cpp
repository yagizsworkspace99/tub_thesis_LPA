#include "adj_list.h"
#include "parlay/parallel.h"

#include <fstream>
#include <cinttypes>

typedef libcuckoo::cuckoohash_map<uint64_t, std::vector<uint64_t>> Edge;


void AdjList::addSingleEdge(uint64_t source, uint64_t destination, uint64_t time,
                            libcuckoo::cuckoohash_map<uint64_t, Edge> &map) {
    if (map.contains(time)) {
        map.update_fn(time,
                      [&source, &destination](Edge &e) {
                          e.upsert(source,
                                   [&destination](std::vector<uint64_t> &d) { d.push_back(destination); },
                                   std::vector<uint64_t>{destination});
                      });
    } else {
        Edge e;
        e.insert(source, std::vector<uint64_t>{destination});
        map.insert(time, e);
    }
}

void AdjList::addEdge(uint64_t source, uint64_t destination, uint64_t time) {

    //filters out duplicates
    bool flag;
    edges.find_fn(time,
                  [&destination, &flag, &source](Edge &e) {
                      e.find_fn(source,
                                [&flag, &destination](std::vector<uint64_t> &d) {
                                    flag = (std::find(d.begin(), d.end(), destination) != d.end());
                                });
                  });

    if (flag) return;

    //insert edges from source
    addSingleEdge(source, destination, time, edges);
    //insert edges from destination
    addSingleEdge(destination, source, time, edges);

}


void AdjList::deleteSingleEdge(uint64_t source, uint64_t destination, uint64_t time,
                               libcuckoo::cuckoohash_map<uint64_t, Edge> &map) {

    if (map.contains(time)) {
        map.update_fn(time, [&source, &destination](Edge &e) {
            e.update_fn(source, [&destination](std::vector<uint64_t> &d) {
             return d.erase(std::find(d.begin(), d.end(), destination)) <= d.end();
            });
        });
    }
}

void AdjList::deleteEdge(uint64_t source, uint64_t destination, uint64_t time) {

    //check if edge to be deleted exists
    bool flag;

    edges.find_fn(time,
                  [&destination, &flag, &source](Edge &e) {
                      e.find_fn(source,
                                [&flag, &destination](std::vector<uint64_t> &d) {
                                    flag = (std::find(d.begin(), d.end(), destination) != d.end());
                                });
                  });

    if (!flag) return;
    //delete edges from source
    deleteSingleEdge(source, destination, time, edges);
    //delete edges from destination
    deleteSingleEdge(destination, source, time, edges);
}

void AdjList::printGraph() {
    std::cout << "Printing all edges:" << std::endl << std::endl;
    uint64_t count = 0;

    auto lt = edges.lock_table();

    for (const auto &innerTbl: lt) {
        Edge edgeData = innerTbl.second;
        auto lt2 = edgeData.lock_table();
        printf("Time %" PRIu64 " contains edges\n", innerTbl.first);

        for (const auto &vector: lt2) {
            for (auto &edge: vector.second) {
                printf("    - between: %" PRIu64 " and %" PRIu64 " at time %" PRIu64 "\n", vector.first, edge,
                       innerTbl.first);
                count++;
            }
        }
        std::cout << std::endl;
    }
    std::cout << "Total number of edges: " << count << std::endl;
}

void AdjList::addFromFile(const std::string &path) {
    std::ifstream file(path);
    if (file.is_open()) {
        uint64_t source, destination, time;
        std::string command;
        uint64_t maxTime = 0;
        //maybe more efficient to go through the file twice?
        std::vector<uint64_t> sourceAdds{}, destinationAdds{}, timeAdds{};
        std::vector<uint64_t> sourceDels{}, destinationDels{}, timeDels{};

        while (file >> command >> source >> destination >> time) {
            if (command == "add") {
                if (time > maxTime) maxTime = time;
                sourceAdds.push_back(source);
                destinationAdds.push_back(destination);
                timeAdds.push_back(time);
            }
            if (command == "delete") {
                sourceDels.push_back(source);
                destinationDels.push_back(destination);
                timeDels.push_back(time);
            }
        }
        file.close();

        //Create new hash map, keys are source vertices and values are vectors of integer pairs (destination, time).
        //This is then filled by sortBatch function.
        libcuckoo::cuckoohash_map<uint64_t, Edge> groupedDataAdds;
        libcuckoo::cuckoohash_map<uint64_t, Edge> groupedDataDels;

        sortBatch(sourceAdds, destinationAdds, timeAdds, groupedDataAdds);
        batchOperationCuckoo(true, groupedDataAdds);

        sortBatch(sourceDels, destinationDels, timeDels, groupedDataDels);
        batchOperationCuckoo(false, groupedDataDels);
        //addBatchCuckooParlay(groupedData, maxTime);
    }

    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file." << std::endl;
        return;
    }
}

void AdjList::batchOperationCuckoo(bool flag, libcuckoo::cuckoohash_map<uint64_t, Edge> &groupedData) {
    auto t1 = std::chrono::high_resolution_clock::now();
    auto lt = groupedData.lock_table();

    for (const auto &innerTbl: lt) {
        Edge edgeData = innerTbl.second;
        auto lt2 = edgeData.lock_table();

        for (const auto &vector: lt2) {
            for (auto &edge: vector.second) {
                if (flag) addEdge(vector.first, edge, innerTbl.first);
                else deleteEdge(vector.first, edge, innerTbl.first);
            }
        }
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout << "batchOperationCuckoo has taken " << ms_int.count() << "ms\n";
}

//doesn't terminate properly
//terrible with large gaps between timestamps
void AdjList::addBatchCuckooParlay(libcuckoo::cuckoohash_map<uint64_t, Edge> &groupedData, uint64_t maxTime) {
    auto t1 = std::chrono::high_resolution_clock::now();
    auto lt = groupedData.lock_table();

    parlay::parallel_for(lt.begin()->first, maxTime + 1, [&](uint64_t i) {
        if (!lt.find(i)->second.empty()) {
            Edge innerTbl = lt.find(i)->second;
            auto lt2 = innerTbl.lock_table();

            for (const auto &vector: lt2) {
                for (auto &edge: vector.second) {
                    addEdge(vector.first, edge, i);
                }
            }
        }
    });
    auto t2 = std::chrono::high_resolution_clock::now();
    auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout << "addBatchCuckooParlay has taken " << ms_int.count() << "ms\n";
}

void AdjList::sortBatch(const std::vector<uint64_t> &sourceAdds, const std::vector<uint64_t> &destinationAdds,
                        const std::vector<uint64_t> &timeAdds, libcuckoo::cuckoohash_map<uint64_t, Edge> &groupedData) {
    auto t1 = std::chrono::high_resolution_clock::now();

    // Determine the number of iterations
    size_t numIterations = timeAdds.size();

    // Group edges by time using hash map
    for (size_t i = 0; i < numIterations; ++i) {
        uint64_t source = sourceAdds[i];
        uint64_t destination = destinationAdds[i];
        uint64_t time = timeAdds[i];

        addSingleEdge(source, destination, time, groupedData);
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout << "sortBatch has taken " << ms_int.count() << "ms\n";

    //printGroupedData(groupedData);
}

void AdjList::printGroupedData(libcuckoo::cuckoohash_map<uint64_t, Edge> &groupedData) {
    std::cout << "Printing grouped data:" << std::endl << std::endl;
    for (auto &it: groupedData.lock_table()) {
        uint64_t source = it.first;
        Edge &DestTime = it.second;
        auto lt2 = DestTime.lock_table();


        // Print the source and its associated edges
        printf("Time %" PRIu64 " contains %" PRIu64 " edges:\n", source, DestTime.size());
        for (const auto &edge: lt2) {
            for (auto &v: edge.second) {
                printf("    - between %" PRIu64 " and %" PRIu64 "\n", edge.first, v);
            }
        }
        std::cout << std::endl;
    }
    std::cout << "-------------------------------------" << std::endl;
}

void AdjList::addBatch(int *source, int *destination, int *time, int numberElements) {
    for (int i = 0; i < numberElements; ++i) {
        addEdge(source[i], destination[i], time[i]);
    }
}

//TODO: update rangeQuery
/*
void AdjList::rangeQuery(uint64_t start, uint64_t end, int (*func)(uint64_t,uint64_t,uint64_t)){
    for (uint64_t i = start; i <= end; i++){
        Edge out;
        if (edges.find(i, out)){
            for (auto& n: out) {
                func(i, n.first, n.second);
            }
        }
    }
}
*/

/*
//No longer needed
void AdjList::sortByTime() {
    for (int i = 0; i < maxEdge; ++i) {
        Edge out;

        if(edges.contains(i)){
            edges.update_fn(i,
                            [](Edge &e){ std::sort(e.begin(), e.end(), compareTime);});
        }
    }
}

bool AdjList::compareTime(std::pair<uint64_t, uint64_t> i1, std::pair<uint64_t, uint64_t> i2) {
    return (i1.second < i2.second);
}

*/