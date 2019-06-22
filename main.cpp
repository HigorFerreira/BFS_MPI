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

        //Get Buffer Total Size
        int bufSize = *(int*)data;

        //Send Graph to servos
        for (int i = 0; i < servos->size(); i++) {
            MPI_Send(data, bufSize, MPI_BYTE, i, 0, MPI_COMM_WORLD);
        }

        while (true) {

        }
    }
    else{
        Graph *g = new Graph(1);
        void *data = 0;

        //Waiting for Graph
        while (true) {

            //Receive Graph from commander
            MPI_Recv(data, 0x1BFFF, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if(data){
                g->mountGraph(data);
                delete data;
                break;
            }
        }

        //Waiting for requests
        while(true){
            MPI_Recv(data, 5, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if(*(char*)data == 'C'){
                char *res = new char[0xFFFFFF];
                char *resbuf = res;

                //4Bytes for buffer total size
                resbuf += 4;

                shortWayResponse *sw = g->shortWay(*(unsigned short*)data+1, *(unsigned short*)data+2);
                *(int*)resbuf = sw->cost;
                resbuf += 4;
                *(int*)resbuf = sw->path->size();
                resbuf += 4;

                for (int i = 0; i < sw->path->size(); i++) {
                    *(unsigned short*)resbuf = (unsigned short)sw->path->at(i);
                    resbuf += 2;
                }

                void *newGraph = g->serialize();
                for (int i = 0; i < *(int*)newGraph; i++) {
                    *(char*)resbuf = *((char*)newGraph+i);
                    resbuf++;
                }

                *(int*)res = resbuf - res;
                delete resbuf;

                MPI_Send(res, *(int*)res, MPI_BYTE, 0, 0, MPI_COMM_WORLD);

            }
            else {
                bool *res = new bool(false);
                MPI_Send(res, 1, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
            }
        }
    }


}
