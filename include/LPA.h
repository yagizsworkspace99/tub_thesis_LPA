//
// Created by Yağız SEMERCİOĞLU on 14.03.24.
//

#ifndef TEMPUS_LPA_H
#define TEMPUS_LPA_H

#include <map>
#include <set>
#include <algorithm> // For std::shuffle
#include <random>

#include <vector>
#include <tuple>
#include <queue>

#include "libcuckoo/cuckoohash_map.hh"

//std::map<uint64_t, uint64_t> vertexLabel;

class LPA {




public:
    std::map<uint64_t, uint64_t> vertexLabel;
    Edge Gstar;


    void  LabelPropagation(std::vector<uint64_t> seeds, NestedMap &G);
    void randomize();

    bool contains(std::queue<uint64_t> q, uint64_t value);
    Edge OCG(NestedMap &G);
    void insertEdgeDirected_OCG(uint64_t source, uint64_t destination, Edge &edge);
    void deleteEdgeDirected_OCG(uint64_t source, uint64_t destination, Edge &edge);
    std::vector<std::tuple<uint64_t, uint64_t, uint64_t ,int>> InsDel; //0 for deletion, 1 for insertion
    void Updates(std::vector<std::tuple<uint64_t, uint64_t,uint64_t, int>> InsDel);
    std::vector<uint64_t> nbrMinus(uint64_t u,Edge &edge);
    uint64_t findMaxRepeatedNumber(const std::vector<uint64_t>& vec);



    void DC_Orient_Ins(Edge &Gstar,uint64_t u ,uint64_t v);
    void DC_Orient_Del(Edge &Gs,uint64_t u ,uint64_t v);
    void CAN(Edge &Gs, std::queue<uint64_t> &q);
    std::vector<uint64_t> CollectColor(uint64_t u);
    bool AssignColor(NestedMap &G, uint64_t u, std::vector<uint64_t> C);
    void NotifyColor(uint64_t u, bool b, std::queue<uint64_t> &q);
    std::vector<uint64_t> OCG_Ins(Edge &Gs, uint64_t u, uint64_t v);
    std::vector<uint64_t> OCG_Del(Edge &Gs, uint64_t u, uint64_t v);
    Edge& GSreturn();
    std::vector<uint64_t> get_neighbors( uint64_t vertex, Edge &Gs);
    std::map<uint64_t, uint64_t>& get_colors();

private:
    static std::vector<uint64_t> Neighbors_LPA(uint64_t u, NestedMap &G);



};


#endif //TEMPUS_LPA_H
