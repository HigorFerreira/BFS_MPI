#include "vertex.h"

Vertex::Vertex()
{
    defaultConstructor(-1, 0);
}

Vertex::Vertex(int key)
{
    defaultConstructor(key, 0);
}

Vertex::Vertex(Vertex *neighbor)
{
    defaultConstructor(-1, neighbor);
}

void Vertex::defaultConstructor(int key, Vertex* neighbor){
    this->key = key < 0 ? -1 : key;

    this->neighbor = neighbor;
    this->color = WHITE;
    this->edgeWeight = 100;
}

