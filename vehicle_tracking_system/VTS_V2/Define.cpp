#include <stdio.h>
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <pthread.h> 
#include "Define.h"
#include "ctime"
#include "tinyxml.h"
int cnt_Client;
int escape_client;
int client_socket_client[30];
int cnt;
int escape;
int client_socket[30];
struct sockaddr_in address;   
struct timeval timeout;
DataStruct sensor[30];
using namespace std;

void *Admin_th (void *arg)
{
    int sock = 0; //creat a socket
    sockaddr_in serv_addr; // creat server_addr
    memset(&serv_addr,0,sizeof(serv_addr));
    sock = socket(AF_INET,SOCK_STREAM, 0);
    // bind the add to socket
    if(inet_pton(AF_INET, "192.168.122.1", &serv_addr.sin_addr) <= 0)  
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
        if(!strncmp(buff,"GetSenSor",sizeof("GetSenSor")-1))
        {
            cout << "Name    ID"<<"\n";
            for (int i =0;i<30;i++)
            {
                if(!strncmp(sensor[i].status,"OPEN",sizeof("OPEN")-1))
                {
                    cout <<sensor[i].Name <<"     "<<sensor[i].ID<< "\n";
                }
            }
        }

        if(!strncmp(buff,"take",sizeof("take")-1))
        {
            char Name[100];
            char s[] = "=>";
            cout << ".... \n";
            strtok(buff, s);
            cout << s<< "\n";
            char temp[20];
            strcpy(temp,strtok(NULL,s));
            strncpy(Name,temp,strnlen(temp, 20)-1);
            for(int i = 0; i<30; i++)
            {
                if(!strncmp(sensor[i].Name,Name, sizeof(Name)))
                {
                    sensor[i].Xuat();
                }
            }
        }
	} 
} 

void *My_thread_1 (void *arg)
{
    int newarg = *(int*)(arg);
    struct sockaddr_in temp;
    temp = address;
    DataStruct buff; 
    char *message  = "OKE";
    //char buff_T[MAX];
	int n; 
    bzero(&buff, sizeof(buff)); 
    read(newarg, &buff, sizeof(buff)); 
    for (int i=0; i<30;i++)
    {
        if (!strncmp(sensor[i].status,"CLOSE",sizeof("CLOSE")-1))
        {
            buff.Xuat();
            sensor[i]=buff;
            break;
        }
    }     
    write(newarg,message,sizeof(message));


	for (;;) 
    { 
		bzero(&buff, sizeof(buff)); 
        read(newarg, &buff, sizeof(buff)); 
        for (int i=0; i<30;i++)
        {
            if (!strncmp(sensor[i].Name,buff.Name,sizeof(buff.Name)-1))
            {
                sensor[i]=buff;
                if(!strncmp(buff.status, "CLOSE",sizeof("CLOSE")-1))
                {
                    cout <<"Da exit....  "<<buff.Name<<"\n";
                    cnt--;
                    cout <<cnt<<"\n";
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
                    bzero(&sensor[i],sizeof(DataStruct));
                    close(newarg); 
                    return 0;
                }
                break;
            }
        }
        write(newarg,message,sizeof(message));
		bzero(&buff, sizeof(buff)); 
	}  
}


void *My_thread (void *arg)
{
    int newarg = *(int*)(arg);
    struct sockaddr_in temp;
    temp = address;
    char buff[MAX]; 
    DataStruct buff_T;
	int n; 
	for (;;) 
    { 
		bzero(buff, sizeof(buff)); 
        read(newarg, buff, sizeof(buff)); 
        cout << buff<<"\n";
        if(!strncmp(buff,"GetSenSor",sizeof("GetSenSor")-1))
        {
            cout <<buff<<"\n";
            for (int i=0;i<30;i++)
            {
                if(!strncmp(sensor[i].status,"OPEN",sizeof("OPEN")-1))
                {
                    write(newarg,&sensor[i],sizeof(DataStruct));
                }
            }
        }
        if(!strncmp(buff,"take",sizeof("take")-1))
        {
            char Name[100];
            char s[] = "=>";
            cout << ".... \n";
            strtok(buff, s);
            cout << s<< "\n";
            char temp[20];
            strcpy(temp,strtok(NULL,s));
            strncpy(Name,temp,strnlen(temp, 20)-1);
            for(int i = 0; i<30; i++)
            {
                if(!strncmp(sensor[i].Name,Name, sizeof(Name)))
                {
                    write(newarg,&sensor[i],sizeof(DataStruct));
                }
            }
        }
        if (!strncmp(buff, "exit", sizeof("exit")-1))
        { 
			printf("Client Exit...\n");
            cnt_Client--;
            for(int i=0; i<30;i++)
            {
                if(client_socket_client[i]==newarg)
                {
                    for(int j=i;j<30;j++)
                    {
                        int a;
                        a= j+1;
                        client_socket_client[j] = client_socket_client[a];
                    }
                    break;
                }
            }
            write(newarg,"exit",sizeof("exit"));
            close(newarg); 
			break; 
		}
	}  
}


void *End_user (void *arg)
{
    escape_client = 1;
    cnt_Client = 0;
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
        client_socket_client[i] = 0;   
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
    cout << "Waiting for connections from user ... \n";   
         
    while(TRUE)   
    {   
        timeout.tv_sec = 10;
        //clear the socket set  
        FD_ZERO(&readfds);   
        //add master socket to set  
        FD_SET(master_socket, &readfds);   
        max_sd = master_socket;    
        for ( i = 0 ; i < max_clients ; i++)   
        {   
            //socket descriptor  
            sd = client_socket_client[i];   

            //if valid socket descriptor then add to read list  
            if(sd > 0)   
            FD_SET( sd , &readfds);   
                
            //highest file descriptor number, need it for the select function  
            if(sd > max_sd)   
            max_sd = sd;   
        } 
        //wait for an activity on one of the sockets , timeout is NULL ,  
        //so wait indefinitely  
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);   //block chuong trinh tai day de doi
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
                 
            cout << "Welcome message sent successfully \n";   
                 
            //add new socket to array of sockets  
            for (i = 0; i < max_clients; i++)   
            {   
                //if position is empty  
                if( client_socket_client[i] == 0 )   
                {   
                    client_socket_client[i] = new_socket;   
                    pthread_create(&pt[i],NULL,My_thread,&new_socket);
                    cnt_Client++; //increase the number of threads
                    escape_client = 0;
                    printf("Adding user to list of sockets as %d\n" , i);   
                    break;   
                }   
            }   
        }      
    }      
    return 0;   
}

void *End_user_1 (void *arg)
{
    for(int i =0; i<30;i++)
    {
        strcpy(sensor[i].status,"CLOSE");
    }
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
    //char *message = "ECHO Daemon v1.0 \r\n";   
     
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
    cout << "Waiting for connections from location gernerator ... \n ";   
         
    while(TRUE)   
    {   
        //clear the socket set  
        FD_ZERO(&readfds);   
        //add master socket to set  
        FD_SET(master_socket, &readfds);   
        max_sd = master_socket;   
             
        //add child sockets to set
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
        //wait for an activity on one of the sockets , timeout is NULL ,  
        //so wait indefinitely  
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);   //block chuong trinh tai day de doi    
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
            cout << "Welcome message sent successfully \n";         
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
                    printf("Adding Sensor to list of sockets as %d\n" , i);   
                    break;   
                }   
            }   
        }      
    }      
    return 0;   
}

void DataStruct:: Xuat() 
{
    cout << "Name: " << Name <<"\n";
    cout <<"Status: " <<status <<"\n";
    cout << "ID: " <<ID <<"\n";
    cout << "Longi: "<<longi <<"\n";
    cout << "Langi: "<<lagi<<"\n";
}
char* DataStruct ::GetStatus()
{
    return(status);
}
void *Write_Xml(void* arg)
{
    //Tạo đối tượng quản lý tài liệu XML
	TiXmlDocument doc;

	//Tạo chỉ thị của tài liệu XML bao gồm version, endcoding sau đó thêm dec vào tài liệu
	TiXmlDeclaration *dec = new TiXmlDeclaration("1.0", "utf-8", "");
	//Thêm dec vào tài liệu
	doc.LinkEndChild(dec);

	//Tạo comment và thêm comment vào tài liệu
	TiXmlComment *cmt = new TiXmlComment("Demo read, write, edit XML document using TinyXML library");
	doc.LinkEndChild(cmt);

	//Tạo node root và thêm root vào tài liệu
	TiXmlElement* root = new TiXmlElement("Authors");
	doc.LinkEndChild(root);
    while(1)
    {
        time_t now = time(0);
        char* Time = ctime(&now);
        char buffer[20];
        strncpy(buffer,Time,(strnlen(Time,50)-1));
        for(int i=0; i<30;i++)
        {
            if(!strncmp(sensor[i].status,"OPEN",sizeof("OPEN")-1))
            {
                //Tạo Author1
                TiXmlElement* author = new TiXmlElement(sensor[i].Name);
                //Set id cho author1
                author->SetAttribute("Id", sensor[i].ID);
                //Thêm author1 vào root
                root->LinkEndChild(author);
                //Tạo node Name 
                TiXmlElement* author_name = new TiXmlElement("Name");
                author->LinkEndChild(author_name);
                TiXmlText* name_content = new TiXmlText(sensor[i].Name);
                author_name->LinkEndChild(name_content);

                TiXmlElement* author_Status = new TiXmlElement("Status");
                author->LinkEndChild(author_Status);
                TiXmlText* Status_content = new TiXmlText(sensor[i].status);
                author_Status->LinkEndChild(Status_content);

                TiXmlElement* author_Date_Time = new TiXmlElement("DateandTime");
                author->LinkEndChild(author_Date_Time);
                TiXmlText* Date_Time_content = new TiXmlText(buffer);
                author_Date_Time->LinkEndChild(Date_Time_content);

                TiXmlElement* author_lag = new TiXmlElement("Lag");
                author_lag->SetAttribute("Lag",sensor[i].lagi);
                author->LinkEndChild(author_lag);
                TiXmlElement* author_log = new TiXmlElement("Log");
                author_log->SetAttribute("Log",sensor[i].longi);
                author->LinkEndChild(author_log); 
            }
               
        }
        doc.SaveFile("Authors_Write.xml");
        sleep(5);
    }
   
	return 0;    
}