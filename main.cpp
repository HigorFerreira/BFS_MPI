#include "graph.h"


int main()
{
    Graph *g = new Graph(3);
    g->insertEdge(0, 1);
    g->insertEdge(0, 2);
    g->insertEdge(2, 1);

    int* e1 = g->getWeight(0, 1);
    g->setWeight(0, 1, 30);
    e1 = g->getWeight(0, 1);

    int i = 2;
}
