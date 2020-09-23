#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

struct Test {
  int Id;
  char data;
};

unsigned char *SerializeInt(unsigned char *buffer, int value) 
{

  buffer[0] = value >> 24;
  buffer[1] = value >> 16;
  buffer[2] = value >> 8;
  buffer[3] = value;
  return buffer + 4;
}

unsigned char *SerializeChar(unsigned char *buffer, char value) 
{
  buffer[0] = value;
  return buffer + 1;
}

unsigned char *SerializeTest(unsigned char *buffer, struct Test value) 
{
  buffer = SerializeInt(buffer, value.Id);
  buffer = SerializeChar(buffer, value.data);
  // for(int i=0; i<strlen(value.Name); i++)
  //   buffer = serialize_char(buffer,value.Name[i]);
  return buffer;
}

int main(int argc, char const *argv[]) 
{
  int sock = 0;
  struct sockaddr_in serv_addr;
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
    printf("\n Socket creation error \n");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) 
	{
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{
    printf("\nConnection Failed \n");
    return -1;
  }
  /////
  struct Test mess;
  mess.Id = 1;
  mess.data = 's';
  unsigned char buffer[32], *ptr;
  ptr = SerializeTest(buffer, mess);
  send(sock, buffer, ptr - buffer, 0);
  return 0;

}
