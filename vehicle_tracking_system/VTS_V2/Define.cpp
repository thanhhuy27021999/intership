#include <stdio.h>
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <pthread.h> 
#include "Define.h"
int cnt;
int escape;
int  client_socket[30];
struct sockaddr_in address;   
struct timeval timeout;
using namespace std;



void *Admin_th (void *arg)
{
    int sock = 0; //creat a socket
    sockaddr_in serv_addr; // creat server_addr
    memset(&serv_addr,0,sizeof(serv_addr));
    sock = socket(AF_INET,SOCK_STREAM, 0);
    // bind the add to socket
    if(inet_pton(AF_INET, "192.168.83.10", &serv_addr.sin_addr) <= 0)  
    { 
        cout<<"\nInvalid address/ Address not supported \n"; 
        return 0; 
    } 
    serv_addr.sin_family = AF_INET; // truong dia chi, client khoi tao dia chi cho cua server de connect toi
    serv_addr.sin_port = htons(PORT_1);

    // connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        cout<<"\nConnection Failed (teminal for admin) \n"; 
        return 0; 
    }
    cout << "connected to local" <<"\n";
    func(sock);
    close(sock);
    return 0;
}


void func(int sockfd) 
{ 
	char buff[MAX]; 
	int n; 
	for (;;) 
    { 
		bzero(buff, sizeof(buff)); 
		//printf("Me: "); 
		n = 0; 
		while ((buff[n++] = getchar()) != '\n'); 
		write(sockfd, buff, sizeof(buff)); 
        if ((strncmp(buff, "exit", 4)) == 0) 
        { 
            //strcpy(CHECK,buff);
			printf("Admin Exit...\n"); 
			break; 
		}  
	} 
} 

void *My_thread_1 (void *arg)
{
    int newarg = *(int*)(arg);
    struct sockaddr_in temp;
    temp = address;
    DataStruct buff; 
    //char buff_T[MAX];
	int n; 
	for (;;) 
    { 
		bzero(&buff, sizeof(buff)); 
        read(newarg, &buff, sizeof(buff)); 
        //buff.Xuat();
        //write(newarg, &buff, sizeof(buff_T)); //tranfer data 
        // strcpy(buff_T,"Ip ");
        // strcat(buff_T,inet_ntoa(temp.sin_addr));
        // strcat(buff_T,":");
        // strcat(buff_T,buff);
        // for(int i=0; i<30; i++)
        // {
        //     if(client_socket[i] == 0)
        //     break;
        //     write(client_socket[i], buff_T, sizeof(buff_T)); 
        // }
        //write(newarg,buff,sizeof(buff));
        // if ((strncmp(buff, "exit", 4)) == 0) 
        // { 
		// 	printf("Client Exit...\n");
        //     cnt--;
        //     for(int i=0; i<30;i++)
        //     {
        //         if(client_socket[i]==newarg)
        //         {
        //             for(int j=i;j<30;j++)
        //             {
        //                 int a;
        //                 a= j+1;
        //                 client_socket[j] = client_socket[a];
        //             }
        //             break;
        //         }
        //     }
        //     close(newarg); 
		// 	break; 
		// }
		bzero(&buff, sizeof(buff)); 
	}  
}


void *My_thread (void *arg)
{
    int newarg = *(int*)(arg);
    struct sockaddr_in temp;
    temp = address;
    char buff[MAX]; 
    char buff_T[MAX];
	int n; 
	for (;;) 
    { 
		bzero(buff, sizeof(buff)); 
        read(newarg, buff, sizeof(buff)); 
        write(newarg, buff, sizeof(buff)); //tranfer data 
        // strcpy(buff_T,"Ip ");
        // strcat(buff_T,inet_ntoa(temp.sin_addr));
        // strcat(buff_T,":");
        // strcat(buff_T,buff);
        // for(int i=0; i<30; i++)
        // {
        //     if(client_socket[i] == 0)
        //     break;
        //     write(client_socket[i], buff_T, sizeof(buff_T)); 
        // }
        //write(newarg,buff,sizeof(buff));
        if ((strncmp(buff, "exit", 4)) == 0) 
        { 
			printf("Client Exit...\n");
            cnt--;
            for(int i=0; i<30;i++)
            {
                if(client_socket[i]==newarg)
                {
                    for(int j=i;j<30;j++)
                    {
                        int a;
                        a= j+1;
                        client_socket[j] = client_socket[a];
                    }
                    break;
                }
            }
            close(newarg); 
			break; 
		}
		bzero(buff, sizeof(buff_T)); 
	}  
}


void *End_user (void *arg)
{
    escape = 1;
    cnt = 0;
    int opt = TRUE;   
    int master_socket , addrlen , new_socket ,  
    max_clients = 30 , activity, i , valread , sd;   
    int max_sd;   
    pthread_t pt[30];         
    //set of socket descriptors  
    fd_set readfds;   
         
    //a message  
    char *message = "ECHO Daemon v1.0 \r\n";   
     
    //initialise all client_socket[] to 0 so not checked  
    for (i = 0; i < max_clients; i++)   
    {   
        client_socket[i] = 0;   
    }   
         
    //create a master socket  
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)   
    {   
        perror("socket failed");   
        exit(EXIT_FAILURE);   
    }   
     
    //set master socket to allow multiple connections ,  
    //this is just a good habit, it will work without this  
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,  
          sizeof(opt)) < 0 )   
    {   
        perror("setsockopt");   
        exit(EXIT_FAILURE);   
    }   
     
    //type of socket created  
    address.sin_family = AF_INET;   
    address.sin_addr.s_addr = INADDR_ANY;   
    address.sin_port = htons(PORT);   
         
    //bind the socket to localhost port 8888  
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)   
    {   
        perror("bind failed");   
        exit(EXIT_FAILURE);   
    }   
    printf("Listener on port %d \n", PORT);   
         
    //try to specify maximum of 3 pending connections for the master socket  
    if (listen(master_socket, 3) < 0)   
    {   
        perror("listen");   
        exit(EXIT_FAILURE);   
    }   
         
    //accept the incoming connection  
    addrlen = sizeof(address);   
    puts("Waiting for connections from user ...");   
         
    while(TRUE)   
    {   
        timeout.tv_sec = 10;
        //clear the socket set  
        FD_ZERO(&readfds);   
        //add master socket to set  
        FD_SET(master_socket, &readfds);   
        max_sd = master_socket;   
             
        //add child sockets to set
        if(cnt!= 0 || escape != 0)
        {
            for ( i = 0 ; i < max_clients ; i++)   
            {   
                //socket descriptor  
                sd = client_socket[i];   

                //if valid socket descriptor then add to read list  
                if(sd > 0)   
                FD_SET( sd , &readfds);   
                 
                //highest file descriptor number, need it for the select function  
                if(sd > max_sd)   
                max_sd = sd;   
            } 
        }  
  
     
        //wait for an activity on one of the sockets , timeout is NULL ,  
        //so wait indefinitely  
        activity = select( max_sd + 1 , &readfds , NULL , NULL , &timeout);   //block chuong trinh tai day de doi
        if (activity == 0 && cnt==0 && escape==0)
        {
            close(master_socket);
            break;
        }     
        if ((activity < 0) && (errno!=EINTR))   
        {   
            printf("select error");   
        }   
  
       //If something happened on the master socket ,  
        //then its an incoming connection  
        if (FD_ISSET(master_socket, &readfds)) // check lai master co mat trong readfds hay khong    
        {  
            if ((new_socket = accept(master_socket,  
                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)   
            {   
                perror("accept");   
                exit(EXIT_FAILURE);   
            }     
                 
            puts("Welcome message sent successfully");   
                 
            //add new socket to array of sockets  
            for (i = 0; i < max_clients; i++)   
            {   
                //if position is empty  
                if( client_socket[i] == 0 )   
                {   
                    client_socket[i] = new_socket;   
                    pthread_create(&pt[i],NULL,My_thread,&new_socket);
                    cnt++; //increase the number of threads
                    escape = 0;
                    printf("Adding to list of sockets as %d\n" , i);   
                    break;   
                }   
            }   
        }      
    }      
    return 0;   
}

void *End_user_1 (void *arg)
{
    escape = 1;
    cnt = 0;
    int opt = TRUE;   
    int master_socket , addrlen , new_socket ,  
    max_clients = 30 , activity, i , valread , sd;   
    int max_sd;   
    pthread_t pt[30];         
    //set of socket descriptors  
    fd_set readfds;   
         
    //a message  
    char *message = "ECHO Daemon v1.0 \r\n";   
     
    //initialise all client_socket[] to 0 so not checked  
    for (i = 0; i < max_clients; i++)   
    {   
        client_socket[i] = 0;   
    }   
         
    //create a master socket  
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)   
    {   
        perror("socket failed");   
        exit(EXIT_FAILURE);   
    }   
     
    //set master socket to allow multiple connections ,  
    //this is just a good habit, it will work without this  
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,  
          sizeof(opt)) < 0 )   
    {   
        perror("setsockopt");   
        exit(EXIT_FAILURE);   
    }   
     
    //type of socket created  
    address.sin_family = AF_INET;   
    address.sin_addr.s_addr = INADDR_ANY;   
    address.sin_port = htons(PORT_2);   
         
    //bind the socket to localhost port 8888  
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)   
    {   
        perror("bind failed");   
        exit(EXIT_FAILURE);   
    }   
    printf("Listener on port %d \n", PORT_2);   
         
    //try to specify maximum of 3 pending connections for the master socket  
    if (listen(master_socket, 3) < 0)   
    {   
        perror("listen");   
        exit(EXIT_FAILURE);   
    }   
         
    //accept the incoming connection  
    addrlen = sizeof(address);   
    puts("Waiting for connections from location gernerator ...");   
         
    while(TRUE)   
    {   
        timeout.tv_sec = 10;
        //clear the socket set  
        FD_ZERO(&readfds);   
        //add master socket to set  
        FD_SET(master_socket, &readfds);   
        max_sd = master_socket;   
             
        //add child sockets to set
        if(cnt!= 0 || escape != 0)
        {
            for ( i = 0 ; i < max_clients ; i++)   
            {   
                //socket descriptor  
                sd = client_socket[i];   

                //if valid socket descriptor then add to read list  
                if(sd > 0)   
                FD_SET( sd , &readfds);   
                 
                //highest file descriptor number, need it for the select function  
                if(sd > max_sd)   
                max_sd = sd;   
            } 
        }  
  
     
        //wait for an activity on one of the sockets , timeout is NULL ,  
        //so wait indefinitely  
        activity = select( max_sd + 1 , &readfds , NULL , NULL , &timeout);   //block chuong trinh tai day de doi
        if (activity == 0 && cnt==0 && escape==0)
        {
            close(master_socket);
            break;
        }     
        if ((activity < 0) && (errno!=EINTR))   
        {   
            printf("select error");   
        }   
  
       //If something happened on the master socket ,  
        //then its an incoming connection  
        if (FD_ISSET(master_socket, &readfds)) // check lai master co mat trong readfds hay khong    
        {  
            if ((new_socket = accept(master_socket,  
                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)   
            {   
                perror("accept");   
                exit(EXIT_FAILURE);   
            }     
                 
            puts("Welcome message sent successfully");   
                 
            //add new socket to array of sockets  
            for (i = 0; i < max_clients; i++)   
            {   
                //if position is empty  
                if( client_socket[i] == 0 )   
                {   
                    client_socket[i] = new_socket;   
                    pthread_create(&pt[i],NULL,My_thread_1,&new_socket);
                    cnt++; //increase the number of threads
                    escape = 0;
                    printf("Adding to list of sockets as %d\n" , i);   
                    break;   
                }   
            }   
        }      
    }      
    return 0;   
}

                    /*Define function for class DataStruct*/

void DataStruct::SetCoordinate()
{
    longi = rand();
    lagi = rand();
}   
void DataStruct::SetID (int *arg)
{
    srand((int)time(0));
    *arg = rand();
    ID = (*arg);
}
void DataStruct::SetName(char *arg)
{
    strcpy(Name,"Huy");
}

void DataStruct:: Xuat() 
{
    cout << "Name: " << Name <<"\n";
    cout << "ID: " <<ID <<"\n";
    cout << "Longi: "<<longi <<"\n";
    cout << "Langi: "<<lagi<<"\n";
}