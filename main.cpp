#include "graph.h"


int main()
{
    Graph *g = new Graph();

    g->connectAll();

    vector<Vertex*>* subGraph = g->colorize(g->test());
    int i = 2;
}
