//
// Created by Yağız SEMERCİOĞLU on 14.03.24.
//

#ifndef TEMPUS_LPA_H
#define TEMPUS_LPA_H

#include <map>
#include <set>
#include <algorithm> // For std::shuffle
#include <random>

#include "libcuckoo/cuckoohash_map.hh"



class LPA {




public:
    std::map<uint64_t, uint64_t> vertexLabel;

    std::map<uint64_t,uint64_t>  LabelPropagation(NestedMap &G);
    void randomize();
private:
    std::vector<uint64_t> Neighbors_LPA(uint64_t u, NestedMap &G);
};


#endif //TEMPUS_LPA_H
