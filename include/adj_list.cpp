#include "adj_list.h"
#include <fstream>
#include <cinttypes>

typedef std::vector<std::pair<uint64_t, uint64_t>> Edge;

void AdjList::addEdge(uint64_t source, uint64_t destination, uint64_t time) {

    //filters out duplicates
    bool flag;
    edges.find_fn(source,
                  [&destination, &time, &flag](Edge &e)
                  {flag = (find(e.begin(), e.end(), std::make_pair(destination, time)) != e.end());});

    if (flag) return;

    Edge eSource;
    eSource.emplace_back(destination, time);

    //insert edge from source
    edges.upsert(source,
                 [&destination, &time](Edge &e){e.emplace_back(destination, time);},
                 eSource);


    Edge eDestination;
    eDestination.emplace_back(source, time);

    //add edge from destination
    edges.upsert(destination,
                 [&source, &time](Edge &e){e.emplace_back(source, time);},
                 eDestination);
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

void AdjList::printGraph() const {
    std::cout << "Printing all edges:" <<std::endl << std::endl;
    for (int i = 0; i < maxEdge + 1; i++) {
        Edge out;
        if (edges.find(i, out)){
            std::cout << "Edge " << i << " contains " << out.size() << " edges." << std::endl;
            for (auto& n: out) {
                printf("    - between: %d and %" PRIu64 " at time %" PRIu64 "\n", i, n.first, n.second);
            }
            std::cout << std::endl;
        }
    }
}

void AdjList::addFromFile(const std::string& path) {
    std::ifstream file(path);
    if(file.is_open()){
        int source, destination, time;
        std::string command;
        int noOfAdds = 0;
        libcuckoo::cuckoohash_map<uint64_t, uint64_t> leftTable;
        libcuckoo::cuckoohash_map<uint64_t, uint64_t> rightTable;

        while(file >> command >> source >> destination >> time){
            int localMax = std::max(source, destination);
            if(localMax > maxEdge) maxEdge = localMax;
            if(command == "add"){
                leftTable.insert(source, destination);
                rightTable.insert(destination, source);
                addEdge(source, destination, time);
                noOfAdds++;
            }
        }

        //Compare number of unique sources and destinations, then accordingly set sortFlag for sortBatch method
        //true -> grouping occurs using source vertices
        //false -> grouping occurs using destinations vertices
        bool flag = (leftTable.size()<rightTable.size());

        file.clear();
        file.seekg(0, std::ifstream::beg);

        std::vector<uint64_t> sourceAdds(noOfAdds), destinationAdds(noOfAdds), timeAdds(noOfAdds);
        int currentLoop = 0;

        while(file >> command >> source >> destination >> time){
            if (command == "add"){
                sourceAdds[currentLoop] = source;
                destinationAdds[currentLoop] = destination;
                timeAdds[currentLoop] = time;
                currentLoop++;
            }

            //if (command == "delete") deleteEdge(source, destination, time);
        }

        //Create new hash map, keys are source vertices and values are vectors of integer pairs (destination, time).
        // This is then filled by sortBatch function.
        libcuckoo::cuckoohash_map<uint64_t, Edge> groupedData;

        sortBatch(flag, sourceAdds, destinationAdds, timeAdds, groupedData);

        addBatchCuckoo(groupedData);

    }
    file.close();

    sortByTime();
}

void AdjList::addBatchCuckoo(libcuckoo::cuckoohash_map<uint64_t, Edge>& groupedData) {
    auto lt = groupedData.lock_table();

    for (const auto& item : lt) {
        uint64_t sourceNode = item.first;
        const Edge& edgeData = item.second;

        // Iterate through edgeData and add edges to the adjacency list
        for (const auto& edge : edgeData) {
            uint64_t destination = edge.first;
            uint64_t time = edge.second;
            addEdge(sourceNode, destination, time);
        }
    }
}

void AdjList::sortBatch(bool sortBySource, const std::vector<uint64_t>& sourceAdds, const std::vector<uint64_t>& destinationAdds,
                        const std::vector<uint64_t>& timeAdds, libcuckoo::cuckoohash_map<uint64_t, Edge>& groupedData) {

    // Determine which vector to use based on the sort flag
    const std::vector<uint64_t>& relevantVector = sortBySource ? sourceAdds : destinationAdds;

    // Determine the number of iterations based on the relevant vector
    size_t numIterations = relevantVector.size();

    // Group edges by source vertex using hash map
    for (size_t i = 0; i < numIterations; ++i) {
        uint64_t vertex = relevantVector[i];
        uint64_t destination = sortBySource ? destinationAdds[i] : sourceAdds[i];
        uint64_t time = timeAdds[i];

        // If source is not present in groupedData, it is automatically inserted --> Maybe upsert or insert_or_assign
        //groupedData[vertex].emplace_back(destination, time);

        // Upsert: Insert the edge into the vector associated with the source vertex
        groupedData.upsert(vertex,
                           [&](Edge& v) {v.emplace_back(destination, time);},
                           Edge(1, std::make_pair(destination, time)));

    }
    printGroupedData(groupedData);
}


void AdjList::printGroupedData(libcuckoo::cuckoohash_map<uint64_t, Edge>& groupedData) {
    std::cout << "Printing grouped data:" <<std::endl << std::endl;
    for (auto & it : groupedData.lock_table()) {
        uint64_t source = it.first;
        const Edge& DestTime = it.second;

        // Print the source and its associated edges
        printf("Source %" PRIu64 " has %" PRIu64 " edges:\n", source, DestTime.size());
        for (const auto& edge : DestTime) {
            printf("    - to %" PRIu64 " at time %" PRIu64 "\n", edge.first, edge.second);
        }
        std::cout << std::endl;
    }
    std::cout << "-------------------------------------" << std::endl;
}


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

void AdjList::addBatch(int *source, int *destination, int *time, int numberElements) {
    for (int i = 0; i < numberElements; ++i) {
        addEdge(source[i], destination[i], time[i]);
    }
}

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
