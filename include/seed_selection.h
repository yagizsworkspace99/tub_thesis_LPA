//
// Created by Yağız SEMERCİOĞLU on 06.02.24.
//

#ifndef TEMPUS_SEED_SELECTION_H
#define TEMPUS_SEED_SELECTION_H

#include <set>
#include "libcuckoo/cuckoohash_map.hh"
#include <string>



class seed_selection {
public:
    std::vector<uint64_t> NeighborsG(uint64_t u, NestedMap &G);
    double Score(std::vector<uint64_t> v,std::vector<uint64_t> u,NestedMap &G);
    std::vector<uint64_t> commonNbrs(std::vector<uint64_t> v,std::vector<uint64_t> u,NestedMap &G);
    double hubPromotedIndex(std::vector<uint64_t> v,std::vector<uint64_t> u,NestedMap &G);
    double lhnIndex(std::vector<uint64_t> v,std::vector<uint64_t> u,NestedMap &G);
    double preferentialAttachment(std::vector<uint64_t> v,std::vector<uint64_t> u,NestedMap &G);
    bool Converge(std::vector<uint64_t> u,NestedMap &G);
    std::vector<uint64_t> link_ss(const std::string& Type, NestedMap& G);
    std::vector<uint64_t> coloring_ss(NestedMap& G);
    uint64_t get_random_element(const std::set<uint64_t> set);

    std::vector<uint64_t> neighbors_ss(uint64_t vertex, uint64_t time, NestedMap &G);
};
#endif //TEMPUS_SEED_SELECTION_H
