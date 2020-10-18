#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

pthread_mutex_t mutex;
int clients_list[20];
int n=0;
int number_exit=0,number_connect=0;
void BroadcastMess(char *msg,int curr_socket)
//void BroadcastMess(char *msg)
{
	int i;
	pthread_mutex_lock(&mutex);
	for(i = 0; i < n; i++) 
    {
		if(clients_list[i] != curr_socket) 
        {
            send(clients_list[i],msg,strlen(msg),0);
		}
	}
	pthread_mutex_unlock(&mutex);
}

void *RecvMess(void *server_sock){
	int sock = *((int *)server_sock);
	char msg[500];
	int len;
	while((len = recv(sock,msg,500,0)) > 0) {
		msg[len] = '\0';
        if(msg[strlen(msg+1)]=='t'&& msg[strlen(msg+2)]=='i'&& msg[strlen(msg+3)]=='x'&& msg[strlen(msg+4)]=='e')
        number_connect -=1;
		BroadcastMess(msg,sock);
         printf("xxxxxxx : %d\n",number_connect);
        //BroadcastMess(msg);

	}
	
}

int main()
{
	struct sockaddr_in address;
	pthread_t recvt;
	int  server_sock=0;
	int max_sd,activity,new_socket;
    fd_set readfds;
    struct timeval time_out;
    time_out.tv_usec = 0;
	address.sin_family = AF_INET;
	address.sin_port = htons(7473);
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_sock = socket( AF_INET , SOCK_STREAM, 0 );
	
	bind( server_sock, (struct sockaddr *)&address, sizeof(address));
	if( listen( server_sock ,20 ) == -1 )
		printf("listening failed n");
		
	while(1)
    {
        ////
     time_out.tv_sec = 10;

    FD_ZERO(&readfds);

    FD_SET(server_sock, &readfds);
    max_sd = server_sock;
    activity = select(max_sd + 1, &readfds, NULL, NULL, &time_out);
    // server khong nhan duoc request tu client thi readfds khong con chua master_socket nua
    
    if (activity == 0 && number_connect==0) 
    {
      break;
    }
   //////
     if (FD_ISSET(server_sock, &readfds)) 
     {
		if( (new_socket = accept(server_sock, (struct sockaddr *)NULL,NULL)) < 0 )
			printf("accept failed  n");
            else 
            number_connect ++;

          //  printf("nm conet : %d\n",number_connect);
		pthread_mutex_lock(&mutex);
		clients_list[n]= new_socket;
		n++;
		// creating a thread for each client 
		pthread_create(&recvt,NULL,(void *)RecvMess,&new_socket);
		pthread_mutex_unlock(&mutex);
	}
    }
	return 0; 
	
	
}