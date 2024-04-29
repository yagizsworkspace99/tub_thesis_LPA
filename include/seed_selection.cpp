//
// Created by Yağız SEMERCİOĞLU on 06.02.24.
//
#include "adj_list.h"
#include "parlay/parallel.h"

#include <fstream>
#include <cinttypes>
#include <algorithm>

#include <map>
#include "seed_selection.h"







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

double seed_selection::hubPromotedIndex(std::vector<uint64_t> v,std::vector<uint64_t> u,NestedMap &G){
    double score =0;
    score = Score(v,u,G) / std::min(v.size(), u.size());
    return score;
}

double seed_selection::lhnIndex(std::vector<uint64_t> v,std::vector<uint64_t> u,NestedMap &G){
    double score =0;
    score = Score(v,u,G) / (v.size() * u.size());
    return score;

}
double seed_selection::preferentialAttachment(std::vector<uint64_t> v,std::vector<uint64_t> u,NestedMap &G){
    double score =0;
    score = v.size() * u.size();
    return score;

}





std::vector<uint64_t> seed_selection::commonNbrs(std::vector<uint64_t> v,std::vector<uint64_t> u,NestedMap &G){ //Resource allocaiton index


    std::vector<uint64_t> commonN;

    for(const auto &neighbor:v){
        if(std::find(u.begin(),u.end(),neighbor) != u.end()){
            commonN.push_back(neighbor);
        }
    }

    return commonN;
}

/*
bool seed_selection::Converge(std::vector<uint64_t> u,NestedMap &G){
    bool converge = true;
    for(const auto &neighbor:u){
        if(color_c[u] == color_c[neighbor] ){
            converge = false;
            break;

        }
    }

    return converge;
}*/

std::vector<uint64_t> seed_selection::link_ss(NestedMap &G) {
    std::map<uint64_t, double> vertexScores;
    std::vector<uint64_t> neighborN;
    std::vector<uint64_t> seeds;

    bool flag;
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
                vertexScores[v] = vertexScores[v] + Score(verticies.second,neighborN, G); //CN
                //vertexScores[v] = vertexScores[v] + hubPromotedIndex(verticies.second,neighborN, G); //HP
                //vertexScores[v] = vertexScores[v] + lhnIndex(verticies.second,neighborN, G); //LHN

                /*
                double RA = 0;
                std::vector<uint64_t> commonNbr = commonNbrs(verticies.second,neighborN, G);
                for(auto cn : commonNbr){
                    for (const auto &verticies: lt2) {
                        if (cn == verticies.first){
                            RA = RA + (1.0 / verticies.second.size());
                        }
                    }
                }
                vertexScores[v] = vertexScores[v] + RA; //RA*/

                //vertexScores[v] = vertexScores[v] + preferentialAttachment(verticies.second,neighborN, G); //PA


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
                flag = true;
                for(const auto &neighbor:verticies.second){
                    if(vertexScores[v]<vertexScores[neighbor]){
                        flag =false;
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

std::vector<uint64_t> seed_selection::coloring_ss(NestedMap &G){ //**********************

    std::map<uint64_t, double> vertexScores_c;
    std::map<std::pair<uint64_t, uint64_t>, uint64_t>  confirm_c;
    std::map<uint64_t, bool> converge_c;
    std::map<uint64_t, uint64_t> color_c;
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

                vertexScores_c[v] = vertexScores_c[v] + Score(verticies.second,neighborN, G); //initialize score of each vertex
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
            std::vector<uint64_t> egonet; // neighbors + the vertex v
            std::vector<uint64_t> neighbors = verticies.second;
            egonet = neighbors;
            egonet.push_back(v);


            for(const auto &neighbor:neighbors){                                //∀𝑢 ∈ Γ(𝑣)
                confirm_c[{neighbor,v}] = 0; //confirm(𝑢, 𝑣) = 0;

            }

            std::cout << "Selected Vertex: " << v << std::endl;
            for (const auto& pair : confirm_c) {
                auto key = pair.first;
                uint64_t value = pair.second;

                std::cout << "Confirm_Key: (" << key.first << ", " << key.second << ") ";
                std::cout << "Value: " << value << std::endl;
            }



            converge_c[v] = false;  //converge(𝑣) = 𝑓 𝑎𝑙𝑠𝑒;
            color_c[v] = 0;            //color(𝑣) = 0;

            for (std::map<uint64_t, bool>::const_iterator it = converge_c.begin(); it != converge_c.end(); ++it) {
                std::cout << "Key: " << it->first << " - Value: " << (it->second ? "true" : "false") << std::endl;
            }

            for(int i = 1; i <= neighbors.size()+1; ++i) {
                availableColors_c[v].insert(i);         //available colors(𝑣)={𝑐1,...,𝑐𝑘𝑣+1} where 𝑘𝑣 =∣Γ(𝑣)∣;
            }





            for (const auto &u: egonet){
                SC.push_back(vertexScores_c[u]);
            }




            int max = *std::max_element(SC.begin(), SC.end());


            for(const auto &u:egonet){  // for all 𝑢 ∈ 𝑒𝑔𝑜𝑛𝑒𝑡(𝑣) do
                if(vertexScores_c[u] == max){  //if score(𝑢) = 𝑚𝑎𝑥(𝑆𝐶) then 𝑐𝑜𝑙𝑜𝑟(𝑢) = 𝑐1; end if
                    color_c[u] = 1;

                }
            }
            availableColors_c[v].erase(1);



            if(color_c[v] == 0){
                color_c[v] = *availableColors_c[v].begin();  //color(𝑣) = pick color(available colors(𝑣));
                availableColors_c[v].erase(availableColors_c[v].begin());
            }





            while(converge_c[v] == false){ //while converge(𝑣) = 𝑓𝑎𝑙𝑠𝑒 do
                for(const auto &neighbor:neighbors){ // for all 𝑢 ∈ Γ(𝑣) do
                    if(neighbor == 12){
                        std::cout<< color_c[neighbor] <<std::endl;
                    }

                    if ((color_c[v]==color_c[neighbor]) and (vertexScores_c[v]  <= vertexScores_c[neighbor])){ //if color(𝑣)=color(𝑢) and score(𝑣)≤score(𝑢) then
                        color_c[v] = *availableColors_c[v].begin();  // color(𝑣) = pick color(available colors(𝑣));
                        availableColors_c[v].erase(availableColors_c[v].begin());

                    }else if(color_c[neighbor]>0){ //else if color(𝑢) > 0

                        confirm_c[{neighbor,v}] = 1; //confirm(𝑢, 𝑣) = 1
                    }

                }


                if (color_c[v] > 0) {  // Check color(v) > 0 first, outside the loop
                    bool all_confirm = true;
                    for (const auto& neighbor : neighbors) {  // Check ∀u∈Γ(v)
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




        }
    }

    return seeds; //return 𝑆


}