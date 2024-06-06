//
// Created by Yağız SEMERCİOĞLU on 06.02.24.
//

#include "adj_list.h"
#include <cinttypes>
#include <algorithm>
#include <map>
#include "seed_selection.h"
#include <string>
#include <cmath>




/**
* finds the neighbors of a node
* @param node of the edge
* @param map
* @return list of neighbors
*/
std::vector<uint64_t> seed_selection::NeighborsG(uint64_t u,NestedMap &G) {
    std::vector<uint64_t> neighbors;
    auto lt = G.lock_table();
    for (const auto &innerTbl: lt) {
        Edge edgeData = innerTbl.second;
        auto lt2 = edgeData.lock_table();
        for (const auto &vector: lt2) {
            if(u == vector.first){
                neighbors.insert(neighbors.end(), vector.second.begin(), vector.second.end());
            }
            lt.unlock();
            lt2.unlock();
        }
    }
    return neighbors;
}


/**
 * Calculates the Neighbors index score of a node
 * @param v node of the edge
 * @param u node of the edge
 * @param G map
 * @return calculated score of the node
 */
double seed_selection::Score(std::vector<uint64_t> v,std::vector<uint64_t> u,NestedMap &G){
    std::vector<uint64_t> commonN;
    double score =0;
    for(const auto &neighbor:v){
        if(std::find(u.begin(),u.end(),neighbor) != u.end()){
            commonN.push_back(neighbor);
        }
    }
    score = commonN.size();
    return score;
}


/**
 * Calculates the Hub promoted index score of a node
 * @param v node of the edge
 * @param u node of the edge
 * @param G map
 * @return calculated score of the node
 */
double seed_selection::hubPromotedIndex(std::vector<uint64_t> v,std::vector<uint64_t> u,NestedMap &G){
    double score =0;
    score = Score(v,u,G) / std::min(v.size(), u.size());
    return score;
}


/**
 * Calculates the Leicht-Holme-Newman index score of a node
 * @param v node of the edge
 * @param u node of the edge
 * @param G map
 * @return calculated score of the node
 */
double seed_selection::lhnIndex(std::vector<uint64_t> v,std::vector<uint64_t> u,NestedMap &G){
    double score =0;
    score = Score(v,u,G) / (v.size() * u.size());
    return score;
}


/**
 * Calculates the Preferential Attachment index score of a node
 * @param v node of the edge
 * @param u node of the edge
 * @param G map
 * @return calculated score of the node
 */
double seed_selection::preferentialAttachment(std::vector<uint64_t> v,std::vector<uint64_t> u,NestedMap &G){
    double score =0;
    score = v.size() * u.size();
    return score;
}


/**
 * Helps to calculate the score for Resource Allocation index
 * finds the common neighbors of two nodes
 * rest of the score calculation is in the seed selection function
 * the reason is the lock mechanism of the map
 * @param v node of the edge
 * @param u node of the edge
 * @param G map
 * @return list of common neighbors of v and u
 */
std::vector<uint64_t> seed_selection::commonNbrs(std::vector<uint64_t> v,std::vector<uint64_t> u,NestedMap &G){ //Resource allocaiton index
    std::vector<uint64_t> commonN;
    for(const auto &neighbor:v){
        if(std::find(u.begin(),u.end(),neighbor) != u.end()){
            commonN.push_back(neighbor);
        }
    }
    return commonN;
}


/**
 * first algorithm, Link prediction-based seed selection
 * finds the seed nodes in the graph
 * @param G map
 * @return seed nodes
 */
std::vector<uint64_t> seed_selection::link_ss(const std::string& Type,NestedMap &G) {
    std::map<uint64_t, double> vertexScores;
    std::vector<uint64_t> neighborN;
    std::vector<uint64_t> seeds;
    bool flag = false;
    auto lt = G.lock_table();
    for (const auto &time_entry: lt) {
        Edge edgeData = time_entry.second;
        auto lt2 = edgeData.lock_table();
        for (const auto &verticies: lt2) {
            uint64_t v = verticies.first;
            //std::cout<<"bu : "<<v <<std::endl;
            //std::cout<<"Score: "<<this->Score(v, G) <<std::endl;
            for(const auto &n : verticies.second){
                for (const auto &verticiesAgain: lt2){
                    if(verticiesAgain.first == n){
                        neighborN = verticiesAgain.second;
                        break;
                    }
                }
                //initialize score of each vertex
                if(Type == "CN"){
                    vertexScores[v] = vertexScores[v] + Score(verticies.second,neighborN, G);
                    vertexScores[v] = std::round(vertexScores[v] * 1000.0) / 1000.0;//CN
                }
                else if(Type == "HP"){
                    vertexScores[v] = vertexScores[v] + hubPromotedIndex(verticies.second,neighborN, G);
                    vertexScores[v] = std::round(vertexScores[v] * 1000.0) / 1000.0;//HP
                }
                else if(Type == "LHN"){
                    vertexScores[v] = vertexScores[v] + lhnIndex(verticies.second,neighborN, G); //LHN
                    vertexScores[v] = std::round(vertexScores[v] * 1000.0) / 1000.0;
                }
                else if(Type == "RA"){
                    double RA = 0;
                    std::vector<uint64_t> commonNbr = commonNbrs(verticies.second,neighborN, G);
                    for(auto cn : commonNbr){
                        for (const auto &verticies: lt2) {
                            if (cn == verticies.first){
                                RA = RA + (1.0 / verticies.second.size());
                            }
                        }
                    }
                    vertexScores[v] = vertexScores[v] + RA; //RA
                    vertexScores[v] = std::round(vertexScores[v] * 1000.0) / 1000.0;
                }
                else if(Type == "PA"){


                    vertexScores[v] = vertexScores[v] + preferentialAttachment(verticies.second,neighborN, G); //PA
                    vertexScores[v] = std::round(vertexScores[v] * 1000.0) / 1000.0;
                }

            }

        }
    }
    for (const auto& pair : vertexScores) {
        std::cout << "Node_link: " << pair.first << ", Score_link: " << pair.second << std::endl; //just for test
    }
    for (const auto &time_entry: lt) {
        Edge edgeData = time_entry.second;
        auto lt2 = edgeData.lock_table();
        for (const auto &verticies: lt2) {
            auto v = verticies.first;
            if (vertexScores[v] > 0) {
                for(const auto &neighbor:verticies.second){
                    if(vertexScores[v]>=vertexScores[neighbor]){
                        flag =true;
                    } else{
                        flag = false;
                        break;
                    }
                }
            }
            if (flag){
                seeds.push_back(v);
            }
            // for each vertex check if it has higher score then all of its neighbors
        }
    }
    return seeds;
}


/**
 * second algorithm, Biased coloring-based seed selection
 * finds the seed nodes in the graph with coloring
 * @param G map
 * @return seed nodes
 */
std::vector<uint64_t> seed_selection::coloring_ss(NestedMap &G){

    std::map<uint64_t, double> vertexScores_c;
    std::map<std::pair<uint64_t, uint64_t>, uint64_t>  confirm_c;
    std::map<uint64_t, bool> converge_c;
    std::map<uint64_t, uint64_t> color_c;
    std::map<uint64_t, uint64_t> color_fix;
    std::map<uint64_t ,std::set<uint64_t>> availableColors_c;
    std::vector<uint64_t> seeds; //seeds that we will return
    std::vector<uint64_t> neighborN;
    bool flag;
    auto lt = G.lock_table();
    for (const auto &time_entry: lt) {
        Edge edgeData = time_entry.second;
        auto lt2 = edgeData.lock_table();
        for (const auto &verticies: lt2) { //for all 𝑣 ∈ 𝑉
            uint64_t v = verticies.first;
            /*for(const auto &n : verticies.second){

                vertexScores_c[v] = vertexScores_c[v] + Score(v,n, G); //initialize score of each vertex
            } //score(𝑣) = 𝑢∈Γ(𝑣) sim(𝑢, 𝑣)*/
            for(const auto &n : verticies.second){
                for (const auto &verticiesAgain: lt2){
                    if(verticiesAgain.first == n){
                        neighborN = verticiesAgain.second;
                        break;
                    }
                }



                //vertexScores_c[v] = vertexScores_c[v] + Score(verticies.second,neighborN, G);
                vertexScores_c[v] = vertexScores_c[v] + preferentialAttachment(verticies.second,neighborN, G); //PA
                vertexScores_c[v] = std::round(vertexScores_c[v] * 1000.0) / 1000.0;//HP

                //vertexScores_c[v] = vertexScores_c[v] + Score(verticies.second,neighborN, G); //initialize score of each vertex
            }
        }
    }
    for (const auto& pair : vertexScores_c) {
        std::cout << "Node: " << pair.first << ", Score: " << pair.second << std::endl; //just for test
    }
    for (const auto &time_entry: lt) {
        Edge edgeData1 = time_entry.second;
        auto lt2 = edgeData1.lock_table();
        for (const auto &verticies: lt2) {
            std::vector<uint64_t> SC;  // Let𝑆𝐶=∅;

            auto v = verticies.first;

            std::vector<uint64_t> egonet = verticies.second;
            egonet.push_back(v);

            for(const auto &neighbor : verticies.second){                                //∀𝑢 ∈ Γ(𝑣)
                confirm_c[{neighbor,v}] = 0; //confirm(𝑢, 𝑣) = 0;
            }

            /*
            //for test
            std::cout << "Selected Vertex: " << v << std::endl;
            for (const auto& pair : confirm_c) {
                auto key = pair.first;
                uint64_t value = pair.second;
                std::cout << "Confirm_Key: (" << key.first << ", " << key.second << ") ";
                std::cout << "Value: " << value << std::endl;
            }*/


            converge_c[v] = false;  //converge(𝑣) = 𝑓 𝑎𝑙𝑠𝑒;
            color_c[v] = 0;            //color(𝑣) = 0;

            /*
            //for test
            for (std::map<uint64_t, bool>::const_iterator it = converge_c.begin(); it != converge_c.end(); ++it) {
                std::cout << "Key: " << it->first << " - Value: " << (it->second ? "true" : "false") << std::endl;
            }*/



            for(int i = 1; i <= verticies.second.size()+1; ++i) {
                availableColors_c[v].insert(i);         //available colors(𝑣)={𝑐1,...,𝑐𝑘𝑣+1} where 𝑘𝑣 =∣Γ(𝑣)∣;
            }


            for (const auto &u: egonet){
                SC.push_back(vertexScores_c[u]);
            }
            availableColors_c[v].erase(1);
            int max = *std::max_element(SC.begin(), SC.end());
            for(const auto &u:egonet){

                if(color_fix[u]){
                    std::cout << "Vsi bu: "<<v<<" U: " <<u<< " Color fix: "<< color_fix[u] << std::endl;
                    color_c[u] = color_fix[u];
                    availableColors_c[v].erase(color_fix[u]);
                    continue;
                }
                else if(vertexScores_c[u] == max){  //if score(𝑢) = 𝑚𝑎𝑥(𝑆𝐶) then 𝑐𝑜𝑙𝑜𝑟(𝑢) = 𝑐1; end if
                    color_c[u] = 1;
                }

                else if(color_c[u] == 0){
                    color_c[u] = *availableColors_c[v].begin();  //color(𝑣) = pick color(available colors(𝑣));
                    availableColors_c[u].erase(color_c[u]);

                }

            }







            while(!converge_c[v]){ //while converge(𝑣) = 𝑓𝑎𝑙𝑠𝑒 do
                for(const auto &neighbor:verticies.second){ // for all 𝑢 ∈ Γ(𝑣) do

                    if ((color_c[v]==color_c[neighbor]) and (vertexScores_c[v]  <= vertexScores_c[neighbor])){ //if color(𝑣)=color(𝑢) and score(𝑣)≤score(𝑢) then
                        color_c[v] = *availableColors_c[v].begin();  // color(𝑣) = pick color(available colors(𝑣));
                        availableColors_c[v].erase(color_c[v]);
                    }else if(color_c[neighbor]>0){ //else if color(𝑢) > 0
                        confirm_c[{neighbor,v}] = 1; //confirm(𝑢, 𝑣) = 1
                    }
                }



                if (color_c[v] > 0) {  // Check color(v) > 0 first, outside the loop
                    bool all_confirm = true;
                    for (const auto& neighbor : verticies.second) {  // Check ∀u∈Γ(v)
                        if (confirm_c[{neighbor, v}] != 1) {  // confirm(u, v) must be 1 for all u
                            all_confirm = false;
                            break;  // No need to check further if one fails
                        }
                    }
                    converge_c[v] = all_confirm;  // Set true only if all neighbors confirmed
                }
            }
            if(color_c[v] == 1 and verticies.second.size() > 1 ){ //if𝑐𝑜𝑙𝑜𝑟(𝑣)=𝑐1and𝑘𝑣 >1
                seeds.push_back(v); // 𝑆=𝑆∪{𝑣};
            }
            SC.clear();
            color_fix[v] = color_c[v];
        }
    }
    return seeds; //return 𝑆
}




std::vector<uint64_t> seed_selection::neighbors_ss(uint64_t vertex, uint64_t time ,NestedMap &G){

    std::vector<uint64_t> neighbors1;
    if (G.contains(time)) {
        G.update_fn(time,[vertex, &neighbors1](Edge &e) {
            e.find_fn(vertex,[&neighbors1](const std::vector<uint64_t>& nbrs){
                neighbors1 = nbrs;
            });
        });
    }

    return neighbors1;
}

uint64_t seed_selection::get_random_element(const std::set<uint64_t> set) {
    if (set.empty()) {
        throw std::runtime_error("Cannot pick a random element from an empty set");
    }
    auto it = set.begin();
    std::advance(it, rand() % set.size());
    return *it;
}