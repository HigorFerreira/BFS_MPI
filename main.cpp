#include "graph.h"


int main()
{
    Graph *g = new Graph(6);
    g->insertEdge(1, 2);
    g->insertEdge(2, 3);
    g->insertEdge(3, 1);
    g->insertEdge(2, 0);
    g->insertEdge(4, 5);
    vector<Vertex*>* subGraph = g->colorize(g->test());
    int i = 2;
}
