//
// Created by Yağız SEMERCİOĞLU on 15.05.24.
//

#include "NMI.h"

#include <map>
#include <set>
#include <cmath>
#include <algorithm>
#include <vector>

typedef std::map< std::pair<uint64_t,uint64_t> , int> Matrix;


std::map<uint64_t, uint64_t> NMI::entrophy_count(std::map<uint64_t, uint64_t> &community) {
    std::map<uint64_t, uint64_t> labelCount;
    for(auto &vertex_label_pair : community){
        labelCount[vertex_label_pair.second]++;
    }
    return labelCount;
}

double NMI::entrophy(std::map<uint64_t,uint64_t>& community){
    double entrophy = 0.0;
    for(const auto &label_COUNT : entrophy_count(community)){
        double p = static_cast<double>(label_COUNT.second) / community.size();
        if( p > 0.0 ) {
            entrophy += -p * std::log2(p);
        }
    }
    return entrophy;
}

double NMI::h(uint64_t w, uint64_t n){

    return -static_cast<double>(w) * std::log2(w/n);
}

Matrix NMI::cover_matrix(std::map<uint64_t, uint64_t> &community){
    Matrix matrix;
    std::vector<uint64_t> labels;
    for (const auto &cluster : entrophy_count(community)) {
        for (int n = 1; n <= community.size(); ++n) {
            matrix[{cluster.first,n}] = 0;
        }
    }
    for(const auto & vL : community){
        matrix[{vL.second,vL.first}] = 1;
    }
    return matrix ;
}

std::tuple<int,int,int,int> NMI:: abcd_table(std::vector<int> &Xi, std::vector<int> &Yi){
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;

    for (int i = 0; i < Xi.size(); ++i) {
        if(Xi[i] == 0){
            if(Yi[i] == 0){
                a++;
            } else{
                b++;
            }
        } else if(Xi[i] == 1){
            if(Yi[i] == 0){
                c++;
            } else{
                d++;
            }

        }
    }


    return std::make_tuple(a,b,c,d);
}

double NMI::H_Xi_Yj(std::vector<int> &Xi, std::vector<int> &Yi){
    auto[a,b,c,d] = abcd_table(Xi,Yi);
    uint64_t n = Xi.size();
    double H = h(a,n) + h(b,n) + h(c,n) + h(d,n) - h(b+d,n) - h(a+c,n);


    return H;


}

double NMI::H_star(std::vector<int> &Xi, std::vector<int> &Yi){
    double hstar = 0.0;

    auto[a,b,c,d] = abcd_table(Xi,Yi);
    uint64_t n = Xi.size();

    if(h(a, n) + h(d, n) >= h(b, n) + h(c, n)){
        hstar = H_Xi_Yj(Xi,Yi);
    } else{
        hstar = h(c + d, n) + h(a + b, n);
    }

    return hstar;
}

double NMI::H_Xi_Y(std::vector<int> &Xi, std::map<uint64_t, uint64_t> &community2){
    std::vector<int> Yi;
    Matrix Y = cover_matrix(community2);

    std::vector<double> minYi;


    for (const auto &cluster : entrophy_count(community2)){
        Yi.clear();
        for (int n = 1; n <= community2.size(); ++n){
            Yi[Y.find({cluster.first,n})->first.second] = Y.find({cluster.first,n})->second;
        }

        minYi.emplace_back(H_star(Xi,Yi));
    }

    return *std::min_element(minYi.begin(), minYi.end());
}



double NMI::H_Yi_X(std::vector<int> &Yi, std::map<uint64_t, uint64_t> &community1){
    std::vector<int> Xi;
    Matrix X = cover_matrix(community1);

    std::vector<double> minXi;

    for (const auto &cluster : entrophy_count(community1)){
        Xi.clear();
        for (int n = 1; n <= community1.size(); ++n){
            Xi[X.find({cluster.first,n})->first.second] = X.find({cluster.first,n})->second;
        }

        minXi.emplace_back(H_star(Xi,Yi));
    }


    return *std::min_element(minXi.begin(), minXi.end());

}

double NMI::H_X_Y(std::map<uint64_t, uint64_t> &community1, std::map<uint64_t, uint64_t> &community2){
    std::vector<int> Xi;
    Matrix X = cover_matrix(community1);
    Matrix Y = cover_matrix(community2);
    double hxy = 0.0;

    for (const auto &cluster : entrophy_count(community2)){
        Xi.clear();
        for (int n = 1; n <= community1.size(); ++n){
            Xi[Y.find({cluster.first,n})->first.second] = Y.find({cluster.first,n})->second;
        }

        hxy += H_Xi_Y(Xi,community2);

    }

    return hxy;
}

double NMI::H_Y_X(std::map<uint64_t, uint64_t> &community1, std::map<uint64_t, uint64_t> &community2){
    std::vector<int> Yi;
    Matrix X = cover_matrix(community1);
    Matrix Y = cover_matrix(community2);
    double hyx = 0.0;

    for (const auto &cluster : entrophy_count(community1)){
        Yi.clear();
        for (int n = 1; n <= community2.size(); ++n){
            Yi[X.find({cluster.first,n})->first.second] = X.find({cluster.first,n})->second;
        }

        hyx += H_Yi_X(Yi,community1);

    }


    return hyx;
}

double NMI:: total_entropy(std::map<uint64_t, uint64_t> &community){
    int equal_1;
    int equal_0;
    Matrix m = cover_matrix(community);
    double totalENT = 0.0;
    for (const auto &cluster : entrophy_count(community)){
        equal_1 = 0.0;
        equal_0 = 0.0;
        for (int n = 1; n <= community.size(); ++n){
            if(m.find({cluster.first,n})->second == 1 ){
                equal_1++;
            } else{
                equal_0++;
            }
        }

        totalENT += h(equal_1,community.size()) + h(equal_0,community.size());

    }

    return totalENT;
}

double NMI::mutualInformation(std::map<uint64_t, uint64_t> &community1,std::map<uint64_t, uint64_t> &community2 ){
    double MI = 0.0;

    MI = 0.5 * (total_entropy(community1) - H_X_Y(community1,community2) + total_entropy(community2) -
            H_Y_X(community1,community2));

    return MI;
}

double NMI::NORMALIZED_mutualInformation(std::map<uint64_t, uint64_t> &community1,std::map<uint64_t, uint64_t> &community2){

    double H_X = total_entropy(community1);
    double H_Y = total_entropy(community2);

    return mutualInformation(community1,community2) / std::max(H_X,H_Y);
}



