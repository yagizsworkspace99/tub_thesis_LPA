#include <vector>
#include "adj_list.h"

using namespace std;
int gSize = 7;

int main() {
    auto* graph = new vector<int>[gSize];

    AdjList::addEdge(graph, 3, 6, gSize);
    AdjList::addEdge(graph, 2, 6, gSize);
    AdjList::addEdge(graph, 2, 6, gSize);
    AdjList::addEdge(graph, 1, 6, gSize);
    AdjList::addEdge(graph, 4, 6, gSize);
    AdjList::deleteEdge(graph, 3, 6);

    vector<int>* newGraph = AdjList::resizeGraph(graph, gSize, 20);
    AdjList::addEdge(newGraph, 4, 15, 20);

    AdjList::printGraph(newGraph, 20);

    return 0;
}
