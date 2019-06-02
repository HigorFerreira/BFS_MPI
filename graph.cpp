#include "graph.h"

Graph::Graph(){
    Graph(true, 0);
}

Graph::Graph(int qtt){
    Graph(false, qtt);
}

Graph::Graph(bool isRandomic, int qtt)
{
    srand(time(NULL));

    this->VERTEX_QTT = qtt < 1 ? 65000 : qtt;

    this->list = new Vertex[this->VERTEX_QTT];

    //Setting vertex keys
    for (int i = 0; i<VERTEX_QTT; i++) {
        this->list[i].key = i;
    }

    if(isRandomic){
        for(int i = 0; i < VERTEX_QTT; i++){
            int from = randd(), to = randd();
            insertEdge(from, to);
        }
    }
}

int Graph::randd(){
    return rand() % VERTEX_QTT;
}

bool Graph::isEmpty(){
    return 0;
}

Vertex *Graph::availableVertex(){
    return availableVertex(0);
}

Vertex *Graph::availableVertex(int start){
    Vertex *v_start = list + start;
    if(v_start->neighbor == 0 && start < VERTEX_QTT){
        v_start = availableVertex(++start);
    }

    return start < VERTEX_QTT ? v_start : 0;
}

bool Graph::insertEdge(int from, int to){
    if(from == to) return false;

    Vertex *v_from = list+from, *v_to = list+to;

    while(v_from->neighbor != 0){
        v_from = v_from->neighbor;
        if(v_from->key == to){
            return false;
        }
    }
    while(v_to->neighbor != 0){
        v_to = v_to->neighbor;
        if(v_to->key == from){
            return false;
        }
    }

    v_from->neighbor = new Vertex(to);
    v_to->neighbor = new Vertex(from);

    return true;
}

bool Graph::insertEdge(int from, int to, int weight){
    //Desabling QT warnnings
    int some = from*to*weight;
    return some&false;
}

void Graph::colorize(Vertex *vertex){
    if(vertex->color == BLACK) return;
    vertex->color = BLACK;
    if(vertex->neighbor != 0){
        vertex->neighbor->color = BLACK;
        colorize(&this->list[vertex->neighbor->key]);
    }
}

void Graph::connectAll(int startFind, Vertex *vertex, Vertex *toConnect){
    startFind = 0;
    vertex = 0;
    toConnect = 0;
    return;
}

Vertex* Graph::test(){
    return &this->list[1];
}
