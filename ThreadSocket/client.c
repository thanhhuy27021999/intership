#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
void *ClientThread(void *arg) 
{
  printf("In thread\n");
  char message[1000] = "Message from client";
  char buffer[1024];
  int clientSocket;
  struct sockaddr_in serverAddr;
  socklen_t addr_size;
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);

  serverAddr.sin_family = AF_INET;

  serverAddr.sin_port = htons(7799);

  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *)&serverAddr, addr_size);
  send(clientSocket, message, strlen(message), 0);

  recv(clientSocket, buffer, 1024, 0);

  printf("%s\n", buffer);
  close(clientSocket);
  pthread_exit(NULL);
}

int main() 
{
  int i = 0;
  pthread_t tid[3];
  while (i < 2) 
  {
    pthread_create(&tid[i], NULL, ClientThread, NULL);
        i++;
  }
    sleep(20);
    i = 0;
    while (i < 2) 
    {
      pthread_join(tid[i++], NULL);
      printf("%d:\n", i);
    }
    return 0;
  
}
