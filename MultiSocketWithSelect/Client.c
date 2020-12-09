
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define MAX 1024
void ChatWithServer(int clientSocket) 
{ 
    char buffC[MAX]; 
    char buffS[MAX]; 
    int n; 
    for (;;) { 
        bzero(buffC, sizeof(buffC)); 
        printf("Send message to Server : "); 
        n = 0; 
        
        while ((buffC[n++] = getchar()) != '\n');  
         if ((strncmp(buffC, "exit", 4)) == 0) { 
             send(clientSocket, buffC, sizeof(buffC), 0); 
            printf("Client Exit...\n"); 
            break; 
        } 
        send(clientSocket, buffC, sizeof(buffC), 0); 
        read(clientSocket, buffS, sizeof(buffS));
        printf("Message from Server : %s", buffS);     
       
    } 
} 
int main() {
  int clientSocket;
  struct sockaddr_in serverAddr;
  socklen_t addr_size;
  char buffS[MAX];
  char buffC[MAX] = "client";
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);

  serverAddr.sin_family = AF_INET;

  serverAddr.sin_port = htons(7473);

  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *)&serverAddr, addr_size);
  ChatWithServer(clientSocket);
  close(clientSocket);
}
