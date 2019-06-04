#include "graph.h"

Graph::Graph(){
    defaultConstructor(true, 0);
}

Graph::Graph(int qtt){
    defaultConstructor(false, qtt);
}

void Graph::defaultConstructor(bool isRandomic, int qtt){

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

    Vertex *v_from = &this->list[from], *v_to = &this->list[to];

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

vector<Vertex*>* Graph::colorize(Vertex *vertex){
    return colorize(vertex, 0);
}

//It Colorizes a subgraph with black and return a list of all vertex involved on this graph
vector<Vertex*>* Graph::colorize(Vertex *vertex, vector<Vertex*>* list){
    list = list != 0 ? list : new vector<Vertex*>;

    if(vertex->color == BLACK) return list;

    vertex->color = BLACK;
    list->push_back(vertex);

    while(vertex->neighbor != 0){
        vertex = vertex->neighbor;
        vertex->color = BLACK;
        colorize(&this->list[vertex->key], list);
    }

    return list;
}

void Graph::getSubGraphsList(int start=0, sgfh *flow=0){
    flow = flow == 0 ? new sgfh : flow;
    flow->lastVertex = availableVertex(start);

    for (int i = 0; i < flow->lastGraph->size(); i++) {
        if(flow->lastGraph->at(i)->key == flow->lastVertex->key){
            flow->lastVertex = flow->lastGraph->at(i);
        }
    }
    flow->lastGraph = colorize(flow->lastVertex);

    getSubGraphsList(flow->lastVertex->key);
}

void Graph::connectAll(int startFind, Vertex *vertex, Vertex *toConnect, vector<int>* stack){
    startFind = 0;
    vertex = 0;
    toConnect = 0;
    return;
}

Vertex* Graph::test(){
    return &this->list[0];
}
