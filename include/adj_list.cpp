#include "adj_list.h"
#include <vector>
#include <fstream>

auto AdjList::findEdge(int source, int destination, int time) const{
    return find(graph[source].begin(), graph[source].end(), std::make_pair(destination, time));
}

void AdjList::addEdge(int source, int destination, int time) const {
    auto findSource = findEdge(source, destination, time);
    auto findDestination = findEdge(destination, source, time);
    if (findSource == graph[source].end() || findDestination == graph[destination].end()) {
        graph[source].emplace_back(destination, time);
        graph[destination].emplace_back(source, time);
    }
}

void AdjList::deleteEdge(int source, int destination, int time) const {
    auto findSource = findEdge(source, destination, time);
    auto findDestination = findEdge(destination, source, time);

    if (findSource != graph[source].end() && findDestination != graph[destination].end()) {
        graph[source].erase(findSource);
        graph[destination].erase(findDestination);
    }
}

void AdjList::resizeGraph(int newSize) {
    auto newGraph = new std::vector<std::pair<int, int>>[newSize];

    for (int i = 0; i < size; ++i) {
        newGraph[i] = graph[i];
    }

    delete[] graph;
    graph = newGraph;
    size = newSize;
}

void AdjList::printGraph() const {
    for (int i = 0; i < size; i++) {
        for (auto& n: graph[i]) {
            printf("Edge between: %d and %d at time %d\n", i, n.first, n.second);
        }
    }
}

void AdjList::addFromFile(const std::string& path) {
    std::ifstream file(path);
    if(file.is_open()){
        int source, destination, time;
        std::string command;

        int maxValue = size;
        while(file >> command >> source >> destination >> time){
            int localMax = std::max(source, destination);
            if(localMax > maxValue) maxValue = localMax;
        }

        //might have to be +2
        if(maxValue > size) resizeGraph(maxValue + 1);

        file.clear();
        file.seekg(0, std::ifstream::beg);

        while(file >> command >> source >> destination >> time){
            if (command == "add") addEdge(source, destination, time);
            if (command == "delete") deleteEdge(source, destination, time);
        }
    }
    file.close();

    sortByTime();
}

//O(N log N)
void AdjList::sortByTime() const {
    for (int i = 0; i < size; ++i) {
        std::sort(graph[i].begin(), graph[i].end(), compareTime);
    }
}

bool AdjList::compareTime(std::pair<int, int> i1, std::pair<int, int> i2) {
    return (i1.second < i2.second);
}


