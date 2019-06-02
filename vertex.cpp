#include "vertex.h"

Vertex::Vertex()
{
    this->key = -1;
    this->color = WHITE;
    this->neighbor = 0;
    this->edgeWeight = 100;
}

Vertex::Vertex(int key)
{
    this->key = key;
    this->color = WHITE;
    this->neighbor = neighbor;
    this->edgeWeight = 100;
}

Vertex::Vertex(Vertex *neighbor)
{
    this->key = -1;
    this->color = WHITE;
    this->neighbor = neighbor;
    this->edgeWeight = 100;
}

