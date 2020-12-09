
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

char clientMess[500];
void *RecvMess(void *socket)
{
	int sock = *((int *)socket);
	int len;
	while((len = recv(sock,clientMess,500,0)) > 0) {
		clientMess[len] = '\0';
		fputs(clientMess,stdout);
	}
}

int main(int argc,char *argv[]){
	pthread_t recvt;
	int len;
	int client_sock;
	char send_msg[500];
	struct sockaddr_in address;
	char client_name[100];
	strcpy(client_name, argv[1]);
   // strcpy(client_name, "with IP : 127.0.0.1");
	client_sock = socket( AF_INET, SOCK_STREAM,0);
	address.sin_port = htons(7473);
	address.sin_family= AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	if( (connect( client_sock ,(struct sockaddr *)&address,sizeof(address))) == -1 )
		printf("connection failed");

	pthread_create(&recvt,NULL,(void *)RecvMess,&client_sock);
	
	
    int n =0;
    // while ((msg[n++] = getchar()) != '\n')
	while(fgets(clientMess,500,stdin) > 0) 
    {
        if ((strncmp(clientMess, "exit", 4)) == 0) 
        {
      printf("Client exit...\n");
      //break;
      return 0;
       }
		strcpy(send_msg,client_name);
		strcat(send_msg,":");
		strcat(send_msg,clientMess);
		len = write(client_sock,send_msg,strlen(send_msg));
		
	}
	
	//thread is closed
	pthread_join(recvt,NULL);
	close(client_sock);
	return 0;
}
