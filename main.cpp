#include "adj_list.h"

int main() {
    //TODO: implement file reading, need to discuss what format we use for that
    AdjList list(20);

    list.addEdge(3, 6);
    list.addEdge(3, 21);
    list.addEdge(2, 25);
    list.addEdge(16, 6);
    list.addEdge(16, 6);
    list.addEdge(15, 6);
    list.deleteEdge(3, 21);

    list.printGraph();

    return 0;
}
