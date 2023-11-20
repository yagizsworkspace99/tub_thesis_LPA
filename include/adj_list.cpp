#include "adj_list.h"
#include <vector>
#include <fstream>

typedef std::vector<std::pair<int, int>> Edge;


void AdjList::addEdge(int source, int destination, int time) {

    //filters out duplicates
    bool flag;
    edges.find_fn(source,
                  [&destination, &time, &flag](Edge &e){flag = (find(e.begin(), e.end(), std::make_pair(destination, time)) != e.end());});

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
    for (int i = 0; i < maxEdge + 1; i++) {
        Edge out;
        if (edges.find(i, out)){
            std::cout << "Edge " << i << " contains " << out.size() << " edges." << std::endl;
            for (auto& n: out) {
                printf("    - between: %d and %d at time %d\n", i, n.first, n.second);
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
        libcuckoo::cuckoohash_map<int, int> leftTable;
        libcuckoo::cuckoohash_map<int, int> rightTable;

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
        bool flag = (leftTable.size()<rightTable.size());
        int sortFlag;

        if (flag) {
            // If the condition is true, set sortFlag to 0 --> grouping occurs using source vertices
            sortFlag = 0;
        } else {
            // If the condition is false, set sortFlag to 1 --> grouping occurs using destinations vertices
            sortFlag = 1;
        }

        file.clear();
        file.seekg(0, std::ifstream::beg);

        std::vector<int> sourceAdds(noOfAdds), destinationAdds(noOfAdds), timeAdds(noOfAdds);
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

        //Create new hash map, keys are source vertices and values are vectors of integer pairs (destination, time). This is then filled by sortBatch function.
        libcuckoo::cuckoohash_map<int, std::vector<std::pair<int, int>>> groupedData;

        sortBatch(sortFlag, sourceAdds, destinationAdds, timeAdds, groupedData);

        // Iterate through the groupedData and add edges to AL --> How does one iterate through the hash map???
        for (auto it = groupedData.begin(); it != groupedData.end(); ++it) {
            int source = it->first;
            const auto& edges = it->second;

            // Iterate over each pair in edges
            for (const auto& edge : edges) {
                // Add edge to the adjacency list
                addEdge(source, edge.first, edge.second);
            }
        }

        //countDistinctValues(sourceAdds, destinationAdds, noOfAdds);
        //addBatch(sourceAdds, destinationAdds, timeAdds, noOfAdds);
        //addBatchHelper(sourceAdds, destinationAdds, timeAdds, noOfAdds, true);

    }
    file.close();

    sortByTime();
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

bool AdjList::compareTime(std::pair<int, int> i1, std::pair<int, int> i2) {
    return (i1.second < i2.second);
}

/*
void AdjList::addBatch(int *source, int *destination, int *time, int numberElements) {
    for (int i = 0; i < numberElements; ++i) {
        addEdge(source[i], destination[i], time[i]);
    }
}
*/

//ignore
void AdjList::addBatchHelper(int *sources, int *destinations, int *times, int numberElements, bool goLeft) {
    int last = -1;
    std::vector<std::pair<int, int>> cur;
    for (int i = 0; i < numberElements; ++i) {
        if (sources[i] != last){
            addBatch(sources[i], cur);
            cur.empty();
        }
        cur.emplace_back(destinations[i], times[i]);
        last = sources[i];
    }
}

void AdjList::addBatch(int sources, std::vector<std::pair<int, int>> v) {
    for (auto n: v) {
        addEdge(sources, n.first, n.second);
    }
}

void AdjList::sortBatch(int sortFlag, const std::vector<int>& sourceAdds, const std::vector<int>& destinationAdds, const std::vector<int>& timeAdds, libcuckoo::cuckoohash_map<int, std::vector<std::pair<int, int>>>& groupedData) {

    // Determine which vector to use based on the sort flag
    const std::vector<int>& relevantVector = (sortFlag == 0) ? sourceAdds : destinationAdds;

    // Determine the number of iterations based on the relevant vector
    size_t numIterations = relevantVector.size();

    // Group edges by source vertex using hash map
    for (size_t i = 0; i < numIterations; ++i) {
        int vertex = relevantVector[i];
        int destination = (sortFlag == 0) ? destinationAdds[i] : sourceAdds[i];
        int time = timeAdds[i];

        // If source is not present in groupedData, it is automatically inserted
        groupedData[vertex].emplace_back(destination, time);
    }

    // Prints out hash map
    processGroupedData(groupedData);

}

void AdjList::processGroupedData(int source, const std::vector<std::pair<int, int>>& edges) {
    // Example: Print the grouped data
    std::cout << "Source " << source << " has " << edges.size() << " edges:" << std::endl;
    for (const auto& edge : edges) {
        std::cout << "    - to " << edge.first << " at time " << edge.second << std::endl;
    }
    std::cout << std::endl;
}




/*
void AdjList::sortBatch(int *sources, int *destinations, int *times, int numberElements) {
    bool goSource = countDistinctValues(sources, destinations, numberElements);

}

bool AdjList::countDistinctValues(libcuckoo::cuckoohash_map<int, int> table, int numberElements) {
    table.
    for (int i = 0; i < table.size(); ++i) {

    }
    printf(" sources: %zu, destinations: %zu\n", sourcesMap.size(), destinationMap.size());
    return sourcesMap.size()<destinationMap.size();
}

void AdjList::sortByVertex(int *sources, int *destinations, int *times, int numberElements, bool goSources) {
    std::vector<std::pair<int, int>> temp[10];
    
}


*/