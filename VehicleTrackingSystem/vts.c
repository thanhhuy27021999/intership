

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
#define MAX 1024
pthread_mutex_t mutex;
int clients_list[20];
int n = 0;
char msg[500];
int len;
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
// Recv coords from each Sensor
void *RecvMess(void *server_sock) {
  int sock = *((int *)server_sock);
  
  int i = 0;
  while ((len = recv(sock, msg, 500, 0)) > 0) {
    int SensorId, x1, x2, y1, y2;

    SensorId = DeserializeInt(msg);
    x1 = DeserializeInt(msg + 4);
    x2 = DeserializeInt(msg + 8);
    y1 = DeserializeInt(msg + 12);
    y2 = DeserializeInt(msg + 16);
    printf("Sensor %d  ", SensorId);
    printf("(X1:%d ,", x1);
    printf(" X2:%d ,", x2);
    printf(" Y1:%d ,", y1);
    printf(" Y2:%d)\n", y2);
  }
}
////// Connect to CLI user

void *SendSensorData(void *arg) {
  int serverSocket = *((int *)arg);
  char buffC[MAX];
  char buffS[MAX];
  int n;
  for (;;) {
    read(serverSocket, buffC, sizeof(buffC));
    if ((strncmp(buffC, "sensor", 6)) == 0) 
    
    {
    printf("Send message to CLI User : ");
    send(serverSocket, msg, sizeof(msg), 0);
    }
    
  }
}

void *ConnectCLI(void* arg) 
{
  int master_socket1, addrlen1, new_socket1, client_socket1[30];
  int max_clients1 = 30,  valread1, sd1;
  int max_sd1;
  struct sockaddr_in address1;
  struct timeval time_out;
  time_out.tv_usec = 0;
  char buffer1[1025];

  fd_set readfds;

  for (int i = 0; i < max_clients1; i++) {
    client_socket1[i] = 0;
  }

  if ((master_socket1 = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  address1.sin_family = AF_INET;
  address1.sin_addr.s_addr = INADDR_ANY;
  address1.sin_port = htons(8888);

  if (bind(master_socket1, (struct sockaddr *)&address1, sizeof(address1)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(master_socket1, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  addrlen1 = sizeof(address1);
  pthread_t tid[3];
  int i = 0;
  while (1) {
    new_socket1 = accept(master_socket1, (struct sockaddr *)&address1,
                        (socklen_t *)&addrlen1);
    pthread_create(&tid[i++], NULL, SendSensorData, &new_socket1);
    if (i >= 3) {
      i = 0;
      while (i < 3) {
        pthread_join(tid[i++], NULL);
      }
      i = 0;
    }
  }
}

///////////////////////////

int main()
 {
  pthread_t recvt, connecLoc,connecCLIUser;
   pthread_create(&connecCLIUser,NULL,&ConnectCLI,NULL);
  struct sockaddr_in address;
  
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

    if (FD_ISSET(server_sock, &readfds)) {
      if ((new_socket = accept(server_sock, (struct sockaddr *)NULL, NULL)) < 0)
        printf("accept failed n");

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





