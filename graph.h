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
//struct sgfh {
//    Vertex* lastVertex;
//    vector<Vertex*>* lastGraph;
//};

struct edge{
    Vertex *u;
    Vertex *v;
    int edgeWeight;
};

struct shortWayResponse{
    vector<int> *path;
    int cost;
    bool sucess = false;
    string message;
};

typedef edge w;

class Graph
{
private:
    int VERTEX_QTT;
    Vertex *list;
    vector<vector<Vertex*>*>* subGraphs = new vector<vector<Vertex*>*>;
    void defaultConstructor(bool, int);

    vector<Vertex*> *aux;

    Vertex *availableVertex(int, bool);
public:
    Graph();
    Graph(int);
    int randd();
    Vertex *availableVertex();
    Vertex *availableVertex(int);
    Vertex *availableVertexWithoutBlack(int);
    bool insertEdge(int, int, int);
     bool insertEdge(int, int);
    vector<Vertex*>* colorize(Vertex*);
    vector<Vertex*>* colorize(Vertex*, vector<Vertex*>*);
    void getSubGraphsList(int);
    void connectAll();

    int* getWeight(int from, int to);
    int* setWeight(int from, int to, int weight, bool first=true);

    void whitise();
    void whitise(int);

    void *serialize();

private:
    void initBF(int);
    void relax(int, int, w*);
    bool bellmanFord(int);

public:
    shortWayResponse *shortWay(int, int);

    Vertex* test();
};

#endif // GRAPH_H
