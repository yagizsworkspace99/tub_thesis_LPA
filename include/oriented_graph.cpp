//
// Created by Yağız SEMERCİOĞLU on 19.04.24.
//

#include "adj_list.h"
#include "seed_selection.h"

#include <fstream>
#include <cinttypes>

#include <map>
#include <set>
#include <algorithm> // For std::shuffle
#include <random>
#include <vector>
#include <tuple>



#include "LPA.h"

#include "oriented_graph.h"
#include <queue>

LPA lpa;

AdjList myList;
auto insertFunc = myList.getInsertEdgeDirectedFunction();

Edge Gstar; //instead of this should I use Nestedmap???

void oriented_graph::Updates(std::vector<std::tuple<uint64_t, uint64_t,uint64_t, int>> InsDel){
    for (const auto [source,destination,time,type] : InsDel) {
        if(type == 0){
            DC_Orient_Del(Gstar,source ,destination);
        }else if (type == 1){
            DC_Orient_Ins(Gstar,source ,destination);
        }
    }
}


std::vector<uint64_t> oriented_graph::nbrMinus(uint64_t u,Edge &edge){
    std::vector<uint64_t> nbrMinusNodes;

    for(int source = 0; source<=Gstar.size(); source++){
        bool flag = false;
        edge.find_fn(source,
                  [&flag, &u](std::vector<uint64_t> &d) {
                      flag = (std::find(d.begin(), d.end(), u) != d.end());
                  });
        if(flag){
            nbrMinusNodes.push_back(source);
        }
    }
    return nbrMinusNodes;
}



void oriented_graph::insertEdgeDirected_OCG(uint64_t source, uint64_t destination, Edge &edge) {
    edge.upsert(source,
             [&destination](std::vector<uint64_t> &d) { d.push_back(destination); },
             std::vector<uint64_t>{destination});
}
void oriented_graph::deleteEdgeDirected_OCG(uint64_t source, uint64_t destination, Edge &edge){
    edge.update_fn(source, [&destination](std::vector<uint64_t> &d) {
        d.erase(std::find(d.begin(), d.end(), destination));

    });
}


Edge oriented_graph::OCG(NestedMap &G) {
    auto lt = G.lock_table();
    for (const auto &time_entry: lt) {
        Edge edgeData = time_entry.second;
        auto lt2 = edgeData.lock_table();
        for (const auto &verticies: lt2) {
            uint64_t v = verticies.first; // vertex v
            for(const auto &n : verticies.second){ //neighbors
                for (const auto &verticiesAgain: lt2){
                    if(verticiesAgain.first == n){ // neighbor in first column
                        if(verticies.second.size() > verticiesAgain.second.size() or (verticies.second.size() == verticiesAgain.second.size() and verticies.first < verticiesAgain.first)){
                            insertEdgeDirected_OCG(verticies.first,verticiesAgain.first,Gstar);

                        }

                    }
                }

            }


        }


    }
    return Gstar;
}

bool oriented_graph::contains(std::queue<uint64_t> q, uint64_t value) {
    while (!q.empty()) {
        if (q.front() == value) {
            return true;
        }
        q.pop();
    }
    return false;
}



void oriented_graph::DC_Orient_Ins(Edge &Gs,uint64_t u ,uint64_t v){
    std::queue<uint64_t> q;
    std::vector<uint64_t> S = OCG_Ins(Gs, u, v);
    for(auto w : S){
        q.push(w);
    }
    CAN(Gs,q);

}

void oriented_graph::DC_Orient_Del(Edge &Gs,uint64_t u ,uint64_t v){
    std::queue<uint64_t> q;
    std::vector<uint64_t> S = OCG_Del(Gs, u, v);
    for(auto w : S){
        q.push(w);
    }
    CAN(Gs, q);

}

void oriented_graph::CAN(Edge &Gs, std::queue<uint64_t> q){
    while(!q.empty()){
        uint64_t u = q.front();
        q.pop();
        std::vector<uint64_t> C = CollectColor(u);
        bool b = AssignColor(myList.getEdges(),u,C);
        NotifyColor(u,b,q);
    }

}

std::vector<uint64_t> oriented_graph::CollectColor(uint64_t u){
    std::vector<uint64_t> C;
    for(uint64_t v : nbrMinus(u,Gstar)){
        C.push_back(lpa.vertexLabel[v]);
    }

    return C;
}

bool oriented_graph::AssignColor(NestedMap &G, uint64_t u, std::vector<uint64_t> C) {
    std::vector<uint64_t> Color;
    uint64_t u_table;
    auto lt = G.lock_table();
    for (const auto &time_entry: lt) {
        Edge edgeData = time_entry.second;
        auto lt2 = edgeData.lock_table();
        for (const auto &verticies: lt2) {
            if(verticies.first == u){
                for (int i = 0; i<=verticies.second.size(); i++){
                    Color.push_back(i);
                }
            }
        }


        uint64_t Cnew;
        for(uint64_t c : Color){
            if(std::find(C.begin(), C.end(), c) == C.end()){
                Cnew = c;
                break;
            }

        }

        if(Cnew != lpa.vertexLabel[u]){
            lpa.vertexLabel[u] = Cnew;
            return true;
        }else{
            return false;
        }

    }
}

void oriented_graph::NotifyColor(uint64_t u, bool b, std::queue<uint64_t> q){
    if(b){
        auto lt = Gstar.lock_table();
        for (const auto &source: lt){
            if(source.first==u){
                for (const auto &v: source.second){ //nbr+
                    if(contains(q,v)){
                        q.push(v);
                    }
                }
            }

        }
    }

}

std::vector<uint64_t> oriented_graph::OCG_Ins(Edge &Gs, uint64_t u, uint64_t v){
    std::vector<uint64_t> S;
    S.push_back(u);
    S.push_back(v);
    insertEdgeDirected_OCG(u,v,Gstar);
    for (auto &uı: nbrMinus(u,Gs)){
        auto lt = Gstar.lock_table();
        for (const auto &source1: lt){
            if(source1.first==u){
                for (const auto &source2: lt){
                    if(source2.first==uı){
                        if(source1.second.size() > source2.second.size() or (source1.second.size() == source2.second.size() and source1.first < source2.first)){
                            deleteEdgeDirected_OCG(uı,u,Gs);
                            insertEdgeDirected_OCG(u,uı,Gs);
                            S.push_back(uı);
                        }

                    }
                }

            }

        }
    }

    for (auto &vı: nbrMinus(v,Gs)){
        auto lt = Gstar.lock_table();
        for (const auto &source3: lt){
            if(source3.first==v){
                for (const auto &source4: lt){
                    if(source4.first==vı){
                        if(source3.second.size() > source4.second.size() or (source3.second.size() == source4.second.size() and source3.first < source4.first)){
                            deleteEdgeDirected_OCG(vı,v,Gs);
                            insertEdgeDirected_OCG(v,vı,Gs);
                            S.push_back(vı);

                        }

                    }
                }

            }

        }
    }

    return S;
}

std::vector<uint64_t> oriented_graph::OCG_Del(Edge &Gs, uint64_t u, uint64_t v){
    std::vector<uint64_t> S;
    S.push_back(u);
    S.push_back(v);
    deleteEdgeDirected_OCG(u,v,Gstar);

    auto lt = Gstar.lock_table();
    for (const auto &s: lt){
        if(s.first==u){
            for (auto &uı: s.second){
                for (const auto &source1: lt){
                    if(source1.first==u){
                        for (const auto &source2: lt){
                            if(source2.first==uı){
                                if(source1.second.size() > source2.second.size() or (source1.second.size() == source2.second.size() and source1.first < source2.first)){
                                    deleteEdgeDirected_OCG(uı,u,Gs);
                                    insertEdgeDirected_OCG(u,uı,Gs);
                                    S.push_back(uı);
                                }

                            }
                        }

                    }

                }
            }
        }

    }


    for (const auto &s: lt){
        if(s.first==v){
            for (auto &vı: s.second){
                for (const auto &source3: lt){
                    if(source3.first==v){
                        for (const auto &source4: lt){
                            if(source4.first==vı){
                                if(source3.second.size() > source4.second.size() or (source3.second.size() == source4.second.size() and source3.first < source4.first)){
                                    deleteEdgeDirected_OCG(vı,v,Gs);
                                    insertEdgeDirected_OCG(v,vı,Gs);
                                    S.push_back(vı);

                                }

                            }
                        }

                    }

                }
            }
        }
    }




    return S;
}











