#include <arpa/inet.h>
#include <ctype.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define MAX 1024
int CheckStringIsNumber(char *a) {
  int c = 1;
  for (int i = 0; i < strlen(a - 1); i++) {
    if (isdigit(a[i]) == 0) {
      c = 0;
      break;
    }
  }
  return c;
}
int DeserializeInt(char *buffer) {
  int value = 0;

  value |= buffer[0] << 24;
  value |= buffer[1] << 16;
  value |= buffer[2] << 8;
  value |= buffer[3];
  return value;
}
void RecvMess(int sock) {
  //  int sock = *((int *)server_sock);
  char msg[50000];
  int len;
  int i = 0;
  char buff[100];
  int n = 0;

  for (;;) {
    bzero(buff, sizeof(buff));

    n = 0;
    while ((buff[n++] = getchar()) != '\n')
      ;
    // printf("Nhap vao buff : ");
    // scanf("%s",buff);
    if ((strncmp(buff, "exit", 4)) == 0)
      break;
    char id_check[100];
    for (int i = 0; i < strlen(buff); i++) {
      id_check[i] = buff[i + 3];
    }

    if (buff[0] == 'g' && buff[1] == 'e' && buff[2] == 't' &&buff[3] == ':') 
    {

      write(sock, buff, sizeof(buff));
      recv(sock, msg, sizeof(msg), 0);
      printf("%s", msg);
    }
     if (buff[0] == 'l' && buff[1] == 'i' && buff[2] == 's' && buff[3] == 't' )
   {
     write(sock, buff, sizeof(buff));
     recv(sock, msg, sizeof(msg), 0);
     printf("%s", msg);
   }
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

  serverAddr.sin_port = htons(8888);

  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *)&serverAddr, addr_size);
  // ChatWithServer(clientSocket);
  RecvMess(clientSocket);
  close(clientSocket);
}
