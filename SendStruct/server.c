#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int DeserializeInt( char *buffer) 
{
  int value = 0;

  value |= buffer[0] << 24;
  value |= buffer[1] << 16;
  value |= buffer[2] << 8;
  value |= buffer[3];
  return value;
}

char DeserializeChar( char *buffer) 
{ 
	return buffer[0]; 
}

int main(int argc, char const *argv[]) 
{
  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	 {
    perror("socket failed");
    exit(EXIT_FAILURE);
   }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) 
	{
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  if (listen(server_fd, 5) < 0) 
	{
    perror("listen");
    exit(EXIT_FAILURE);
  }
  if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t *)&addrlen)) < 0) 
	{
    perror("accept");
    exit(EXIT_FAILURE);
  }
   read(new_socket, buffer, 1024);
  // printf("%s\n",buffer );
  int id = DeserializeInt(buffer);
  printf("ID from client : %d\n", id);
  char data;
  data = DeserializeChar(buffer + 4);
  printf("Data from client : %c\n", data);
  return 0;
	
}
