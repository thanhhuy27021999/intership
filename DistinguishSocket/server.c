#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 7473 
int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int addrlen = sizeof(address); 
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    char *data = "Message from server"; 
    socklen_t optlen;
    int rcvbuf;
    char buff[100];
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    
   bind(server_fd, (struct sockaddr *)&address,sizeof(address));
    
   listen(server_fd, 3);
    int type;
    new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen);
    read(server_fd,buff,100);
    getsockopt(server_fd,SOL_SOCKET,SO_RCVBUF,&rcvbuf,&optlen);
    getsockopt(server_fd,SOL_SOCKET,SO_TYPE,&type,&optlen);
    printf("Length data from client : %d\n",rcvbuf);
    printf("Type socket : %d\n",type);
    printf("IP address of socket client: %s\n", inet_ntoa(address.sin_addr));
    //printf("port is: %d\n", (int) ntohs(address.sin_port));
    
    return 0; 
} 