#include "adj_list.h"
#include "seed_selection.h"
#include "LPA.h"
#include <map>
#include <oriented_graph.h>

int main() {
    AdjList list;
    AdjList list1;
    seed_selection ss;
    LPA lpa;
    oriented_graph orientedGraph;

    //std::string filePath = "../testinputAsonam";
    std::string filePath = "../testLPA2";
    list.addFromFile(filePath);

    //std::string filePathLpa = "../testinputLPA";
    //list1.addFromFile(filePathLpa);



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


    //std::vector<uint64_t> seeds = ss.coloring_ss(list.getEdges());
    //std::vector<uint64_t> seeds = ss.link_ss(list.getEdges());
    /*
    std::cout<<"size of SEEDS is: "<<(seeds.size())<<std::endl;
    for(auto itemSeed: seeds){
        std::cout<<"The node has SEED: "<<(itemSeed)<<std::endl;

    }*/
    /*
    Edge e = orientedGraph.OCG(list.getEdges());
    auto lt = e.lock_table();
    for(const auto &v:lt){
        std::cout<<"The vertex: "<<v.first<<std::endl;
        for(const auto &n : v.second){
            std::cout<<"The neighbors: "<<n<<std::endl;
        }


    }*/











    std::map<uint64_t,uint64_t> lM = lpa.LabelPropagation(list.getEdges()); //LPA


    for (size_t i = 1; i <= lM.size(); ++i) {
        std::cout << "Vertex: " << i << ", Label: " << lM[i] << std::endl;
    }






    return 0;
}
