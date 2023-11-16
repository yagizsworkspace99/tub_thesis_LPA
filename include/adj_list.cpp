#include "adj_list.h"
#include <vector>

void AdjList::addEdge(int source, int destination) {
    //can probably change this later to look for the max value in an inserted batch
    // and change the size manually once so this doesn't has to be done constantly
    if (std::max(source, destination) > size) {
        //might have to be +2?
        resizeGraph(std::max(source, destination) + 1);
    }
    graph[source].push_back(destination);
    graph[destination].push_back(source);
}

auto AdjList::findEdge(int source, int destination) const{
    return find(graph[source].begin(), graph[source].end(), destination);
}

void AdjList::deleteEdge(int source, int destination) const {
    auto findSource = findEdge(source, destination);
    auto findDestination = findEdge(destination, source);

    if (findSource != graph[source].end()) {
        graph[source].erase(findSource);
        graph[destination].erase(findDestination);
    }
}

void AdjList::resizeGraph(int newSize) {
    auto newGraph = new std::vector<int>[newSize];

    for (int i = 0; i < size; ++i) {
        newGraph[i] = graph[i];
    }

    delete[] graph;
    graph = newGraph;

    size = newSize;
}

void AdjList::printGraph() const {
    for (int i = 0; i < size; i++) {
        for (int& n: graph[i]) {
            printf("Edge between: %d and %d\n", i, n);
        }
    }
}