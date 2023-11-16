#include "adj_list.h"
#include <vector>

auto AdjList::findEdge(int source, int destination, int time) const{
    return find(graph[source].begin(), graph[source].end(), std::make_pair(destination, time));
}

void AdjList::addEdge(int source, int destination, int time) {
    //can probably change this later to look for the max value in an inserted batch
    // and change the size manually once so this doesn't has to be done constantly
    if (std::max(source, destination) > size) {
        //might have to be +2?
        resizeGraph(std::max(source, destination) + 1);
    }

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