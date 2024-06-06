//
// Created by Yağız SEMERCİOĞLU on 21.05.24.
//

#ifndef TEMPUS_MODULARITYDENSITY_H
#define TEMPUS_MODULARITYDENSITY_H


#include "adj_list.h"
#include "seed_selection.h"
#include "LPA.h"

#include <fstream>
#include <cinttypes>

#include <map>
#include <set>
#include <algorithm> // For std::shuffle
#include <random>
#include <vector>
#include <tuple>
#include <queue>


class ModularityDensity {

public:
    std::map<uint64_t, std::vector<uint64_t>> label_verticies_Map(const std::map<uint64_t, uint64_t> &vertexLabel);
    double modularity_Q(std::map<uint64_t, std::vector<uint64_t>> label_verticies, std::map<uint64_t, uint64_t> &vertexLabel,NestedMap &G);
    std::tuple<uint64_t,uint64_t,uint64_t> in_and_out_Communities(uint64_t community,std::map<uint64_t, std::vector<uint64_t>> label_verticies, std::map<uint64_t, uint64_t> &vertexLabel,NestedMap &G);
};


#endif //TEMPUS_MODULARITYDENSITY_H
