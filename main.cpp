#include <vector>
#include "adj_list.h"

using namespace std;
const int gSize = 20;


int main() {
    vector<int> graph[gSize];

    vector<int>* g2 = AdjList::addEdge(graph, 3, 6);
    vector<int>* g3 = AdjList::addEdge(g2, 2, 6);
    vector<int>* g4 = AdjList::addEdge(g3, 2, 6);
    vector<int>* g5 = AdjList::addEdge(g4, 1, 6);
    vector<int>* g6 = AdjList::addEdge(g5, 4, 6);
    vector<int>* g7 = AdjList::deleteEdge(g6, 3, 6);

    AdjList::printGraph(g7);

    return 0;
}
