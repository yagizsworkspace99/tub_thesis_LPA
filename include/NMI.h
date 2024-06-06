//
// Created by Yağız SEMERCİOĞLU on 15.05.24.
//

#ifndef TEMPUS_NMI_H
#define TEMPUS_NMI_H
#include <map>


class NMI {
    typedef std::map< std::pair<uint64_t,uint64_t> , int> Matrix;
public:
    double entrophy(std::map<uint64_t,uint64_t>& community);
    Matrix cover_matrix(std::map<uint64_t, uint64_t> &community);
    double mutualInformation(std::map<uint64_t, uint64_t> &community1,std::map<uint64_t, uint64_t> &community2);
    double NORMALIZED_mutualInformation(std::map<uint64_t, uint64_t> &community1,std::map<uint64_t, uint64_t> &community2);
    std::map<uint64_t, uint64_t> entrophy_count(std::map<uint64_t, uint64_t> &community);
private:

    static double h(uint64_t w, uint64_t n);
    std::tuple<int,int,int,int> abcd_table(std::vector<int> &Xi, std::vector<int> &Yi);
    double H_Xi_Yj(std::vector<int> &Xi, std::vector<int> &Yi);
    double H_star(std::vector<int> &Xi, std::vector<int> &Yi);
    double H_Xi_Y(std::vector<int> &Xi, std::map<uint64_t, uint64_t> &community2);
    double H_X_Y(std::map<uint64_t, uint64_t> &community1, std::map<uint64_t, uint64_t> &community2);
    double H_Yi_X(std::vector<int> &Yi, std::map<uint64_t, uint64_t> &community1);
    double H_Y_X(std::map<uint64_t, uint64_t> &community1, std::map<uint64_t, uint64_t> &community2);
    double total_entropy(std::map<uint64_t, uint64_t> &community);

};


#endif //TEMPUS_NMI_H
