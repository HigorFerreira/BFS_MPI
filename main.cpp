#include <mpi.h>

#include "graph.h"

bool getServos(vector<int> *servos, int size){
    if(size == 1 || size < 0) return false;
    for (int i = 1; i < size; i++) {
        servos->push_back(i);
    }
    return true;
}

bool hasServo(vector<int> *servos){
    return servos->size();
}

bool isServosEmpty(vector<int> *servos){
    return servos->size() == 0;
}


int main()
{
    //Variables to store rank and the number of processes
    int rank, size;

    //Stack to store servo processes
    vector<int> *servos = new vector<int>;

    //Starting MPI (Message Passing Interface)
    MPI_Init(NULL, NULL);

    //Get number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //Get rank
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //Getting servos
    getServos(servos, size);

    if(rank == 0){
        //Creating Graph
        Graph *g = new Graph();
        g->connectAll();

        //Serialize Graph
        void *data = g->serialize();

        char *ptr = (char *)data;
        int buffersize = *(int *)ptr;
        ptr += 4;
        int vertexqtt = *(unsigned short*)ptr;
        ptr += 2;
        int vertex = *(unsigned short*)ptr;
        ptr += 2;
        int vertexQttToTake = *(unsigned short*)ptr;

        int i = 0;
    }
    else{

    }


}
