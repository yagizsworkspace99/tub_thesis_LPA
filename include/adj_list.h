#ifndef TEMPUS_ADJ_LIST_H
#define TEMPUS_ADJ_LIST_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "libcuckoo/cuckoohash_map.hh"

class AdjList{
public:
    void addFromFile(const std::string& path);
    void printGraph();

private:
    //time < source < list of destinations>>
    libcuckoo::cuckoohash_map<uint64_t, libcuckoo::cuckoohash_map<uint64_t, std::vector<uint64_t>>> edges;

    //TODO: std::unorderedmap<uint64_t, libcuckoo::cuckoohash_map<uint64_t, std::vector<uint64_t>>>
    //TODO: std::map<uint64_t, libcuckoo::cuckoohash_map<uint64_t, std::vector<uint64_t>>>

    void addEdge(uint64_t source, uint64_t destination, uint64_t time);
    static void addSingleEdge(uint64_t source, uint64_t destination, uint64_t time,
                       libcuckoo::cuckoohash_map<uint64_t,libcuckoo::cuckoohash_map<uint64_t, std::vector<uint64_t>>> &map);
    void deleteEdge(int source, int destination, int time) const;
    void addBatchCuckoo(libcuckoo::cuckoohash_map<uint64_t, libcuckoo::cuckoohash_map<uint64_t, std::vector<uint64_t>>>& groupedData);
    static void sortBatch(bool sortBySource, const std::vector<uint64_t>& sourceAdds, const std::vector<uint64_t>& destinationAdds,
                          const std::vector<uint64_t>& timeAdds,
                          libcuckoo::cuckoohash_map<uint64_t,libcuckoo::cuckoohash_map<uint64_t, std::vector<uint64_t>>>& groupedData);
    static void printGroupedData(libcuckoo::cuckoohash_map<uint64_t,libcuckoo::cuckoohash_map<uint64_t, std::vector<uint64_t>>>& groupedData);
    void addBatch(int *source, int *destination, int *time, int numberElements);
    void rangeQuery(uint64_t start, uint64_t end, int (*func)(uint64_t,uint64_t,uint64_t));
};

#endif //TEMPUS_ADJ_LIST_H
