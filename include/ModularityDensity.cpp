//
// Created by Yağız SEMERCİOĞLU on 21.05.24.
//

#include "ModularityDensity.h"


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


LPA lpa;


std::map<uint64_t, std::vector<uint64_t>> ModularityDensity::label_verticies_Map(const std::map<uint64_t, uint64_t> &vertexLabel) {
    std::map<uint64_t, std::vector<uint64_t>> label_vertices;

    for (const auto &pair : vertexLabel) {
        uint64_t vertex = pair.first;
        uint64_t label = pair.second;
        label_vertices[label].push_back(vertex);
    }

    return label_vertices;
}



double ModularityDensity::modularity_Q(std::map<uint64_t, std::vector<uint64_t>> label_verticies, std::map<uint64_t, uint64_t> &vertexLabel,NestedMap &G){
    double q = 0.0;
    //static_cast<double>

    for(auto &label : label_verticies ){
        auto[E_in, E_out, E] = in_and_out_Communities(label.first,label_verticies,vertexLabel,G);
        auto E_in_d = static_cast<double>(E_in);
        auto E_out_d = static_cast<double>(E_out);
        auto E_d = static_cast<double>(E);

        q+= (E_in_d / E_d) - std::pow((2 * E_in_d + E_out_d) / (2 * E_d), 2);

    }

    return q;
}

std::tuple<uint64_t,uint64_t,uint64_t> ModularityDensity::in_and_out_Communities(uint64_t community,std::map<uint64_t, std::vector<uint64_t>> label_verticies, std::map<uint64_t, uint64_t> &vertexLabel,NestedMap &G){

    uint64_t E_in = 0;
    uint64_t E_out = 0;
    uint64_t E = 0;

    for(auto &v : label_verticies[community] ){

        auto lt = G.lock_table();
        for (const auto &innerTbl: lt) {
            Edge edgeData = innerTbl.second;
            std::vector<uint64_t> nbrs = lpa.get_neighbors(v,edgeData);
            E += nbrs.size();
            for(const auto &second_vertex: nbrs){

                if(vertexLabel[second_vertex] == community) {
                    E_in++;
                } else if(vertexLabel[second_vertex] != community){
                    E_out++;

                }

            }

        }

    }

    return std::make_tuple(E_in,E_out,E);

}