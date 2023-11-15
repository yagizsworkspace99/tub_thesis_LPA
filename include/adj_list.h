#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#ifndef TEMPUS_ADJ_LIST_H
#define TEMPUS_ADJ_LIST_H

class AdjList{
public:
    static vector<int>* addEdge(vector<int> graph[], int source, int destination);
    static vector<int>* deleteEdge(vector<int> graph[], int source, int destination);
    static auto findEdge(vector<int> graph[], int source, int destination);
    static void printGraph(vector<int> graph[]);
};

#endif //TEMPUS_ADJ_LIST_H
