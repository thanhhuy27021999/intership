#include <stdio.h>
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include "Define.h"
#include<pthread.h> 
using namespace std;
int main()
{
    pthread_t My_thread[3];
    pthread_create(&My_thread[0],NULL,End_user,NULL); // recieve from user 
    pthread_create(&My_thread[1],NULL,Admin_th,NULL); // send request to location_generator
    pthread_create (&My_thread[2],NULL,End_user,NULL); // recieve data from loction_gernerator 
    for (int i=0; i<3; i++)
    {
        (void) pthread_join(My_thread[i], NULL);
    }
}