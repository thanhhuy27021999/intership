#include <arpa/inet.h> //close
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <sys/types.h>
#include <unistd.h> //close
#define PORT 7473
#define MAX 1024

void ChatWithClient(int serverSocket,struct sockaddr_in address) {
  char buffC[MAX];
  char buffS[MAX];
  int n;
  for (;;) {
    read(serverSocket, buffC, sizeof(buffC));
    printf("Message from Client : %s", buffC);
    if ((strncmp(buffC, "exit", 4)) == 0) {
      printf("Client have IP : %s  exit...\n",inet_ntoa(address.sin_addr));
      break;
    }
    bzero(buffS, sizeof(buffS));
    printf("Send message to Client : ");
    n = 0;
    while ((buffS[n++] = getchar()) != '\n')
      ;
    send(serverSocket, buffS, sizeof(buffS), 0);
  }
}

int main(int argc, char *argv[]) {
  int master_socket, addrlen, new_socket, client_socket[30];
  int max_clients = 30, activity, i, valread, sd;
  int max_sd;
  struct sockaddr_in address;
  struct timeval time_out;
  time_out.tv_sec = 5;
  time_out.tv_usec = 0;
  char buffer[1025];

  // set of socket descriptors
  fd_set readfds; // socket descriptor

  for (i = 0; i < max_clients; i++) {
    client_socket[i] = 0;
  }

  // create a master socket
  if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(master_socket, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  addrlen = sizeof(address);

  while (1) {
    // clear the socket set
    FD_ZERO(&readfds);

    // add master socket to set
    FD_SET(master_socket, &readfds);
    max_sd = master_socket;
    //	max_sd = 2;
    // printf("max_sd = %d\n",max_sd);
    // printf("master_socket = %d\n",master_socket);
    // add child sockets to set
    for (i = 0; i < max_clients; i++) {
      // socket descriptor
      sd = client_socket[i];

      // if valid socket descriptor then add to read list
      if (sd > 0)
        FD_SET(sd, &readfds);

      // highest file descriptor number, need it for the select function
      if (sd > max_sd)
        max_sd = sd;
    }
    // activity = select( max_sd + 1 , &readfds , NULL , NULL , &time_out);
    activity = select(max_sd + 1, &readfds, NULL, NULL, &time_out);
    if ((activity < 0) && (errno != EINTR)) {
      printf("select error");
    }
    // if(activity==0)
    // return 0;
    // break;
    // If something happened on the master socket ,
    // then its an incoming connection
    if (FD_ISSET(master_socket, &readfds)) {
      if ((new_socket = accept(master_socket, (struct sockaddr *)&address,
                               (socklen_t *)&addrlen)) < 0)

      {
        perror("accept");
        exit(EXIT_FAILURE);
      }
      ChatWithClient(new_socket,address);
      // add new socket to array of sockets
      for (i = 0; i < max_clients; i++) {
        // if position is empty
        if (client_socket[i] == 0) {
          client_socket[i] = new_socket;
          break;
        }
      }
    }
  }

  return 0;
}
