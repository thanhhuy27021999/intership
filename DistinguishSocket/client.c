#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define PORT 7473 
   
int main(int argc, char const *argv[]) 
{ 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *data = "Message from client"; 
    char buffer[1024] = {0}; 
    sock = socket(AF_INET, SOCK_STREAM, 0);
    socklen_t optlen;
   // char* buffer;
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
    //read( sock , buffer, 1024); 
    int rcvbuf;
    
    inet_pton(AF_INET, "10.230.5.186", &serv_addr.sin_addr);
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
     send(sock , data , strlen(data) , 0 ); 
    return 0; 
} 