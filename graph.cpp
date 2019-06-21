#include "graph.h"
#include <cmath>

Graph::Graph(){
    defaultConstructor(true, 0);
}

Graph::Graph(int qtt){
    defaultConstructor(false, qtt);
}

void Graph::defaultConstructor(bool isRandomic, int qtt){

//    srand(time(NULL));

    this->VERTEX_QTT = qtt < 1 ? 65000 : qtt;

    this->list = new Vertex[this->VERTEX_QTT];

    //Setting vertex keys
    for (int i = 0; i<VERTEX_QTT; i++) {
        this->list[i].key = i;
    }

    if(isRandomic){
        for(int i = 0; i < 5000; i++){
            int from = randd(), to = randd();
            insertEdge(from, to);
        }
    }
}

//int Graph::randd(){
//    return rand() % VERTEX_QTT;
//}

int Graph::randd()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand ((time_t )ts.tv_nsec);
    int rd = rand()%VERTEX_QTT;
    return rd;
}

Vertex *Graph::availableVertex(){
    return availableVertex(0);
}

Vertex *Graph::availableVertex(int start){
    availableVertex(start, true);
}

Vertex *Graph::availableVertexWithoutBlack(int start){
    availableVertex(start, false);
}

Vertex *Graph::availableVertex(int start, bool withBlack){
    Vertex *v_start = list + start;
    if(withBlack){
        if(v_start->neighbor == 0 && start < VERTEX_QTT){
            v_start = availableVertex(++start);
        }
    }
    else {
        if((v_start->neighbor == 0 || v_start->color == BLACK) && start < VERTEX_QTT){
            v_start = availableVertex(++start, false);
        }
    }

    return start < VERTEX_QTT ? v_start : 0;
}

bool Graph::insertEdge(int from, int to, int weight){
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

    v_from->neighbor = new Vertex(to, weight);
    v_to->neighbor = new Vertex(from, weight);

    return true;
}

bool Graph::insertEdge(int from, int to){
    insertEdge(from, to, 100);
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

void Graph::getSubGraphsList(int start=0){
//    flow = flow == 0 ? new sgfh : flow;
//    flow->lastVertex = availableVertex(start);

//    for (int i = 0; i < flow->lastGraph->size(); i++) {
//        if(flow->lastGraph->at(i)->key == flow->lastVertex->key){
//            flow->lastVertex = flow->lastGraph->at(i);
//        }
//    }
//    flow->lastGraph = colorize(flow->lastVertex);

//    getSubGraphsList(flow->lastVertex->key);
    Vertex* next = availableVertexWithoutBlack(start);
//    if(next->color != BLACK && next != 0){
    if(next != 0){
        vector<Vertex*>* sub = colorize(next);
        this->subGraphs->push_back(sub);
        getSubGraphsList(next->key+1);
    }
}

void Graph::connectAll(){

    this->getSubGraphsList(0);

    this->whitise();

    for (int i=1; i<this->subGraphs->size(); i++) {
        vector<Vertex*>* GraphA = this->subGraphs->at(i-1);
        vector<Vertex*>* GraphB = this->subGraphs->at(i);

        int from = rand() % GraphA->size();
        int to = rand() % GraphB->size();

        from = GraphA->at(from)->key;
        to = GraphB->at(to)->key;

        this->insertEdge(from, to);
    }

    delete this->subGraphs;
    subGraphs = new vector<vector<Vertex*>*>;
}

Vertex* Graph::test(){
    return &this->list[0];
}

int* Graph::getWeight(int from, int to){
    if(from == to) return 0;

    Vertex* aux = &this->list[from];
    while(aux != 0 && aux->key != to){
        aux = aux->neighbor;
    }

    return (int*)aux == 0 ? 0 : new int(aux->edgeWeight);
}

int* Graph::setWeight(int from, int to, int weight, bool first){
    if(from == to) return 0;

    Vertex* aux = &this->list[from];
    while(aux != 0 && aux->key != to){
        aux = aux->neighbor;
    }

    if(aux == 0) return 0;

    aux->edgeWeight = weight;

    if(first)
    if(setWeight(to, from, weight, false) == 0) return 0;

    return new int;
}

void Graph::whitise(){
    for (int i = 0; i < VERTEX_QTT; i++) {
        whitise(i);
    }
}

void Graph::whitise(int vertex){
    this->list[vertex].color = WHITE;

    Vertex *next = this->list[vertex].neighbor;
    while(next != 0){
        next->color = WHITE;
        next = next->neighbor;
    }
}


void *Graph::serialize(){
    if(this->VERTEX_QTT > 0xffff) return 0;

    char *buffer = new char[0x1BFFF];
    char *bufferPointer = buffer;

    //Reserving 4Bytes to the buffer total size
    bufferPointer += 4;

    *(unsigned short*)bufferPointer = this->VERTEX_QTT;
    bufferPointer += 2;

    for (unsigned short i = 0; i < this->VERTEX_QTT; i++) {
        short acumulator = 0;
        Vertex *aux = this->list[i].neighbor;
        Vertex *aux1 = aux;
        char *vertexAffected = bufferPointer;
        char *listSize = bufferPointer+2;
        char *list = bufferPointer+4;

        while(aux){
            *(int*)list = aux->key;
            list += 4;
            acumulator++;

            aux = aux->neighbor;
        }

        if(aux1){
            *(unsigned short*)vertexAffected = i;
            *(unsigned short*)listSize = acumulator;
            bufferPointer = list;
        }
    }

    //Assing buffer total size
    *(int *)buffer = bufferPointer-buffer;

    return (void*)buffer;
}

void Graph::initBF(int s){
    for (int i = 0; i < VERTEX_QTT; i++) {
        this->list[i].vertexWeight = INFTY;
    }
    this->list[s].vertexWeight = 0;
}

void Graph::relax(int u, int v, w *w){
    if(this->list[v].vertexWeight > this->list[u].vertexWeight + w->edgeWeight){
        this->list[v].vertexWeight = (int)this->list[u].vertexWeight + w->edgeWeight;
        this->list[v].predecessor = &this->list[u];
    }
}

bool Graph::bellmanFord(int s){
    initBF(s);

    for (int i = 1; i < VERTEX_QTT; i++) {
        for (int j = 0; j < VERTEX_QTT; j++) {
            Vertex *aux_vertex = this->list[j].neighbor;
            while (aux_vertex) {

                edge *w = new edge;
                w->edgeWeight = aux_vertex->edgeWeight;
                w->u = &this->list[j];
                w->v = &this->list[aux_vertex->key];

                relax(w->u->key, w->v->key, w);

                delete w;

                aux_vertex = aux_vertex->neighbor;
            }
        }
    }

    for (int j = 0; j < VERTEX_QTT; j++) {
        Vertex *aux_vertex = this->list[j].neighbor;
        while (aux_vertex) {

            edge *w = new edge;
            w->edgeWeight = aux_vertex->edgeWeight;
            w->u = &this->list[j];
            w->v = &this->list[aux_vertex->key];

            if(this->list[w->v->key].edgeWeight > this->list[w->u->key].edgeWeight + w->edgeWeight){
                //relax(w->u->key, w->v->key, w);
                return false; //Vai acontecer relaxaento
            }

            aux_vertex = aux_vertex->neighbor;
        }
    }

    return true;
}

shortWayResponse *Graph::shortWay(int u, int v){

    shortWayResponse *ret = new shortWayResponse;
    ret->cost = 0;

    if(this->list[u].neighbor == 0 || this->list[v].neighbor == 0){
        ret->message = "Some one of the vertexes isn't valid";
        return ret;
    }


    vector<int> *path = new vector<int>;

    //Bellman ford está errado
    bellmanFord(u);

    Vertex *at = &this->list[v], *s = &this->list[u];
    //ret->cost += at->vertexWeight;
    while(at != s){ //Esta condição não serve
        path->push_back(at->key);
        at = at->predecessor ? &this->list[at->predecessor->key] : 0;

        if(path->size() > 3*this->VERTEX_QTT){
            at = &this->list[u], s = &this->list[v];
            delete path;
            path = new vector<int>;
            //ret->cost += at->vertexWeight;
            while(at != s){ //Esta condição não serve
                path->push_back(at->key);
                at = at->predecessor ? &this->list[at->predecessor->key] : 0;

                if(path->size() > 3*this->VERTEX_QTT){
                    ret->message = "Algoritm error";
                    ret->cost = 0;
                    return ret;
                }
            }
            break;
        }
    }

    path->push_back(at->key);

    vector<int> *aux = new vector<int>;
    if(s == &this->list[u]){
        while(path->size() != 0){
            aux->push_back(path->at(path->size()-1));
            path->pop_back();
        }
        delete path;
    }
    else {
        aux = path;
    }

    ret->path = aux;

    //Calculating cost
    for (int i = 0; i < aux->size()-1; i++) {
        int u = aux->at(i);
        int v = aux->at(i+1);
        ret->cost += *(this->getWeight(u, v));
    }

    //Decrement way
    for (int i = 0; i < aux->size()-1; i++) {
        int u = aux->at(i);
        int v = aux->at(i+1);
        int *size = this->getWeight(u, v);
        this->setWeight(u, v, *size-1);
        delete size;
    }

    //Cleanning
    for (int i = 0; i < VERTEX_QTT; i++) {
        this->list[i].vertexWeight = UNDEFINED;
        this->list[i].predecessor = 0;
    }

    ret->sucess = true;

    return ret;
}
