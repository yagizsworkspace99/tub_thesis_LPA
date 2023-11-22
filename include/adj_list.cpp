#include "adj_list.h"
#include "ParallelTools/parallel.h"

#include <fstream>
#include <cinttypes>
#include <unordered_set>
#include <map>

typedef libcuckoo::cuckoohash_map<uint64_t, std::vector<uint64_t>> Edge;

void AdjList::addSingleEdge(uint64_t source, uint64_t destination, uint64_t time, libcuckoo::cuckoohash_map<uint64_t, Edge> &map) {
    Edge eSource;
    std::vector<uint64_t> tempVectorS;
    tempVectorS.push_back(destination);
    eSource.insert(source, tempVectorS);

    // Upsert: Insert the edge into the vector associated with the source vertex
    map.upsert(time,
                 [&source, &tempVectorS, &destination](Edge &e)
                 {e.upsert(source,
                           [&destination](std::vector<uint64_t> &d){d.push_back(destination);},
                           tempVectorS);},
                 eSource);
}

void AdjList::addEdge(uint64_t source, uint64_t destination, uint64_t time) {

    //filters out duplicates
    bool flag;
    edges.find_fn(time,
                  [&destination, &flag, &source](Edge &e)
                  {e.find_fn(source,
                             [&flag, &destination](std::vector<uint64_t> &d)
                             {flag = (find(d.begin(), d.end(), destination) != d.end());});});

    if (flag) return;

    //insert edges from source
    addSingleEdge(source, destination, time, edges);
    //insert edges from destination
    addSingleEdge(destination, source, time, edges);

}

//not updated yet
/*
void AdjList::deleteEdge(int source, int destination, int time) const {
    auto findSource = findEdge(source, destination, time);
    auto findDestination = findEdge(destination, source, time);

    if (findSource != graph[source].end() && findDestination != graph[destination].end()) {
        graph[source].erase(findSource);
        graph[destination].erase(findDestination);
    }
}
*/

void AdjList::printGraph()  {
    std::cout << "Printing all edges:" <<std::endl << std::endl;

    auto lt = edges.lock_table();

    for (const auto& innerTbl : lt) {
        Edge edgeData = innerTbl.second;
        auto lt2 = edgeData.lock_table();
        printf("Time %" PRIu64 " contains %" PRIu64 " edges\n", innerTbl.first, innerTbl.second.size());

        for (const auto& vector : lt2) {
            for (auto &edge: vector.second) {
                printf("    - between: %" PRIu64 " and %" PRIu64 " at time %" PRIu64 "\n", vector.first, edge, innerTbl.first);
            }
        }
        std::cout << std::endl;
    }
}

void AdjList::addFromFile(const std::string& path) {
    std::ifstream file(path);
    if(file.is_open()){
        int source, destination, time;
        std::string command;
        int noOfAdds = 0;
        std::unordered_set<uint64_t> distinctSources;
        std::unordered_set<uint64_t> distinctDestination;
        //maybe more efficient to go through the file twice?
        std::vector<uint64_t> sourceAdds(noOfAdds), destinationAdds(noOfAdds), timeAdds(noOfAdds);


        while(file >> command >> source >> destination >> time){
            if(command == "add"){
                sourceAdds.push_back(source);
                destinationAdds.push_back(destination);
                timeAdds.push_back(time);

                distinctSources.insert(source);
                distinctDestination.insert(destination);
                noOfAdds++;
            }
        }

        //Compare number of unique sources and destinations, then accordingly set sortFlag for sortBatch method
        //true -> grouping occurs using source vertices
        //false -> grouping occurs using destinations vertices
        bool flag = distinctSources.size() < distinctDestination.size();


        //Create new hash map, keys are source vertices and values are vectors of integer pairs (destination, time).
        //This is then filled by sortBatch function.
        libcuckoo::cuckoohash_map<uint64_t, Edge> groupedData;

        sortBatch(flag, sourceAdds, destinationAdds, timeAdds, groupedData);

        addBatchCuckoo(groupedData);

    }
    file.close();
}

void AdjList::addBatchCuckoo(libcuckoo::cuckoohash_map<uint64_t, Edge>& groupedData) {
    auto lt = groupedData.lock_table();

    ParallelTools::parallel_for_each(lt, [&](auto i, auto innerTbl ){
        auto lt2 = innerTbl.lock_table();

        for (const auto& vector : lt2) {
            for (auto &edge: vector.second) {
                addEdge(vector.first, edge, i);
            }
        }
    });
}


void AdjList::sortBatch(bool sortBySource, const std::vector<uint64_t>& sourceAdds, const std::vector<uint64_t>& destinationAdds,
                        const std::vector<uint64_t>& timeAdds, libcuckoo::cuckoohash_map<uint64_t, Edge>& groupedData) {

    // Determine which vector to use based on the sort flag
    const std::vector<uint64_t>& relevantVector = sortBySource ? sourceAdds : destinationAdds;
    const std::vector<uint64_t>& irrelevantVector = sortBySource ? destinationAdds : sourceAdds;

    // Determine the number of iterations based on the relevant vector
    size_t numIterations = relevantVector.size();

    // Group edges by source vertex using hash map
    for (size_t i = 0; i < numIterations; ++i) {
        uint64_t source = relevantVector[i];
        uint64_t destination = irrelevantVector[i];
        uint64_t time = timeAdds[i];

        // If source is not present in groupedData, it is automatically inserted --> Maybe upsert or insert_or_assign
        //groupedData[vertex].emplace_back(destination, time);

        addSingleEdge(source, destination, time, groupedData);
    }
    printGroupedData(groupedData);
}

void AdjList::printGroupedData(libcuckoo::cuckoohash_map<uint64_t, Edge>& groupedData) {
    std::cout << "Printing grouped data:" <<std::endl << std::endl;
    for (auto & it : groupedData.lock_table()) {
        uint64_t source = it.first;
        Edge &DestTime = it.second;
        auto lt2 = DestTime.lock_table();


        // Print the source and its associated edges
        printf("Time %" PRIu64 " contains %" PRIu64 " edges:\n", source, DestTime.size());
        for (const auto& edge : lt2) {
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