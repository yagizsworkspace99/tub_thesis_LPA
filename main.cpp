#include "adj_list.h"

int main() {

    AdjList list(1);

    std::string filePath = "../testinput";
    list.addFromFile(filePath);

    list.printGraph();

    return 0;
}
