#include "adj_list.h"
#include "libcuckoo/cuckoohash_map.hh"

int main() {

    AdjList list;

    std::string filePath = "../testinput";
    list.addFromFile(filePath);

    list.printGraph();



    return 0;
}
