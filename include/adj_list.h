
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
    void rangeQuery(int start, int end, int (*func)(int,int,int)) ;

private:
    int maxEdge;
    static libcuckoo::cuckoohash_map<int, std::vector<std::pair<int, int>>> edges;

    [[nodiscard]] auto findEdge(std::vector<std::pair<int, int>> edge, int source, int destination, int time) const;
    void addEdge(int source, int destination, int time);
    void deleteEdge(int source, int destination, int time) const;
    void sortByTime();
    static bool compareTime(std::pair<int, int>, std::pair<int, int>);
    void addBatchCuckoo(libcuckoo::cuckoohash_map<int, std::vector<std::pair<int, int>>>& groupedData);
    static void sortBatch(int sortFlag, const std::vector<int>& sourceAdds, const std::vector<int>& destinationAdds, const std::vector<int>& timeAdds, libcuckoo::cuckoohash_map<int, std::vector<std::pair<int, int>>>& groupedData);
    static void printGroupedData(libcuckoo::cuckoohash_map<int, std::vector<std::pair<int, int>>>& groupedData);
    void addBatch(int *source, int *destination, int *time, int numberElements);
};

#endif //TEMPUS_ADJ_LIST_H
