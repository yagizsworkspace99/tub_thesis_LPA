//
// Created by Yağız SEMERCİOĞLU on 19.04.24.
//

#ifndef TEMPUS_ORIENTED_GRAPH_H
#define TEMPUS_ORIENTED_GRAPH_H

#include <map>
#include <set>
#include <algorithm> // For std::shuffle
#include <random>
#include <queue>

#include "libcuckoo/cuckoohash_map.hh"


class oriented_graph {
public:
    bool contains(std::queue<uint64_t> q, uint64_t value);
    Edge OCG(NestedMap &G);
    void insertEdgeDirected_OCG(uint64_t source, uint64_t destination, Edge &edge);
    void deleteEdgeDirected_OCG(uint64_t source, uint64_t destination, Edge &edge);
    std::vector<std::tuple<uint64_t, uint64_t, uint64_t ,int>> InsDel; //0 for deletion, 1 for insertion
    void Updates(std::vector<std::tuple<uint64_t, uint64_t,uint64_t, int>> InsDel);
    std::vector<uint64_t> nbrMinus(uint64_t u,Edge &edge);



    void DC_Orient_Ins(Edge &Gstar,uint64_t u ,uint64_t v);
    void DC_Orient_Del(Edge &Gs,uint64_t u ,uint64_t v);
    void CAN(Edge &Gs, std::queue<uint64_t> q);
    std::vector<uint64_t> CollectColor(uint64_t u);
    bool AssignColor(NestedMap &G, uint64_t u, std::vector<uint64_t> C);
    void NotifyColor(uint64_t u, bool b, std::queue<uint64_t> q);
    std::vector<uint64_t> OCG_Ins(Edge &Gs, uint64_t u, uint64_t v);
    std::vector<uint64_t> OCG_Del(Edge &Gs, uint64_t u, uint64_t v);


};


#endif //TEMPUS_ORIENTED_GRAPH_H
