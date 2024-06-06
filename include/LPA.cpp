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
#include <vector>
#include <tuple>
#include <queue>



#include "LPA.h"

std::map<uint64_t ,std::set<uint64_t>> labelMember; //which nodes does a label has
std::map<uint64_t, uint64_t> vertexLabel; //list of node with the label of the node

std::vector<uint64_t> nodes;
std::vector<std::tuple<uint64_t, uint64_t,uint64_t, int>> InsDel;
AdjList myList;
auto insertFunc = myList.getInsertEdgeDirectedFunction();

Edge Gstar; //instead of this should I use Nestedmap???


/**
 * finds the neighbors of the node
 * @param u node of the edge
 * @param G map
 * @return list of neighbors of the node
 */
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


/**
 * randomizes the order of the nodes so that the order does not affect the result
 */
void LPA::randomize(){

    std::random_device randomer;
    std::mt19937 generator(randomer());
    std::shuffle(nodes.begin(), nodes.end(), generator);
}


/**
 * state of art label propagation algorithm assigns a label to each node
 * @param G map
 * @return list of node with the label of the node
 */
void LPA::LabelPropagation(std::vector<uint64_t> seeds ,NestedMap &G){


    auto lt = G.lock_table();
    Edge edgeData = lt.find(0)->second;
    uint64_t label = 0;

    auto lt2 = edgeData.lock_table();
    for (const auto &verticies: lt2) {
        label = label + 1;
        uint64_t v = verticies.first;
        vertexLabel[v] = label;
        nodes.push_back(v);
        lt2.unlock();
        lt.unlock();
    }


    randomize();

    bool convergence = false;

    while(!convergence){
        convergence = true;

        for (const auto &verticies: nodes) {
            std::map<uint64_t , uint64_t> labelCount;
            uint64_t v = verticies;

            if (std::find(seeds.begin(), seeds.end(), v) != seeds.end()) {
                continue;
            }


            std::vector<uint64_t> neighbors = Neighbors_LPA(v,G);

            for(const auto &neighbor : neighbors){
                //std::cout<< "neighbor budur: "<<neighbor<< std::endl;
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


    //For test of the seed selection maintain
    /*vertexLabel[0] = 3;
    vertexLabel[1] = 4;
    vertexLabel[2] = 5;
    vertexLabel[3] = 1;
    vertexLabel[4] = 2;
    vertexLabel[5] = 0;
    vertexLabel[6] = 2;
    vertexLabel[7] = 1;
    vertexLabel[8] = 0;
    vertexLabel[9] = 1;
    vertexLabel[10] = 0;
    vertexLabel[11] = 1;*/
}


/**
 *  the function calls the corresponding function depending on the update type
 * @param InsDel passed edge time and update type(insertion or deletion) from adj_list insertion/deletion functions
 */


/**
 * the dominating neighbors of a node
 * or the nodes that are directed to the given node
 * @param u the node
 * @param edge oriented graph
 * @return list of dominating neighbors
 */
std::vector<uint64_t> LPA::nbrMinus(uint64_t u,Edge &edge){

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

/**
 * inserts a directed edge to the oriented graph
 * @param source node of the edge
 * @param destination node of the edge
 * @param edge oriented graph
 */
void LPA::insertEdgeDirected_OCG(uint64_t source, uint64_t destination, Edge &edge) {
    edge.upsert(source,
                [&destination](std::vector<uint64_t> &d) { d.push_back(destination); },
                std::vector<uint64_t>{destination});

}


/**
 * deletes a directed edge from the oriented graph
 * @param source node of the edge
 * @param destination node of the edge
 * @param edge oriented graph
 */
void LPA::deleteEdgeDirected_OCG(uint64_t source, uint64_t destination, Edge &edge){
    edge.update_fn(source, [&destination](std::vector<uint64_t> &d) {
        d.erase(std::find(d.begin(), d.end(), destination));
    });
}

/**
 * creates a directed graph from the actual undirected graph, based on the total order of nodes
 * @param G the undirected graph map
 * @return oriented graph
 */
Edge LPA::OCG(NestedMap &G) {
    //auto lt = G.find(0).lock_table();

    auto lt = G.lock_table();
    Edge edgeData = lt.find(0)->second;
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



    return Gstar;
}


/**
 * checks if a given node is in the queue
 * @param q queue
 * @param value node
 * @return true or false
 */
bool LPA::contains(std::queue<uint64_t> q, uint64_t value) {
    while (!q.empty()) {
        if (q.front() == value) {
            return true;
        }
        q.pop();
    }
    return false;
}

/**
 * if the update is an insertion it calls OCG_Ins and puts the nodes that needs a recalculation into the queue
 * @param Gs oriented graph
 * @param u node of the edge
 * @param v node of the edge
 */
void LPA::DC_Orient_Ins(Edge &Gs,uint64_t u ,uint64_t v){
    std::queue<uint64_t> q;
    std::vector<uint64_t> S = OCG_Ins(Gs, u, v);
    for(auto w : S){
        //std::cout<< "W budur : "<<w <<std::endl;
        q.push(w);
    }
    CAN(Gs,q);
}


/**
 * does the same thing in DC_Orient_Ins for a deletion
 * @param Gs oriented graph
 * @param u node of the edge
 * @param v node of the edge
 */
void LPA::DC_Orient_Del(Edge &Gs,uint64_t u ,uint64_t v){
    std::queue<uint64_t> q;
    std::vector<uint64_t> S = OCG_Del(Gs, u, v);
    for(auto w : S){
        q.push(w);
    }
    CAN(Gs, q);
}


/**
 * a step that calls the CollectColor, AssignColor and NotifyColor functions in order
 * @param Gs oriented graph
 * @param q queue
 */
void LPA::CAN(Edge &Gs, std::queue<uint64_t> &q){
    while(!q.empty()){
        uint64_t u = q.front();

        q.pop();
        std::vector<uint64_t> C = CollectColor(u);
        bool b = AssignColor(myList.getEdges(),u,C);
        NotifyColor(u,b,q);
    }

}


/**
 * collects the colors of all dominating neighbors of the node
 * @param u node of the edge
 * @return list of collected colors
 */
std::vector<uint64_t> LPA::CollectColor(uint64_t u){
    std::vector<uint64_t> C;
    for(uint64_t v : nbrMinus(u,Gstar)){
        C.push_back(vertexLabel[v]);

    }
    return C;
}


/**
 * assigns the best possible color to the node if necessary
 * true if the node has a new color
 * false if the color of the node didn't change
 * @param G the actual undirected graph map
 * @param u node of the edge
 * @param C list colors of all dominating neighbors of the node
 * @return true of false
 */
bool LPA::AssignColor(NestedMap &G, uint64_t u, std::vector<uint64_t> C) {



    uint64_t Cnew = findMaxRepeatedNumber(C);

    if(C.size() == 0){
        return false;
    }
    else if(Cnew != vertexLabel[u]){
        vertexLabel[u] = Cnew;
        return true;
    }else{
        return false;
    }

}


uint64_t LPA::findMaxRepeatedNumber(const std::vector<uint64_t>& vec) { //değiştir
    std::unordered_map<uint64_t, size_t> countMap;
    for (const auto& num : vec) {
        countMap[num]++;
    }

    uint64_t maxNumber = 0;
    size_t maxCount = 0;

    for (const auto& pair : countMap) {
        if (pair.second > maxCount) {
            maxCount = pair.second;
            maxNumber = pair.first;
        }
    }

    return maxNumber;
}


/*bool LPA::AssignColor(NestedMap &G, uint64_t u, std::vector<uint64_t> C) {
    std::vector<uint64_t> Color;


    for (int i = 0; i<=get_neighbors(u,Gstar).size() + nbrMinus(u,Gstar).size(); i++){
        Color.push_back(i);
    }

    uint64_t Cnew;
    for(uint64_t c : Color){
        if(std::find(C.begin(), C.end(), c) == C.end()){
            Cnew = c;
            break;
        }
    }
    if(Cnew != vertexLabel[u]){
        vertexLabel[u] = Cnew;
        return true;
    }else{
        return false;
    }

}
 */


/**
 * notifies the dominated neighbors of the node about the color change if the node has a new color
 * @param u node of the edge
 * @param b if the color of the node is new or not
 * @param q queue
 */
void LPA::NotifyColor(uint64_t u, bool b, std::queue<uint64_t> &q){
    if(b){
        for (const auto &v: get_neighbors(u,Gstar)){ //nbr+
            if(!contains(q,v)){
                q.push(v);
            }
        }
    }

}


/**
 * updates the direction of the edges based on the total order of nodes after the insertion to maintain the graph order
 * @param Gs oriented graph
 * @param u node of the inserted edge
 * @param v node of the inserted edge
 * @return list of the seed vertices to compute
 */
std::vector<uint64_t> LPA::OCG_Ins(Edge &Gs, uint64_t u, uint64_t v){

    std::vector<uint64_t> S;
    S.push_back(u);
    S.push_back(v);
    insertEdgeDirected_OCG(u,v,Gs);

    for (uint64_t ui: nbrMinus(u,Gs)){
        uint64_t u_degree = get_neighbors(u,Gs).size() + nbrMinus(u,Gs).size();
        uint64_t ui_degree = get_neighbors(ui,Gs).size() + nbrMinus(ui,Gs).size();

        if(u_degree > ui_degree or (u_degree == ui_degree and u < ui)){
            deleteEdgeDirected_OCG(ui,u,Gs);
            insertEdgeDirected_OCG(u,ui,Gs);
            S.push_back(ui);
        }
    }

    for (uint64_t vi: nbrMinus(v,Gs)){
        uint64_t v_degree = get_neighbors(v,Gs).size() + nbrMinus(v,Gs).size();
        uint64_t vi_degree = get_neighbors(vi,Gs).size() + nbrMinus(vi,Gs).size();
        if(v_degree > vi_degree or (v_degree == vi_degree and v < vi)){
            deleteEdgeDirected_OCG(vi, v,Gs);
            std::cout<<"Bu silindi "<< vi << " " <<v<<std::endl;
            insertEdgeDirected_OCG(v, vi,Gs);
            std::cout<<"Bu eklendi "<< v << " " <<vi<<std::endl;
            S.push_back(vi);
        }
    }
    return S;
}


/**
 * updates the direction of the edges based on the total order of nodes after the deletion to maintain the graph order
 * @param Gs oriented graph
 * @param u node of the inserted edge
 * @param v node of the inserted edge
 * @return list of the seed vertices to compute
 */
std::vector<uint64_t> LPA::OCG_Del(Edge &Gs, uint64_t u, uint64_t v){
    std::vector<uint64_t> S;
    S.push_back(u);
    S.push_back(v);
    deleteEdgeDirected_OCG(u,v,Gs); //Gs or Gstar?

    for (uint64_t ui: get_neighbors(u,Gs)){
        uint64_t u_degree = get_neighbors(u,Gs).size() + nbrMinus(u,Gs).size();
        uint64_t ui_degree = get_neighbors(ui,Gs).size() + nbrMinus(ui,Gs).size();

        if(u_degree < ui_degree or (u_degree == ui_degree and u > ui)){
            deleteEdgeDirected_OCG(u,ui,Gs);
            insertEdgeDirected_OCG(ui,u,Gs);
            S.push_back(ui);
        }
    }

    for (uint64_t vi: get_neighbors(v,Gs)){
        uint64_t v_degree = get_neighbors(v,Gs).size() + nbrMinus(v,Gs).size();
        uint64_t vi_degree = get_neighbors(vi,Gs).size() + nbrMinus(vi,Gs).size();
        if(v_degree < vi_degree or (v_degree == vi_degree and v > vi)){
            deleteEdgeDirected_OCG(v, vi,Gs);
            std::cout<<"Bu silindi "<< vi << " " <<v<<std::endl;
            insertEdgeDirected_OCG(vi, v,Gs);
            std::cout<<"Bu eklendi "<< v << " " <<vi<<std::endl;
            S.push_back(vi);
        }
    }
    return S;
}

Edge& LPA::GSreturn(){
    return this->Gstar;
}

std::vector<uint64_t> LPA::get_neighbors(uint64_t vertex, Edge &Gs){
    std::vector<uint64_t> neighbors;
    Gs.find_fn(vertex,[&neighbors](const std::vector<uint64_t>& nbrs){
        neighbors = nbrs;
    });
    return neighbors;
}

std::map<uint64_t, uint64_t>& LPA:: get_colors(){
    return this->vertexLabel;
}













