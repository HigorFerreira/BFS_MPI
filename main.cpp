#include "graph.h"


int main()
{
    Graph *g = new Graph();

    g->connectAll();
    g->getSubGraphsList(0);

    vector<Vertex*>* subGraph = g->colorize(g->test());
    int i = 2;
}
