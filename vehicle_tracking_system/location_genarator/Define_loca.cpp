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
    Name_SenSor temp  = *(Name_SenSor*)(arg);
    int a;
    //char name[]= "SS1";
    char status[] = "OPEN";
    char status_exit[] = "exit";
    char message[20] = "OKE";
    char buff[20];
    cout << "welcome" <<temp.Name<<"\n";
    int sock;
    sock = ConnectToVts();
    DataStruct DataSensor;
    while(temp.flag == 0)
    {
        DataSensor.SetStatus(status);
        DataSensor.SetID(&a);
        DataSensor.SetName(temp.Name);
        DataSensor.SetCoordinate();
        write(sock,&DataSensor,sizeof(DataSensor));
        read(sock,buff,sizeof(buff));
        if(strncmp(buff,"OKE",sizeof("OKE")-1)!= 0) // nhan error
        {
            temp.flag ==1;
            break;
        }
        temp = *(Name_SenSor*)(arg);
    }
    if(temp.flag == 1)
    {
        cout << "SS2 send exit" <<"\n";
        DataSensor.SetStatus(status_exit);
        DataSensor.SetID(&a);
        DataSensor.SetName(temp.Name);
        DataSensor.SetCoordinate();
        write(sock,&DataSensor,sizeof(DataSensor));
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
char* DataStruct::SetName(char *arg)
{
    strcpy(Name,arg);
    return Name;
}
void DataStruct::SetStatus(char*arg)
{
    strcpy(status,arg);
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
    Name_SenSor sensor1; 
    Name_SenSor sensor2;
    Name_SenSor sensor3;
    sensor1.flag = 1;
    strcpy(sensor1.Name,"SenSor1");
    //sensor1.Name = SenSor1;
    sensor2.flag = 1;
    strcpy(sensor2.Name,"SenSor2");
    sensor3.flag = 1;
    strcpy(sensor3.Name,"SenSor3");
    pthread_t sensor1_thread;
    pthread_t sensor2_thread;
    pthread_t sensor3_thread;
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
    cout << "da ket noi" << "\n";
    while(1)
    {
        read(newsocket,&buffer,sizeof(buffer));
        if (!strncmp(buffer,"open1", sizeof("open1") - 1) && sensor1.flag!=0)
        {
            cout << "..." << "\n";
            sensor1.flag = FALSE;
            pthread_create(&sensor1_thread,NULL,Add_Sensor,&sensor1);
        }
        if (!strncmp(buffer,"close1", sizeof("close1") - 1))
        {
            sensor1.flag = TRUE;
            pthread_join(sensor1_thread,NULL);
           // pthread_exit()
        }

        if (!strncmp(buffer,"open2", sizeof("open2") - 1))
        {
            sensor2.flag = FALSE;
            pthread_create(&sensor2_thread,NULL,Add_Sensor,&sensor2);
        }

        if (!strncmp(buffer,"close2", sizeof("close2") - 1))
        {
            sensor2.flag = TRUE;
            pthread_join(sensor2_thread,NULL);

        }
        bzero(buffer, sizeof(buffer));

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
    if(inet_pton(AF_INET, "192.168.1.6", &serv_addr.sin_addr) <= 0)  
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