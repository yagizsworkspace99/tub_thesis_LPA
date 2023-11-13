#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
const int gSize = 10;


void addEdge(vector<int> graph[], int source, int destination){
    if(max(source, destination) < gSize){
        graph[source].push_back(destination);
        graph[destination].push_back(source);
    }
}

auto findEdge(vector<int> graph[], int source, int destination){
    return find(graph[source].begin(), graph[source].end(), destination);
}

void deleteEdge(vector<int> graph[], int source, int destination){
    auto findSource = findEdge(graph, source, destination);
    auto findDestination = findEdge(graph, destination, source);

    if (findSource != graph[source].end()){
        graph[source].erase(findSource);
        graph[destination].erase(findDestination);

    }
}

void printGraph(vector<int> graph[]){
    for (int i = 0; i < gSize; i++) {
        for (int n: graph[i]) {
            printf("Edge between: %d and %d\n", i, n);
        }
    }
}

int main() {

    vector<int> graph[gSize];


    addEdge(graph, 3, 6);
    addEdge(graph, 2, 6);
    addEdge(graph, 1, 6);
    addEdge(graph, 4, 6);
    deleteEdge(graph, 3, 6);

    printGraph(graph);

    return 0;
}
