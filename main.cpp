#include "graph.h"


int main()
{
    Graph *g = new Graph(3);
    g->insertEdge(1, 2);
    g->colorize(g->test());
}
