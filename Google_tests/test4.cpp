//
// Created by Yağız SEMERCİOĞLU on 14.05.24.
//

#include "gtest/gtest.h"
#include "adj_list.h"
#include "adj_list.cpp"
#include "LPA.cpp"
#include "seed_selection.cpp"
#include "NMI.h"
#include "NMI.cpp"
#include "ModularityDensity.h"
#include "ModularityDensity.cpp"
#include <map>
#include <iostream>

TEST(tempus, BasicAssertions) {
    AdjList list;
    LPA lpa;
    std::string filePathLpa = "../testinputLPA";
    list.addFromFile(filePathLpa);
    lpa.LabelPropagation(list.getEdges());
    lpa.get_colors();




}

TEST(tempus, entrophy){
    NMI nmi;
    std::map<uint64_t, uint64_t> example= {{0,1}, {1,1}, {2,1}, {3,1}, {4,1}, {5,1}, {6,6}, {7,6}, {8,6}, {9,6}, {10,6}, {11,6}, {12,6}, {13,6}, {14,6}, {15,6}};
    double ent = nmi.entrophy(example);
    std::cout<<"LOOOO"<<ent<<std::endl;
    EXPECT_NEAR(ent, 1.0, 1e-9);

}