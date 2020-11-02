

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <sys/types.h>
#include <unistd.h> //close
pthread_mutex_t mutex;
int clients_list[20];
int n = 0;
int number_exit = 0, number_connect = 0;
////
void *conectLocGen(void *sockarg) {
  int sockfd = *((int *)sockarg);
  char buff[100];
  int n;
  for (;;) {
    bzero(buff, 100);
    n = 0;
    while ((buff[n++] = getchar()) != '\n')
      ;
    write(sockfd, buff, sizeof(buff));
    if (strncmp("exit", buff, 4) == 0) {
      printf("Exit...\n");
      break;
    }
  }
}
int DeserializeInt(char *buffer) {
  int value = 0;

  value |= buffer[0] << 24;
  value |= buffer[1] << 16;
  value |= buffer[2] << 8;
  value |= buffer[3];
  return value;
}
/////
void *RecvMess(void *server_sock) {
  int sock = *((int *)server_sock);
  char msg[500];
  //char msg_name[500];
  int len;
  int i = 0;
  //recv(sock, msg_name, 500, 0);
  //printf("%s",msg_name);
  while ((len = recv(sock, msg, 500, 0)) > 0) {
    int x1, x2, y1, y2;
    x1 = DeserializeInt(msg);
    x2 = DeserializeInt(msg + 4);
    y1 = DeserializeInt(msg + 8);
    y2 = DeserializeInt(msg + 12);
    // msg[len] = '\0';
    printf("(X1:%d ,", x1);
    printf(" X2:%d ,", x2);
    printf(" Y1:%d ,", y1);
    printf(" Y2:%d)\n", y2);
  }
}

int main() {
  struct sockaddr_in address;
  pthread_t recvt, connecLoc;
  int server_sock = 0;
  int max_sd, activity, new_socket;
  fd_set readfds;
  address.sin_family = AF_INET;
  address.sin_port = htons(7473);
  address.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_sock = socket(AF_INET, SOCK_STREAM, 0);

  bind(server_sock, (struct sockaddr *)&address, sizeof(address));
  if (listen(server_sock, 20) == -1)
    printf("listening failed n");

  while (1) {

    FD_ZERO(&readfds);

    FD_SET(server_sock, &readfds);
    max_sd = server_sock;
    activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
    // server khong nhan duoc request tu client thi readfds khong con chua
    // master_socket nua

    if (activity == 0 && number_connect == 0) {
      break;
    }
    if (FD_ISSET(server_sock, &readfds)) {
      if ((new_socket = accept(server_sock, (struct sockaddr *)NULL, NULL)) < 0)
        printf("accept failed n");
      else
        number_connect++;

      // conectLocGen(new_socket);
      pthread_create(&connecLoc, NULL, (void *)conectLocGen, &new_socket);
      pthread_mutex_lock(&mutex);
      clients_list[n] = new_socket;
      n++;
      
      pthread_create(&recvt, NULL, (void *)RecvMess, &new_socket);
      pthread_mutex_unlock(&mutex);
    }
  }
  return 0;
}