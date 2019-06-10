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

//Helper for organize structures to getSubGraphsList flow
//SGFH ---> Sub Graph Flow Helper
struct sgfh {
    Vertex* lastVertex;
    vector<Vertex*>* lastGraph;
};

class Graph
{
public:
    int VERTEX_QTT;
    Vertex *list;
    vector<vector<Vertex*>*>* subGraphs = new vector<vector<Vertex*>*>;
    void defaultConstructor(bool, int);

    Vertex *availableVertex(int, bool);
public:
    Graph();
    Graph(int);
    int randd();
    Vertex *availableVertex();
    Vertex *availableVertex(int);
    Vertex *availableVertexWithoutBlack(int);
    bool insertEdge(int, int);
    bool insertEdge(int, int, int);
    vector<Vertex*>* colorize(Vertex*);
    vector<Vertex*>* colorize(Vertex*, vector<Vertex*>*);
    void getSubGraphsList(int);
    void connectAll();

    void whitise();
    void whitise(int);

    Vertex* test();
};

#endif // GRAPH_H
