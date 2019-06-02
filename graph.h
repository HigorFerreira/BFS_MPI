#include "vertex.h"
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

#ifndef GRAPH_H
#define GRAPH_H

//#define VERTEX_QTT 5

class Graph
{
public:
    int VERTEX_QTT;
    Vertex *list;
public:
    Graph();
    Graph(int);
    Graph(bool, int);
    int randd();
    bool isEmpty();
    Vertex *availableVertex();
    Vertex *availableVertex(int);
    bool insertEdge(int, int);
    bool insertEdge(int, int, int);
    void colorize(Vertex*);
    void connectAll(int, Vertex*, Vertex*);
    void whitise();

    Vertex* test();
};

#endif // GRAPH_H
