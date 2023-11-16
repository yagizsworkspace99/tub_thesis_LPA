#include "adj_list.h"

int main() {
    //TODO: implement file reading, need to discuss what format we use for that
    AdjList list(20);

    list.addEdge(3, 6, 3);
    list.addEdge(3, 21, 5);
    list.addEdge(16, 6, 25);
    list.addEdge(16, 6, 7);
    list.addEdge(15, 6, 1);
    list.addEdge(15, 6, 1);
    list.deleteEdge(3, 21, 5);

    list.printGraph();

    return 0;
}
