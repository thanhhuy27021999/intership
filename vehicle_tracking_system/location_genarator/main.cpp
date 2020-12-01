#include <stdio.h>
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include "Define_loca.h"
#include <pthread.h>
using namespace std;
DataStruct sensor[30];
int main()
{
    pthread_t Thread[2];
    pthread_create(&Thread[0],NULL,Recv_from_ad,NULL);
    for(int i =0; i<2; i++)
    {
        pthread_join (Thread[i],NULL);
    }
}