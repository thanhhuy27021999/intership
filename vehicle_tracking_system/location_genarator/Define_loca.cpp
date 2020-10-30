#include <stdio.h>
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <pthread.h> 
#include "Define_loca.h"
#include <stdlib.h>
using namespace std;

                    /* Send data from sensor to VTS for user */
                    
void *Add_Sensor(void *arg)
{
    int flag = *((int*)(arg));
    int a;
    char name[]= "123";
    //DataSensor DataSensor1;
    cout << "welcom to ss1" <<"\n";
    int sock;
    sock = ConnectToVts();
    DataStruct DataSensor1;
    while(flag == 0)
    {
        DataSensor1.SetID(&a);
        DataSensor1.SetName(name);
        DataSensor1.SetCoordinate();
        write(sock,&DataSensor1,sizeof(DataSensor1));
    }

    close(sock);
}

            /*Define function for class DataStruct*/

void DataStruct::SetCoordinate()
{
    srand((int)time(0));
    longi = rand();
    lagi = rand();
}   
void DataStruct::SetID (int *arg)
{
    srand((int)time(0));
    *arg = rand();
    ID = (*arg);
}
void DataStruct::SetName(char *arg)
{
    strcpy(Name,"Huy");
}

                    /*create a thread for sensor*/

// pthread_t newthread (pthread_t *arg1)
// {
//     pthread_create(arg1,NULL,Add_user,NULL);
//     return (*arg1);
// }



                    /*recieve request from VTS*/

void *Recv_from_ad (void *arg)
{
    static int flag;
    pthread_t sensor1_thread;
    char buffer[100];
    sockaddr_in server_addr;
    int opt =1; 
    int listenfd ; // socket for listenning
    int newsocket   ; //socket for communication between server and client
    int addr_lenght = sizeof(server_addr);
    memset(&buffer,0,sizeof(buffer));
    memset(&server_addr,0,sizeof(server_addr));
    listenfd = socket(AF_INET, SOCK_STREAM, 0);// create a socket 

    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                   &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    };

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT_1);
    
    if(bind(listenfd,(sockaddr*)&server_addr, sizeof(server_addr)) <0)
    {
        close(listenfd);
        exit(2);
    } // bind IP address for server
    //NOTE: in server you must bind the Ip add for it, in client you just connect to server by server add
    listen(listenfd,3); // waiting for connection maximum 3 geue
    newsocket = accept(listenfd,(sockaddr*) &server_addr,(socklen_t*)&addr_lenght);
    while(1)
    {
        read(newsocket,&buffer,sizeof(buffer));
        if (strcmp(buffer,"open1"))
        {
            flag = false;
            //newthread(&sensor1_thread);
            pthread_create(&sensor1_thread,NULL,Add_Sensor,&flag);
        }
        if (strcmp(buffer,"close1"))
        {
            flag = true;
        }

        //pthread_join (sensor1_thread,NULL);
    }
}


                        /*create connection from location_generator to VTS*/

int ConnectToVts()
{
    int sock = 0; //creat a socket
    sockaddr_in serv_addr; // creat server_addr
    memset(&serv_addr,0,sizeof(serv_addr));
    sock = socket(AF_INET,SOCK_STREAM, 0);
    // bind the add to socket
    if(inet_pton(AF_INET, "192.168.83.10", &serv_addr.sin_addr) <= 0)  
    { 
        cout<<"\nInvalid address/ Address not supported \n"; 
        return 0; 
    } 
    serv_addr.sin_family = AF_INET; // truong dia chi, client khoi tao dia chi cho cua server de connect toi
    serv_addr.sin_port = htons(PORT_2);

    // connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        cout<<"\nConnection Failed \n"; 
        return 0; 
    }
    return sock;   
}