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
    libcuckoo::cuckoohash_map<uint64_t, std::vector<std::pair<uint64_t, uint64_t>>> edges;

    void addEdge(uint64_t source, uint64_t destination, uint64_t time);
    void deleteEdge(int source, int destination, int time) const;
    void sortByTime();
    static bool compareTime(std::pair<uint64_t, uint64_t>, std::pair<uint64_t, uint64_t>);
    void addBatchCuckoo(libcuckoo::cuckoohash_map<uint64_t, std::vector<std::pair<uint64_t, uint64_t>>>& groupedData);
    static void sortBatch(bool sortBySource, const std::vector<uint64_t>& sourceAdds, const std::vector<uint64_t>& destinationAdds,
                          const std::vector<uint64_t>& timeAdds, libcuckoo::cuckoohash_map<uint64_t, std::vector<std::pair<uint64_t, uint64_t>>>& groupedData);
    static void printGroupedData(libcuckoo::cuckoohash_map<uint64_t, std::vector<std::pair<uint64_t, uint64_t>>>& groupedData);
    void addBatch(int *source, int *destination, int *time, int numberElements);
    void rangeQuery(uint64_t start, uint64_t end, int (*func)(uint64_t,uint64_t,uint64_t));
};

#endif //TEMPUS_ADJ_LIST_H
