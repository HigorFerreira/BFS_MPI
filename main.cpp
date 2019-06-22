#include <mpi.h>

#include <stdio.h>
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

int popServo(vector<int> *servos){
    int ret = servos->at(servos->size()-1);
    servos->pop_back();
    return ret;
}

int main()
{
    //Variables to store rank and the number of processes
    int rank, size;

    //Stack to store servo processes
    vector<int> *servos = new vector<int>;
    vector<int> *servosStack = new vector<int>;

    //Starting MPI (Message Passing Interface)
    MPI_Init(NULL, NULL);

    //Get number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //Get rank
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //Getting servos
    getServos(servos, size);
    getServos(servosStack, size);

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
            int u, v;

            while (u==v) {
                u = g->randd();
                v = g->randd();
            }

            if(hasServo(servosStack)){
                //Mounting Request for short way
                char *data = new char('C');
                *(unsigned short*)(data+1) = (unsigned short)u;
                *(unsigned short*)(data+3) = (unsigned short)v;

                //Sending request
                MPI_Send(data, 5, MPI_BYTE, popServo(servosStack), 0, MPI_COMM_WORLD);
            }

            void *data;

            for (int i = 1; i < servos->size()+1; i++) {
                //Wait for response from some servo
                MPI_Recv(data, 0x1BFFF, MPI_BYTE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                char *dataPointer = (char*)data;

                int responseRank = *(unsigned short*)dataPointer;
                dataPointer += 2;
                char responseType = *(char*)dataPointer;
                dataPointer++;
                int bufferSize = *(int*)dataPointer;
                dataPointer += 4;

                int cost = *(int*)dataPointer; dataPointer += 4;
                int waySize = *(int*)dataPointer; dataPointer += 4;

                char *way = new char[waySize*3+1];
                char *wayPointer = way;

                //Releasing unused space on memory
                char *toErase = (char*)data+bufferSize;
                delete toErase;

                if(responseRank == i){

                    servosStack->push_back(i);

                    //Taking way string to show
                    if(responseType == 'C'){

                        for (int i = 0; i < waySize; i++) {

                            *way = (char)*(unsigned short*)dataPointer;
                            *(way+(i*3)) = ', ';

                            dataPointer += 2;
                        }

                        *(way+waySize*3) = 0x00;

                        //Remounting Graph
                        g->mountGraph(dataPointer);

                        //Send new graph to all Servos
                        void *graphSerialized = g->serialize();

                        //Get Buffer Total Size
                        int bufSize = *(int*)graphSerialized;

                        char *data = new char[bufSize+1];
                        char *dataPointer = data;

                        //Attach command type to servo
                        *dataPointer = 'R'; dataPointer++;

                        //Copy Graph
                        for (int i = 0; i < bufSize; i++) {
                            *dataPointer = *(char*)graphSerialized;
                            dataPointer++;
                        }

                        bufferSize++;

                        //Send Graph to servos
                        for (int i = 0; i < servos->size(); i++) {
                            MPI_Send(data, bufSize, MPI_BYTE, i, 0, MPI_COMM_WORLD);
                        }

                    }
                }
            }

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

                //Store servo rank
                *(unsigned short*)resbuf = (unsigned short)rank;
                resbuf += 2;

                //Store ResponseType
                *(char*)resbuf = 'C';
                resbuf++;

                //4Bytes for buffer total size
                int *bufferSize = (int*)resbuf;
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

                //Store buffer size
                *bufferSize = resbuf - res;
                delete resbuf;

                MPI_Send(res, *(int*)res, MPI_BYTE, 0, 0, MPI_COMM_WORLD);

            }
            else if (*(char*)data == 'R') {
                char *graph = (char*)data+1;
                g->mountGraph(graph);
            }
            else {
                bool *res = new bool(false);
                MPI_Send(res, 1, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
            }
        }
    }


}
