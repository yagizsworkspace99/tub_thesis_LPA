#include "adj_list.h"
#include "seed_selection.h"
#include "LPA.h"
#include <map>
#include <oriented_graph.h>
#include "NMI.h"


int main() {
    AdjList list;
    AdjList list1;
    seed_selection ss;
    LPA lpa;
    oriented_graph orientedGraph;


    //std::string filePath = "../testAsonam2";
    std::string filePath = "../r3";
    //std::string filePath = "../testLPA2";
    //std::string filePath = "../testOG";
    //std::string filePath = "../new_test";
    //std::string filePath = "../testinputAsonam";
    //std::string filePath = "../testinputLPA";
    //std::string filePath = "../testinputLPA";
    //std::string filePath = "../TestInputLarge";


    list.addFromFile(filePath);

    //NestedMap a = list.getEdges();
    /*
    auto lt = a.lock_table();
    for (const auto &time_entry: lt) {
        Edge edgeData = time_entry.second;
        std::cout << "time: "<<time_entry.first << std::endl;
        auto lt2 = edgeData.lock_table();
        for (const auto &verticies: lt2) {
            uint64_t v = verticies.first;

            for(const auto &komşu: verticies.second){
                std::cout<<" vertex: " << v<< " komşu: " << komşu<< std::endl;
            }

        }
    }*/

    /*NMI nmi;
    std::map<uint64_t, uint64_t> example= {{1,2}, {2,1}, {3,9}, {4,1}, {5,5}, {6,5}, {7,5}, {8,5}, {9,5}, {10,11}, {11,11}, {12,11}, {13,11}, {14,11}, {15,11}, };
    std::map< std::pair<uint64_t,uint64_t> , int>  ent = nmi.cover_matrix(example);


    for(auto pair : ent){
        std::cout<< "cluster: " <<pair.first.first << " vertex: "<< pair.first.second << " value: "<<pair.second<<std::endl;
    }*/






    //list.printGraph();
    /*
    for(uint64_t i = 0; i<16; i++){
        std::cout<<"The" << i <<"has neighbor: "<<std::endl;
        for(auto item: ss.Neighbors(i,list.getEdges())){
            std::cout<<"*"<<(item)<<std::endl;

        }

    }/*









    /*std::vector<uint64_t> neighbors = ss.Neighbors(2,list.getEdges());
    std::cout<<"size of neighbors is: "<<(neighbors.size())<<std::endl;
    for(auto item: neighbors){
        std::cout<<"The node has neighbor: "<<(item)<<std::endl;

    }*/


    /*std::vector<uint64_t> seeds = ss.coloring_ss(list.getEdges());

    //std::vector<uint64_t> seeds = ss.link_ss( "RA", list.getEdges());
    std::cout<<"size of SEEDS is: "<<(seeds.size())<<std::endl;

    for(auto itemSeed: seeds){
        std::cout<<"The node has SEED: "<<(itemSeed)<<std::endl;

    }*/


    std::vector<uint64_t> seeds = {};

    lpa.LabelPropagation(seeds, list.getEdges()); //LPA   BURAYI AÇ
    for(auto a: lpa.get_colors()){

        std::cout << "vertex: " << a.first <<" labellar: "<<a.second<<std::endl;
    }

    /*for(auto a: lpa.get_colors()){
        std::cout<<"{"<<a.first <<","<<a.second<<"}"<<", ";
    }*/



    //Edge e = lpa.OCG(list.getEdges());  // BU BU BURAYI AAAÇÇÇÇÇ
    /*auto lt = e.lock_table();
    for(const auto &v:lt){
        std::cout<<"The vertex: "<<v.first<<std::endl;
        for(const auto &n : v.second){
            std::cout<<"The neighbors: "<<n<<std::endl;
        }


    }*/
    /*
    std::cout<<"ARA"<<std::endl;
    uint64_t dada =8;
    for(const auto &dg:lpa.nbrMinus(dada,lpa.GSreturn())){
        std::cout<<dada << "ın eksi komşuları: "<<dg<<std::endl;

    }*/


    /*
    uint64_t key = 3;
    std::vector<uint64_t> c;
    lpa.GSreturn().find_fn(key,[&c](const std::vector<uint64_t>& nbrs){
        c = nbrs;
    });



    for(auto a: c){
        std::cout <<" vertex bu"<< key <<"komş lar bunlar "<< a <<std::endl;
    }

    for(auto r: lpa.get_neighbors(3,lpa.GSreturn())){
        std::cout <<" vertex bu ikinci test"<< key <<"komş lar bunlar "<< r <<std::endl;
    }*/




    //lpa.DC_Orient_Ins(lpa.GSreturn(),3 ,12); //BU BU BU

    /*NestedMap a = list.getEdges();

    auto lt = a.lock_table();
    for (const auto &time_entry: lt) {
        if (time_entry.first > 0){
            Edge edgeData = time_entry.second;          BURAYI AAAÇÇÇÇ

            auto lt2 = edgeData.lock_table();
            for (const auto &verticies: lt2) {
                uint64_t v = verticies.first;
                for(const auto &nbr: verticies.second){
                    lpa.DC_Orient_Ins(lpa.GSreturn(),v ,nbr);
                    //std::cout<<"Bu girildi "<< " " <<v<<std::endl;
                    lt2.unlock();
                    lt.unlock();

                }



            }




        }

    }*/





    /*for(int i = 0; i<63731; i++){

        std::cout<< "vertex "<<i <<" Labelı "<<lpa.get_colors()[i]  <<std::endl;
    }*/



    /*
    lpa.DC_Orient_Del(lpa.GSreturn(),5 ,8);

    for(int i = 0; i<12; i++){

        std::cout<< "vertex yeni "<<i <<" Labelı "<<lpa.get_colors()[i]  <<std::endl;
    }*/






    /*for(const auto &nbrout : ss.neighbors_ss(0,0,list.getEdges())){
        std::cout<<"The neighbors are test: "<<nbrout<<std::endl;
    }*/







    /*auto lt1 = lpa.GSreturn().lock_table();
    for(const auto &v1:lt1){
        std::cout<<"The vertex func: "<<v1.first<<std::endl;
        for(const auto &n1 : v1.second){
            std::cout<<"The neighbors func: "<<n1<<std::endl;
        }


    }*/




    //list.getUpdates();


    //seed selection -> std::vector<uint64_t> seeds -> LPA -> vertexLabel -> orientedGraph -> Edge e















    /*for (size_t i = 1; i <= lM.size(); ++i) {
        std::cout << "Vertex: " << i << ", Label: " << lM[i] << std::endl;
    }*/








    return 0;
}
