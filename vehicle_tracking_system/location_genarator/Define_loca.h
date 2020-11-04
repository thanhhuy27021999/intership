#include <stdio.h>
#include <iostream>
#include <pthread.h> 
#define     PORT_1    12346
#define     PORT_2    12347
#define     TRUE      1
#define     FALSE     0
#define     SenSor1    "SenSor1"
//static int flag;
void *Recv_from_ad (void *arg); //recieve request from VTS as a server 
void *Add_Sensor(void *arg); //create a connection from local_genarator to VTS as a client
int ConnectToVts(); //sensor send data to VTS
struct Name_SenSor
{
    int flag;
    char Name[20];
};
class DataStruct
{
    private:
        char Name[20];
        char status[20];
        int ID ;
        int lagi, longi;
    public:
        char* SetName(char *arg);
        void SetID (int *arg);
        void SetCoordinate ();
        void SetStatus(char *arg);
};
