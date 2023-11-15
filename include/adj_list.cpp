#include "adj_list.h"
#include <vector>

using namespace std;

void AdjList::addEdge(vector<int>* graph, int source, int destination, int size) {
    if (max(source, destination) < size) {
        graph[source].push_back(destination);
        graph[destination].push_back(source);
    }
}

auto AdjList:: findEdge(vector<int>* graph, int source, int destination){
    return find(graph[source].begin(), graph[source].end(), destination);
}

void AdjList::deleteEdge(vector<int>* graph, int source, int destination) {
    auto findSource = findEdge(graph, source, destination);
    auto findDestination = findEdge(graph, destination, source);

    if (findSource != graph[source].end()) {
        graph[source].erase(findSource);
        graph[destination].erase(findDestination);
    }
}

void AdjList::printGraph(vector<int>* graph, int size){
    for (int i = 0; i < size; i++) {
        for (int n: graph[i]) {
            printf("Edge between: %d and %d\n", i, n);
        }
    }
}

vector<int> *AdjList::resizeGraph(vector<int> *oldGraph, int oldSize, int newSize) {
    auto* newGraph = new vector<int>[newSize];
    for (int i = 0; i < oldSize; ++i) {
        newGraph[i] = oldGraph[i];
    }
    delete[] oldGraph;
    return newGraph;
}

