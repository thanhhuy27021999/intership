#include <stdio.h>
#include <iostream>
#include <pthread.h> 
#define     PORT    8888
void *Recv_from_ad (void *arg); //recieve request from VTS as a server 
void *Add_user(void *arg); //create a connection from local_genarator to VTS as a client
int ConnectToVts(); //sensor send data to VTS
pthread_t newthread (pthread_t *arg1); // creat a thread for a sensor
class DataStruct
{
    private:
        char Name[];
        int ID ;
        int lagi, longi;
    public:
        void SetName(char *arg);
        void SetID (int *arg);
        void SetCoordinate ();
};
