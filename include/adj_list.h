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
        graph = new std::vector<std::pair<int, int>>[size + 1];
    }

    void addFromFile(const std::string& path);
    void printGraph() const;

private:
    //Array of Vectors containing pairs
    std::vector<std::pair<int, int>>* graph;
    int size;

    [[nodiscard]] auto findEdge(int source, int destination, int time) const;
    void addEdge(int source, int destination, int time) const;
    void deleteEdge(int source, int destination, int time) const;
    void resizeGraph(int newSize);
    void sortByTime() const;
    static bool compareTime(std::pair<int, int>, std::pair<int, int>);
};

#endif //TEMPUS_ADJ_LIST_H
