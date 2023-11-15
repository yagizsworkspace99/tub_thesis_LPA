#include "adj_list.h"
#include <vector>

using namespace std;

vector<int>* AdjList::addEdge(vector<int> graph[], int source, int destination) {
    if(max(source, destination) < 10){
        graph[source].push_back(destination);
        graph[destination].push_back(source);
    }
    return graph;
}

auto AdjList:: findEdge(vector<int> graph[], int source, int destination){
    return find(graph[source].begin(), graph[source].end(), destination);
}

vector<int>* AdjList::deleteEdge(vector<int> graph[], int source, int destination){
    auto findSource = findEdge(graph, source, destination);
    auto findDestination = findEdge(graph, destination, source);

    if (findSource != graph[source].end()){
        graph[source].erase(findSource);
        graph[destination].erase(findDestination);
    }
    return graph;
}

void AdjList::printGraph(vector<int> graph[]){
    for (int i = 0; i < 20; i++) {
        for (int n: graph[i]) {
            printf("Edge between: %d and %d\n", i, n);
        }
    }
}

