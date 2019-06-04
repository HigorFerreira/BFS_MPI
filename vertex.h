#ifndef VERTEX_H
#define VERTEX_H

//Vertex color constants
#define UNDEFINED 0
#define WHITE 1
#define GRAY 2
#define BLACK 3

class Vertex
{
    void defaultConstructor(int, Vertex*);
public:
    int key;
    int color;
    int edgeWeight;
    Vertex *neighbor;

    Vertex();
    Vertex(int);
    Vertex(Vertex*);
};

#endif // VERTEX_H
