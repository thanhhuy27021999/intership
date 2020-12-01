#include <stdio.h>
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <pthread.h> 
#include <stdlib.h>
#include "tinyxml.h"
#include "unistd.h"
#include "Define_loca.h"
#include "ctime"
extern DataStruct sensor[30];
using namespace std;

                    /* Send data from sensor to VTS for user */
                    
void *Add_Sensor(void *arg)
{
    DataStruct* temp  = (DataStruct*)(arg);
    int a;
    char status_open[] = "OPEN";
    char status_exit[] = "CLOSE";
    char message[20] = "OKE";
    char buff[20];
    cout << "welcome " <<temp->Name<<"\n";
    cout <<temp->ID<<"\n";
    int sock;
    sock = ConnectToVts();
    //DataStruct DataSensor;
    while(temp->flag == 0)
    {
        strcpy(temp->status,"OPEN");
        temp->SetID(&a);
        //temp.SetName(temp.Name);
        temp->SetCoordinate();
        write(sock,temp,sizeof(DataStruct));
        read(sock,buff,sizeof(buff));
        if(strncmp(buff,"OKE",sizeof("OKE")-1)!= 0) // nhan error
        {
            temp->flag ==1;
            break;
        }
    }
    if(temp->flag == 1)
    {
        cout << temp->Name<<" exit" <<"\n";
        //temp->Name = NULL;
        temp->SetStatus(status_exit);
        temp->lagi = 0;
        temp->longi = 0;
        temp->ID = 0;
        write(sock,temp,sizeof(DataStruct));
    }
    close(sock);
}

            /*Define function for class DataStruct*/

void DataStruct::SetCoordinate()
{
    longi = rand();
    lagi = rand();
}   
void DataStruct::SetID (int *arg)
{
    ID = rand()%100+1;
}
char* DataStruct::SetName(char *arg)
{
    strcpy(Name,arg);
    return Name;
}
void DataStruct::SetStatus(char*arg)
{
    strcpy(status,arg);
}

void DataStruct::Xuat()
{
    cout << "Name: "<< Name;
    cout << "ID: "<<ID;
    cout <<"Lag: "<<lagi;
    cout << "Logi: "<< longi;
}
                    /*create a thread for sensor*/

// pthread_t newthread (pthread_t *arg1)
// {
//     pthread_create(arg1,NULL,Add_user,NULL);
//     return (*arg1);
// }



                    /*recieve request from VTS*/

void *Recv_from_ad (void *arg)
{

    for(int i =0; i<30;i++)
    {
        strcpy(sensor[i].status,"CLOSE");
        cout << sensor[i].status;
        sensor[i].flag = 0;
        cout <<"1 \n";
    }
    pthread_t sensor1_thread[30];
    pthread_t CreateXml;    
    char buffer[100];
    sockaddr_in server_addr;
    int opt =1; 
    int listenfd ; // socket for listenning
    int newsocket   ; //socket for communication between server and client
    int addr_lenght = sizeof(server_addr);
    memset(buffer,0,sizeof(buffer));
    memset(&server_addr,0,sizeof(server_addr));
    listenfd = socket(AF_INET, SOCK_STREAM, 0);// create a socket 

    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                   &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    };

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT_1);
    
    if(bind(listenfd,(sockaddr*)&server_addr, sizeof(server_addr)) <0)
    {
        close(listenfd);
        exit(2);
    } // bind IP address for server
    //NOTE: in server you must bind the Ip add for it, in client you just connect to server by server add
    listen(listenfd,3); // waiting for connection maximum 3 geue
    newsocket = accept(listenfd,(sockaddr*) &server_addr,(socklen_t*)&addr_lenght);
    cout << "da ket noi" << "\n";
    pthread_create(&CreateXml,NULL,Write_Xml,NULL);
    while(1)
    {
        read(newsocket,buffer,sizeof(buffer));
        cout <<buffer;
        if(!strncmp(buffer,"NEW",sizeof("NEW")-1))
        {
            char Name[100];
            char s[] = "=>";
            cout << ".... \n";
            strtok(buffer, s);
            cout << s<< "\n";
            char temp[20];
            strcpy(temp,strtok(NULL,s));
            strncpy(Name,temp,strnlen(temp, 20)-1);
            cout << Name <<"\n";
            for (int i = 0; i<30; i++)
            {
                cout << "---- \n";
                cout << sensor[i].status <<"\n";
                if(!strncmp(sensor[i].status,"CLOSE",sizeof("CLOSE")-1))
                {
                    cout <<i << "\n";
                    strcpy(sensor[i].Name,Name);
                    cout <<sensor[i].Name<< "\n";
                    sensor[i].flag=0;
                    pthread_create(&sensor1_thread[i],NULL,Add_Sensor,&sensor[i]);
                    break;
                }
            }
        }
        if(!strncmp(buffer,"CLOSE",sizeof("CLOSE")-1))
        {
            char Name[100];
            char s[]="=>";
            char temp[20];
            strtok(buffer, s);
            strcpy(temp,strtok(NULL,s));
            strncpy(Name,temp,strnlen(temp, 20)-1);
            for(int i = 0 ; i<30; i++)
            {
                if(!strncmp(sensor[i].Name,Name,sizeof(Name)))
                {
                    cout << sensor[i].Name<< "\n";
                    sensor[i].flag = 1;
                    cout << sensor[i].flag<<"\n";
                    break;
                }
            }
        }
        // for (int i=0; i<30;i++)
        // {
        //     if(!strncmp(buffer,sensor[i].Name, sizeof(sensor[i].Name)-1))
        //     {
        //         sensor[i].Xuat();
        //     }
        // }
        if(!strncmp(buffer,"GetSenSor",sizeof("GetSenSor")-1))
        {
            cout << "Name    ID"<<"\n";
            for (int i =0;i<30;i++)
            {
                if(!strncmp(sensor[i].status,"OPEN",sizeof("OPEN")-1))
                {
                    char temp[20];
                    //cout << strnlen(sensor[i].Name,20);
                    for (int j =0; j<(sizeof(sensor[i].Name)/sizeof(char)-1);j++)
                    {
                        temp[j] = sensor[i].Name[j];
                    }
                    cout<<temp<< "      "<< sensor[i].ID<<"\n";
                }
            }
        }
        bzero(buffer, sizeof(buffer));
        //pthread_join (sensor1_thread,NULL);
    }
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




                        /*create connection from location_generator to VTS*/

int ConnectToVts()
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
    serv_addr.sin_port = htons(PORT_2);

    // connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        cout<<"\nConnection Failed \n"; 
        return 0; 
    }
    return sock;   
}
int SplitString( char* arg, char* arg_name)
{

  
}