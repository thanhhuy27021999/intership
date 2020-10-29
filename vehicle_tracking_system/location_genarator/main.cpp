#include <stdio.h>
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include "Define_loca.h"
using namespace std;

int main()
{
    pthread_t Thread[2];
    pthread_create(&Thread[0],NULL,Recv_from_ad,NULL);
    for(int i =0; i<2; i++)
    {
        pthread_join (Thread[i],NULL);
    }
}