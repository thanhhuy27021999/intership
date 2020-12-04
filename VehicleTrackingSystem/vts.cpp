#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <sys/types.h>
#include <time.h>
#include "tinyxml.h"
#include <unistd.h> //close
#include <string>
#define MAX 1024
pthread_mutex_t mutex;
int clients_list[50];
int SenSorOpen[50];
int n = 0;
char msg[50][50000];
//char tmp_msg[500];
int len;
FILE *XML_file;
FILE *wtrieLogSensor;
char CheckNameCLI[50][100];
char list_sensor_open[500];
struct SensorData
{
  char Name[100];
  int SensorId, x1, x2, y1, y2;
};
struct NameID
{
  char Name[100];
  int ID, Open,sameName;
};
struct NameID ListnameID[50];
struct SensorData messStruct;
void *conectLocGen(void *sockarg)
{
  int sockfd = *((int *)sockarg);
  char buff[100];
  char buffTmp[100];
  char idCheckOpen[10];
  char idCheckClose[10];
  int n;
  for (int i = 0; i < 50; i++)
  {
    SenSorOpen[i] = 0;
  }
  // wtrieLogSensor = fopen("WriteLog.txt", "w");
  TiXmlDocument doc;
  TiXmlDeclaration *dec = new TiXmlDeclaration("1.0", "utf-8", "");
  doc.LinkEndChild(dec);
  TiXmlElement *root = new TiXmlElement("WriteLog");
  doc.LinkEndChild(root);
  for (;;)
  {
    bzero(buff, 100);
    n = 0;
    while ((buff[n++] = getchar()) != '\n')
      ;
    strcpy(buffTmp,buff);
    for (int i = 0; i < strlen(buff); i++)
    {
      idCheckOpen[i] = buff[i + 4];
    }
    if (buff[0] == 'a' && buff[1] == 'd' && buff[2] == 'd' && buff[3] == ':')
    {
      for (int i = 0; i < 50; i++)
      {
        if (strcmp("", ListnameID[i].Name) == 0)
        {
          strcpy(ListnameID[i].Name, idCheckOpen);
          ListnameID[i].Open = 1;
         // ListnameID[i].sameName=1;
          break;
        }
      }

      // SenSorOpen[atoi(idCheckOpen)]=1;
      time_t rawtime;
      struct tm *timeinfo;
      time(&rawtime);
      timeinfo = localtime(&rawtime);
      TiXmlElement *Sensor = new TiXmlElement("Sensor");
      Sensor->SetAttribute("Name", idCheckOpen);
      root->LinkEndChild(Sensor);
      TiXmlElement *time_xml = new TiXmlElement("TimeOpen");
      Sensor->LinkEndChild(time_xml);
      TiXmlText *time_xml_content = new TiXmlText(asctime(timeinfo));
      time_xml->LinkEndChild(time_xml_content);
      doc.SaveFile("WriteLog.xml");
    }
    for (int i = 0; i < strlen(buff); i++)
    {
      idCheckClose[i] = buff[i + 5];
    }
    if (buff[0] == 'c' && buff[1] == 'l' && buff[2] == 'o' && buff[3] == 's' &&
        buff[4] == 'e' && buff[5] == ':')
    {
     // SenSorOpen[atoi(idCheckClose)] = 0;
     for (int i = 0; i < 50; i++)
      {
        if (strcmp(idCheckClose, ListnameID[i].Name) == 0)
        {
          strcpy(ListnameID[i].Name, "");
          ListnameID[i].Open = 0;
          break;
        }
      }
      time_t rawtime;
      struct tm *timeinfo;
      time(&rawtime);
      timeinfo = localtime(&rawtime);
      TiXmlElement *Sensor = new TiXmlElement("Sensor");
      Sensor->SetAttribute("Name", idCheckClose);
      root->LinkEndChild(Sensor);
      TiXmlElement *time_xml = new TiXmlElement("TimeClose");
      Sensor->LinkEndChild(time_xml);
      TiXmlText *time_xml_content = new TiXmlText(asctime(timeinfo));
      time_xml->LinkEndChild(time_xml_content);
      doc.SaveFile("WriteLog.xml");
    }
    strcpy(buff,buffTmp);
    write(sockfd, buff, sizeof(buff));
    if (strncmp("exit", buff, 4) == 0)
    {
      printf("Exit...\n");
      break;
    }
  }
  for(int i=0;i<5;i++)
 {
   printf("name %s ID %d\n",ListnameID[i].Name,ListnameID[i].ID);
 }
}

// Recv coords from each Sensor
void *RecvMess(void *server_sock)
{
  int sock = *((int *)server_sock);
  int i = 0;
  char buf[500];
  TiXmlDocument doc;
  TiXmlDeclaration *dec = new TiXmlDeclaration("1.0", "utf-8", "");
  doc.LinkEndChild(dec);
  TiXmlElement *root = new TiXmlElement("DataXml");
  doc.LinkEndChild(root);
  char SSname[100];
  while ((len = recv(sock, &messStruct, sizeof(messStruct), 0)) > 0)
  {
    int SensorId, x1, x2, y1, y2;

    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    SensorId = messStruct.SensorId;
    strcpy(SSname, messStruct.Name);
    x1 = messStruct.x1;
    x2 = messStruct.x2;
    y1 = messStruct.y1;
    y2 = messStruct.y2;
    for (int i = 0; i < 50; i++)
    {
      if (strcmp(SSname, ListnameID[i].Name) == 0)
      {
        ListnameID[i].ID = SensorId;
      }
    }
    char id_tmp[10], X1[10], X2[10], Y1[10], Y2[10];
    TiXmlElement *Sensor = new TiXmlElement("Sensor");
    Sensor->SetAttribute("Name", SSname);
    root->LinkEndChild(Sensor);

    TiXmlElement *Location = new TiXmlElement("Location");
    Sensor->LinkEndChild(Location);
    //Time
    TiXmlElement *time_xml = new TiXmlElement("Time");
    Location->LinkEndChild(time_xml);
    TiXmlText *time_xml_content = new TiXmlText(asctime(timeinfo));
    time_xml->LinkEndChild(time_xml_content);

    //x1
    TiXmlElement *x1_xml = new TiXmlElement("X1");
    Location->LinkEndChild(x1_xml);
    snprintf(buf, sizeof(buf), "%d", x1);
    TiXmlText *x1_xml_content = new TiXmlText(buf);
    x1_xml->LinkEndChild(x1_xml_content);
    //x2
    TiXmlElement *x2_xml = new TiXmlElement("X2");
    Location->LinkEndChild(x2_xml);
    snprintf(buf, sizeof(buf), "%d", x2);
    TiXmlText *x2_xml_content = new TiXmlText(buf);
    x2_xml->LinkEndChild(x2_xml_content);
    //y1
    TiXmlElement *y1_xml = new TiXmlElement("Y1");
    Location->LinkEndChild(y1_xml);
    snprintf(buf, sizeof(buf), "%d", y1);
    TiXmlText *y1_xml_content = new TiXmlText(buf);
    y1_xml->LinkEndChild(y1_xml_content);
    //y2
    TiXmlElement *y2_xml = new TiXmlElement("Y2");
    Location->LinkEndChild(y2_xml);
    snprintf(buf, sizeof(buf), "%d", y2);
    TiXmlText *y2_xml_content = new TiXmlText(buf);
    y2_xml->LinkEndChild(y2_xml_content);
    doc.SaveFile("VTS_XML.xml");
    //////
    sprintf(id_tmp, "%d", SensorId);
    sprintf(X1, "%d", x1);
    sprintf(X2, "%d", x2);
    sprintf(Y1, "%d", y1);
    sprintf(Y2, "%d", y2);
    strcat(msg[SensorId], "\nSensor ");
    strcat(msg[SensorId], SSname);
    strcat(msg[SensorId], "(id: ");
    strcat(msg[SensorId], id_tmp);
    strcat(msg[SensorId], "X1: ");
    strcat(msg[SensorId], X1);
    strcat(msg[SensorId], " X2: ");
    strcat(msg[SensorId], X2);
    strcat(msg[SensorId], " Y1: ");
    strcat(msg[SensorId], Y1);
    strcat(msg[SensorId], " Y2 :");
    strcat(msg[SensorId], Y2);
    strcat(msg[SensorId], ")\n");

    printf("Sensor %s  :", SSname);
    printf("(id:%d ,", SensorId);
    printf("X1:%d ,", x1);
    printf(" X2:%d ,", x2);
    printf(" Y1:%d ,", y1);
    printf(" Y2:%d)\n", y2);
  }
}
////// Connect to CLI user

void *SendSensorData(void *arg)
{
  int serverSocket = *((int *)arg);
  char buffC[MAX];
  char buffS[MAX];
  int n;
  int SensorIdMsg;
  char id_check[100];
  for (;;)
  {
    bzero(buffC, sizeof(buffC));
    read(serverSocket, buffC, sizeof(buffC));
    // if ((strncmp(buffC, "", 7)) == 0)
    for (int i = 0; i < strlen(buffC); i++)
    {
      id_check[i] = buffC[i + 4];
    }
    if (buffC[0] == 'g' && buffC[1] == 'e' && buffC[2] == 't' && buffC[3] == ':')
    {
      for (int i = 0; i < 50; i++)
      {
        if (strcmp(id_check, ListnameID[i].Name) == 0)
        {
          send(serverSocket, msg[ListnameID[i].ID], sizeof(msg[ListnameID[i].ID]), 0);
          break;
        }
      }
    }
    int checkName=0;
    if (buffC[0] == 'l' && buffC[1] == 'i' && buffC[2] == 's' && buffC[3] == 't')
    {

      printf("Send list Sensor to CLI User\n");
      strcpy(list_sensor_open, "");
      char tmp[5];
      /*
      for (int i = 0; i < 50; i++)
      {
        if (ListnameID[i].Open == 1)
        {
         // ListnameID[i].sameName=1;
          for(int j=0;j<50;j++)
          {
            if(strcmp(CheckNameCLI[j],ListnameID[i].Name)==0)
            {
              checkName=1;
              break;
            }
          }
          if(checkName==0)
          {
          sprintf(tmp, "%d", ListnameID[i].ID);
          strcat(list_sensor_open, "Sensor: ");
          strcat(list_sensor_open, ListnameID[i].Name);
          strcpy(CheckNameCLI[i], ListnameID[i].Name);
          }
         
       
        }
      }
      */
     for (int i = 0; i < 50; i++)
      {
        if (ListnameID[i].Open == 1)
        {
         sprintf(tmp, "%d", ListnameID[i].ID);
          strcat(list_sensor_open, "Sensor: ");
          strcat(list_sensor_open, ListnameID[i].Name);
          strcpy(CheckNameCLI[i], ListnameID[i].Name);
          
        }
      }
      strcat(list_sensor_open, "\n");
      int listEmpty = 0;
      for (int i = 0; i < 50; i++)
      {
        if (ListnameID[i].Open == 1)
          listEmpty = 1;
      }
      if (listEmpty == 0)
      {
        strcpy(list_sensor_open, "List is empty\n");
      }
      send(serverSocket, list_sensor_open, sizeof(list_sensor_open), 0);
    }
  }
}
/*
    if (buffC[0] == 'g' && buffC[1] == 'e' && buffC[2] == 't' &&
       buffC[3] == ':') {
         for(int i=0;i<50;i++)
         {
           if(strcmp(id_check,ListnameID[i].Name)==0)
           {
             send(serverSocket, msg[ListnameID[i].ID], sizeof(msg[ListnameID[i].ID]), 0);
             break;
           }
         }
     // SensorIdMsg = atoi(id_check);
     // send(serverSocket, msg[SensorIdMsg], sizeof(msg[SensorIdMsg]), 0);
    }
  }
  */


void *ConnectCLI(void *arg)
{
  int master_socket1, addrlen1, new_socket1, client_socket1[50];
  int max_clients1 = 50, valread1, sd1;
  int max_sd1;
  struct sockaddr_in address1;
  struct timeval time_out;
  time_out.tv_usec = 0;
  char buffer1[1025];

  fd_set readfds;

  for (int i = 0; i < max_clients1; i++)
  {
    client_socket1[i] = 0;
  }

  if ((master_socket1 = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  address1.sin_family = AF_INET;
  address1.sin_addr.s_addr = INADDR_ANY;
  address1.sin_port = htons(8888);

  if (bind(master_socket1, (struct sockaddr *)&address1, sizeof(address1)) <
      0)
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(master_socket1, 3) < 0)
  {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  addrlen1 = sizeof(address1);
  pthread_t tid[3];
  int i = 0;
  while (1)
  {
    new_socket1 = accept(master_socket1, (struct sockaddr *)&address1,
                         (socklen_t *)&addrlen1);
    pthread_create(&tid[i++], NULL, SendSensorData, &new_socket1);
    if (i >= 3)
    {
      i = 0;
      while (i < 3)
      {
        pthread_join(tid[i++], NULL);
      }
      i = 0;
    }
  }
}

///////////////////////////

int main()
{

  for (int i = 0; i < 50; i++)
  {
    strcpy(ListnameID[i].Name, "");
    ListnameID[i].Open = 0;
    ListnameID[i].sameName=0;
    strcpy(CheckNameCLI[i],"");
  }
  pthread_t recvt, connecLoc, connecCLIUser;
  pthread_create(&connecCLIUser, NULL, &ConnectCLI, NULL);
  struct sockaddr_in address;

  int server_sock = 0;
  int max_sd, activity, new_socket;
  fd_set readfds;
  address.sin_family = AF_INET;
  address.sin_port = htons(7473);
  address.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_sock = socket(AF_INET, SOCK_STREAM, 0);

  bind(server_sock, (struct sockaddr *)&address, sizeof(address));
  if (listen(server_sock, 20) == -1)
    printf("listening failed n");

  while (1)
  {

    FD_ZERO(&readfds);

    FD_SET(server_sock, &readfds);
    max_sd = server_sock;
    activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
    // server khong nhan duoc request tu client thi readfds khong con chua
    // master_socket nua

    if (FD_ISSET(server_sock, &readfds))
    {
      if ((new_socket = accept(server_sock, (struct sockaddr *)NULL, NULL)) < 0)
        printf("accept failed n");

      //   pthread_create(&connecLoc, NULL, (void *)conectLocGen, &new_socket);
      pthread_create(&connecLoc, NULL, conectLocGen, &new_socket);
      pthread_mutex_lock(&mutex);
      clients_list[n] = new_socket;
      n++;

      // pthread_create(&recvt, NULL, (void *)RecvMess, &new_socket);
      pthread_create(&recvt, NULL, RecvMess, &new_socket);
      pthread_mutex_unlock(&mutex);
    }
  }
 
  return 0;
}
