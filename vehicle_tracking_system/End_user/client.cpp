#include <stdio.h>
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include<pthread.h> 
#include "define.hpp"
#define MAX 100
#define PORT 12345
using namespace std;
char CHECK[MAX];
int flag;

void *mythread (void *arg)
{
    int newarg = *(int*)(arg);
    DataSenSor buff;                                                                                                                                                                                                                                                      
    while(strncmp(CHECK,"exit",sizeof("exit")-1)!=0)
    {
        bzero(&buff,sizeof(buff));
        read(newarg,&buff, sizeof(buff));
        if(flag)
        {
            cout << buff.Name <<"        "<<buff.ID<<"\n";
        }
        else
        {
            Xuat(&buff);
        }
    }
    cout <<"Da exit Client"<<"\n";
}                                                
void func(int sockfd) 
{ 
	char buff[MAX]; 
	int n; 
	for (;;) { 
		bzero(buff, sizeof(buff)); 
		//printf("Me: "); 
		n = 0; 
		while ((buff[n++] = getchar()) != '\n'); 
        if(!strncmp(buff,"GetSenSor",sizeof("GetSenSor")-1))
        {
            flag = 1;
            cout<<"Name         ID \n";
        }  
        else
        {
            flag = 0;
        }
		write(sockfd, buff, sizeof(buff)); 
        if ((strncmp(buff, "exit", 4)) == 0) { 
            strcpy(CHECK,buff);
            //cout << buff<<"\n";
            cout <<CHECK <<"\n";
			//printf("Client Exit...\n"); 
			break; 
		}

        
	} 
} 
int main()
{
    pthread_t pth; // recieve from VTS
    int sock = 0; //creat a socket
    sockaddr_in serv_addr; // creat server_addr
    memset(&serv_addr,0,sizeof(serv_addr));
    sock = socket(AF_INET,SOCK_STREAM, 0);
    // bind the add to socket
    if(inet_pton(AF_INET, "192.168.122.1", &serv_addr.sin_addr)<=0)  
    { 
        cout<<"\nInvalid address/ Address not supported \n"; 
        return -1; 
    } 
    serv_addr.sin_family = AF_INET; // truong dia chi, client khoi tao dia chi cho cua server de connect toi
    serv_addr.sin_port = htons(PORT);

    // connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        cout<<"\nConnection Failed \n"; 
        return -1; 
    }
    do
    {
        (void) pthread_create(&pth,NULL,mythread,&sock);

    } while (false);
    func(sock);
    (void) pthread_join(pth,NULL);
    close(sock);
    return 0;
}