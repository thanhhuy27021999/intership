#include <stdio.h>
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define MAX 100
#define PORT 8888
using namespace std;
void func(int sockfd) 
{ 
	char buff[MAX]; 
	int n; 
	for (;;) { 
		bzero(buff, sizeof(buff)); 
		printf("Enter the string : "); 
		n = 0; 
		while ((buff[n++] = getchar()) != '\n') 
			; 
		write(sockfd, buff, sizeof(buff)); 
        if ((strncmp(buff, "exit", 4)) == 0) { 
			printf("Client Exit...\n"); 
			break; 
		}
		bzero(buff, sizeof(buff)); 
		read(sockfd, buff, sizeof(buff)); 
		printf("From Server : %s", buff);  
	} 
} 
int main()
{
    int sock = 0; //creat a socket
    sockaddr_in serv_addr; // creat server_addr
    // char buffer[1024];
    // char String[1024];
    //Student buffer;
   // memset(&buffer,0, sizeof(buffer));
    memset(&serv_addr,0,sizeof(serv_addr));
    sock = socket(AF_INET,SOCK_STREAM, 0);
    // bind the add to socket
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
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
    func(sock);
    // while(1)
    // {
    //     cout <<"Client rep: ";
    //     memset(String,0,sizeof(String));
    //   //  write()
    //     cin.getline(String,sizeof(String));
    //     send(sock,String,sizeof(String),0);
    //     recv(sock,buffer,sizeof(buffer),0);
    //     cout << "Server rep: " << buffer << "\n";
    //     memset(buffer,0,sizeof(buffer));
    // }
    
    close(sock);
    return 0;
}