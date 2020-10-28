#include <stdio.h>
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <pthread.h> 
#include "Define_loca.h"
void *End_user (void *arg)
{
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
    server_addr.sin_port = htons(PORT);
    
    if(bind(listenfd,(sockaddr*)&server_addr, sizeof(server_addr)) <0)
    {
        close(listenfd);
        exit(2);
    } // bind IP address for server
    //NOTE: in server you must bind the Ip add for it, in client you just connect to server by server add
    listen(listenfd,3); // waiting for connection maximum 3 geue
    newsocket = accept(listenfd,(sockaddr*) &server_addr,(socklen_t*)&addr_lenght);
}