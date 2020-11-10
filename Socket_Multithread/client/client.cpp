#include <stdio.h>
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include<pthread.h> 
#define MAX 100
#define PORT 8888
using namespace std;
char CHECK[MAX];

void *mythread (void *arg)
{
    int newarg = *(int*)(arg);
    char buff[MAX];                                                                                                                                                                                                                                                      
    while(strncmp(CHECK,"exit",4)!=0)
    {
        bzero(buff,sizeof(buff));
        read(newarg,buff, sizeof(buff));
        cout<<buff;
    }

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
		write(sockfd, buff, sizeof(buff)); 
        if ((strncmp(buff, "exit", 4)) == 0) { 
            strcpy(CHECK,buff);
			printf("Client Exit...\n"); 
			break; 
		}  
	} 
} 
int main()
{
    pthread_t pth;
    int sock = 0; //creat a socket
    sockaddr_in serv_addr; // creat server_addr
    memset(&serv_addr,0,sizeof(serv_addr));
    sock = socket(AF_INET,SOCK_STREAM, 0);
    // bind the add to socket
    if(inet_pton(AF_INET, "192.168.83.10", &serv_addr.sin_addr)<=0)  
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