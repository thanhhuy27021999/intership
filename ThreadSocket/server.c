#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
char client_message[2000];
//char buffer[1024] = "Message from server";
char buffer[1024];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
void *SocketThread(void *arg) 
{
  int newSocket = *((int *)arg);
  recv(newSocket, client_message, 2000, 0);
  printf("%s\n", client_message);
  pthread_mutex_lock(&lock);
  int n=0;
  while ((buffer[n++] = getchar()) != '\n')
  pthread_mutex_unlock(&lock);
  sleep(1);
  send(newSocket, buffer, strlen(buffer), 0);
  printf("Exit socketThread \n");
  close(newSocket);
  pthread_exit(NULL);
}

int main() 
{

  int serverSocket, newSocket;
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  serverSocket = socket(PF_INET, SOCK_STREAM, 0);

  serverAddr.sin_family = AF_INET;

  serverAddr.sin_port = htons(7799);

  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

  listen(serverSocket, 50);

  pthread_t tid[3];
  int i = 0;
  while (1) 
  {

    addr_size = sizeof serverStorage;
    //accept all request
    newSocket = accept(serverSocket, (struct sockaddr *)&serverStorage, &addr_size);
    pthread_create(&tid[i++], NULL, SocketThread, &newSocket);
    if (i >= 3) 
     {
       i = 0;
       while (i < 3) 
        {
         pthread_join(tid[i++], NULL);
        }
       i = 0;
      }
    }
  return 0;
  
}
