#include "vertex.h"

Vertex::Vertex()
{
    defaultConstructor(-1, 100, 0);
}

Vertex::Vertex(int key)
{
    defaultConstructor(key, 100, 0);
}

Vertex::Vertex(int key, int weight){
    defaultConstructor(key, weight, 0);
}

Vertex::Vertex(Vertex *neighbor)
{
    defaultConstructor(-1, 100, neighbor);
}

void Vertex::defaultConstructor(int key, int weight, Vertex* neighbor){
    this->key = key < 0 ? -1 : key;

    this->neighbor = neighbor;
    this->color = WHITE;
    this->edgeWeight = weight;

    vertexWeight = UNDEFINED;
    predecessor = 0;
}

