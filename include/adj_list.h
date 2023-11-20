#ifndef TEMPUS_ADJ_LIST_H
#define TEMPUS_ADJ_LIST_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "libcuckoo/cuckoohash_map.hh"

class AdjList{
public:
    void addFromFile(const std::string& path);
    void printGraph() const;

private:
    int maxEdge;
    libcuckoo::cuckoohash_map<int, std::vector<std::pair<int, int>>> edges;

    [[nodiscard]] auto findEdge(std::vector<std::pair<int, int>> edge, int source, int destination, int time) const;
    void addEdge(int source, int destination, int time);
    void deleteEdge(int source, int destination, int time) const;
    void resizeGraph(int newSize);
    void sortByTime();
    void sortByVertex(int sources[], int destinations[], int times[], int numberElements, bool goSources);
    static bool compareTime(std::pair<int, int>, std::pair<int, int>);
    void addBatch(int sources[], int destinations[], int times[], int numberElements);
    void addBatch(int sources, std::vector<std::pair<int, int>>);
    void addBatchHelper(int sources[], int destinations[], int times[], int numberElements, bool goLeft);
    void sortBatch(int sources[], int destinations[], int times[], int numberElements);
    static bool countDistinctValues(libcuckoo::cuckoohash_map<int, int>, int numberElements);
};

#endif //TEMPUS_ADJ_LIST_H
