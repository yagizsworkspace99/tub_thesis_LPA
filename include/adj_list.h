#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#ifndef TEMPUS_ADJ_LIST_H
#define TEMPUS_ADJ_LIST_H

class AdjList{
public:
    static void addEdge(vector<int>* graph, int source, int destination, int size);
    static void deleteEdge(vector<int>* graph, int source, int destination);
    static auto findEdge(vector<int>* graph, int source, int destination);
    static void printGraph(vector<int>* graph, int size);
    static vector<int>* resizeGraph(vector<int>* oldGraph, int oldSize, int newSize);
};

#endif //TEMPUS_ADJ_LIST_H
