#ifndef VERTEX_H
#define VERTEX_H

//Vertex color constants
#define WHITE 1
#define GRAY 2
#define BLACK 3

typedef long int_v;

const int_v INFTY = 0xf00000000;
const int_v EMPTY = 0x100000000;
const int_v UNDEFINED = 0x200000000;
const int_v INT_V_MASK = 0x00000000ffffffff;
const int_v INVALID_INT_V = 0x300000000;

class Vertex
{
    void defaultConstructor(int, int, Vertex*);
public:
    int key;
    int color;
    int edgeWeight;
    Vertex *neighbor;

    int_v vertexWeight;
    Vertex *predecessor;

    Vertex();
    Vertex(int);
    Vertex(int, int);
    Vertex(Vertex*);
};

#endif // VERTEX_H
