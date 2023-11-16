#include <iostream>
#include <vector>
#include <algorithm>

#ifndef TEMPUS_ADJ_LIST_H
#define TEMPUS_ADJ_LIST_H

class AdjList{
public:
    explicit AdjList(int size){
        this->size = size;
        //might have to be +2?
        graph = new std::vector<int>[size + 1];
    }
    std::vector<int>* graph;
    int size;

    void addEdge(int source, int destination);
    [[nodiscard]] auto findEdge(int source, int destination) const;
    void deleteEdge(int source, int destination) const;
    void resizeGraph(int newSize);
    void printGraph() const;

};

#endif //TEMPUS_ADJ_LIST_H
