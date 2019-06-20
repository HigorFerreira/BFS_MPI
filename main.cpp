#include "graph.h"


int main()
{
    Graph *g = new Graph(3);

    g->insertEdge(0, 1, -1);
    g->insertEdge(1, 2, 2);
    g->insertEdge(2, 0, 3);

    vector<Vertex*> graph;
    g->aux = &graph;
    for (int i = 0; i < g->VERTEX_QTT; i++) {
        graph.push_back(&g->list[i]);
    }

    shortWayResponse *way1 = g->shortWay(0, 2);

    int i = 0;
}
