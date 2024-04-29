//
// Created by Yağız SEMERCİOĞLU on 14.03.24.
//
#include "adj_list.h"
#include "seed_selection.h"

#include <fstream>
#include <cinttypes>

#include <map>
#include <set>
#include <algorithm> // For std::shuffle
#include <random>



#include "LPA.h"

std::map<uint64_t ,std::set<uint64_t>> labelMember;
std::map<uint64_t, uint64_t> vertexLabel;
std::vector<uint64_t> nodes;

std::vector<uint64_t> LPA::Neighbors_LPA(uint64_t u, NestedMap &G) {

    std::vector<uint64_t> neighbors;

    auto lt = G.lock_table();

    for (const auto &innerTbl: lt) {
        Edge edgeData = innerTbl.second;
        auto lt2 = edgeData.lock_table();


        for (const auto &vector: lt2) {
            if(u == vector.first){
                neighbors.insert(neighbors.end(), vector.second.begin(), vector.second.end());
            }
        }
    }
    return neighbors;
}


void LPA::randomize(){

    std::random_device randomer;
    std::mt19937 generator(randomer());
    std::shuffle(nodes.begin(), nodes.end(), generator);
}




std::map<uint64_t,uint64_t> LPA::LabelPropagation(NestedMap &G){


    auto lt = G.lock_table();
    uint64_t label = 0;
    for (const auto &time_entry: lt) {

        Edge edgeData = time_entry.second;
        auto lt2 = edgeData.lock_table();

        for (const auto &verticies: lt2) {


            label = label + 1;
            uint64_t v = verticies.first;
            std::cout<< "Nodes: "<<v<< std::endl;

            vertexLabel[v] = label;
            nodes.push_back(v);
            lt2.unlock();
            lt.unlock();
        }
    }

    randomize();
    /*for (const auto &verticies: nodes){
        std::cout << "randomlanmış nodelar"<<verticies<<std::endl;
    }*/

    bool convergence = false;

    while(!convergence){
        convergence = true;

        for (const auto &verticies: nodes) {
            std::map<uint64_t , uint64_t> labelCount;
            uint64_t v = verticies;
            std::cout<< "vertex: "<<v<< std::endl;
            std::vector<uint64_t> neighbors = Neighbors_LPA(v,G);
            std::cout<< "neighbor size ı budur: "<<neighbors.size()<< std::endl;
            for(const auto &neighbor : neighbors){
                std::cout<< "neighbor budur: "<<neighbor<< std::endl;
                labelCount[vertexLabel[neighbor]]++;
            }
            int maxCount = 0;
            int maxLabel = vertexLabel[v];
            for (auto& [label, count] : labelCount) {
                if (count > maxCount || (count == maxCount && label < maxLabel)) {
                    maxCount = count;
                    maxLabel = label;
                }
            }
            if (maxLabel != vertexLabel[v]) {
                vertexLabel[v] = maxLabel;
                convergence = false;
            }


        }
    }

    return vertexLabel;
}