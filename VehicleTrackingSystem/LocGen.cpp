#include <arpa/inet.h>
#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include "tinyxml.h"
#define PORT 7473

int sensors_list[50];
int same_senser_list[50];
struct sockaddr_in serv_addr;
pthread_t sensor[50];
int check[50];
int Sensor_id;
char buff[1000];
struct NameID
{
    char Name[100];
    int  id;
    int Open,sameName;
};
struct NameID Listname[50];
struct SensorData {
  char Name[100];
  int SensorId, x1, x2, y1, y2;
};
FILE *XML_file;

void delay(int number_of_seconds) {

  int milli_seconds = 1000 * number_of_seconds;
  clock_t start_time = clock();
  while (clock() < start_time + milli_seconds)
    ;
}


void *SendCoords(void *arg) {
  int sock = *((int *)arg);
  struct SensorData coord1;
   char buf[500];
 TiXmlDocument doc;
 TiXmlDeclaration *dec=new TiXmlDeclaration("1.0","utf-8","");
 doc.LinkEndChild(dec);
 TiXmlElement* root= new TiXmlElement("LocGenXML");
 doc.LinkEndChild(root);
  coord1.SensorId = Sensor_id;
  strcpy(coord1.Name,Listname[Sensor_id].Name);
  int i = 0;
  while (1) {

    delay(10000);
   
    coord1.x1 = rand() % 100 + 1;
    coord1.x2 = rand() % 100 + 1;
    coord1.y1 = rand() % 100 + 1;
    coord1.y2 = rand() % 100 + 1;
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    TiXmlElement* Sensor = new TiXmlElement("Sensor");
    Sensor->SetAttribute("Name",coord1.Name );
    root->LinkEndChild(Sensor);

    TiXmlElement* Location = new TiXmlElement("Location");
    Sensor->LinkEndChild(Location);
    //Time
    TiXmlElement* time_xml = new TiXmlElement("Time");
    Location->LinkEndChild(time_xml);
    TiXmlText* time_xml_content = new TiXmlText(asctime(timeinfo));
    time_xml->LinkEndChild(time_xml_content);

    //x1
    TiXmlElement* x1_xml = new TiXmlElement("X1");
    Location->LinkEndChild(x1_xml);
    snprintf(buf, sizeof(buf), "%d",coord1.x1 );
    TiXmlText* x1_xml_content = new TiXmlText(buf);
    x1_xml->LinkEndChild(x1_xml_content);
    //x2
    TiXmlElement* x2_xml = new TiXmlElement("X2");
    Location->LinkEndChild(x2_xml);
    snprintf(buf, sizeof(buf), "%d", coord1.x2);
    TiXmlText* x2_xml_content = new TiXmlText(buf);
    x2_xml->LinkEndChild(x2_xml_content);
    //y1
    TiXmlElement* y1_xml = new TiXmlElement("Y1");
    Location->LinkEndChild(y1_xml);
    snprintf(buf, sizeof(buf), "%d", coord1.y1);
    TiXmlText* y1_xml_content = new TiXmlText(buf);
    y1_xml->LinkEndChild(y1_xml_content);
    //y2
     TiXmlElement* y2_xml = new TiXmlElement("Y2");
    Location->LinkEndChild(y2_xml);
    snprintf(buf, sizeof(buf), "%d", coord1.y2);
    TiXmlText* y2_xml_content = new TiXmlText(buf);
    y2_xml->LinkEndChild(y2_xml_content);
     doc.SaveFile("LocGenXml.xml");
  //  unsigned char buffCoord[32], *ptr;
  //  ptr = SerializeCoord(buffCoord, coord1);
  //  send(sock, buffCoord, ptr - buffCoord, 0);
   // char * namestring="TranNgocSon";
   // send(sock, namestring, sizeof(namestring), 0);
    send(sock,&coord1 ,sizeof(coord1) , 0);
  }
}

int main(int argc, char const *argv[]) {
//  XML_file = fopen("Loc_Gen_XML.xml", "w");
  int sock = 0, valread;
  for (int i = 0; i < 50; i++) {
    check[i] = 0;
    same_senser_list[i]=0;
   strcpy(Listname[i].Name,""); 
    Listname[i].Open=0;
  }

  char buffer[1024] = {0};
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("\nConnection Failed \n");
    return -1;
  }
  char id_check_add[100];
  char id_check_close[100];
  char SSNameOpen[100];
  char SSNameClose[100];
  int n;
  int i = 0;
  int checkName=0;
  for (;;) {
    

    bzero(buff, sizeof(buff));
    read(sock, buff, sizeof(buff));
    printf("From VTS : %s", buff);
    
    // if (buff[0] == 'c' && buff[1] == 'l' && buff[2] == 'o' && buff[3] == 's' &&
    //     buff[4] == 'e' && CheckStringIsNumber(id_check_close) == 1) 
    if (buff[0] == 'c' && buff[1] == 'l' && buff[2] == 'o' && buff[3] == 's' &&
        buff[4] == 'e' &&  buff[5] == ':') 
    {
      for (int i = 0; i < strlen(buff); i++) 
      {
        SSNameClose[i] = buff[i + 6];
      }
     // printf("closename: %s ",SSNameClose);
      for(int i=0;i<50;i++)
      {
        if(strcmp(SSNameClose,Listname[i].Name)==0)
        {
        //   printf("ListName: %s ",Listname[i].Name);
          check[i]=0;
          strcpy(Listname[i].Name,"");
           Listname[i].Open=0;
          break;
        }
      }
     // Sensor_id = atoi(id_check_close);
     // check[Sensor_id] = 0;
     // same_senser_list[Sensor_id]=0;
    }
    for (int i = 0; i < strlen(buff); i++) {
      id_check_add[i] = buff[i + 3];
    }
   
   
    // if (buff[0] == 'a' && buff[1] == 'd' && buff[2] == 'd' &&
    //     CheckStringIsNumber(id_check_add) == 1) 
    if (buff[0] == 'a' && buff[1] == 'd' && buff[2] == 'd' &&buff[3]==':') 
   {
     int index;
     for (int i = 0; i < strlen(buff); i++)
   {
      SSNameOpen[i] = buff[i + 4];
    }
    for(int i=0;i<50;i++)
    {
      if(check[i]==0)
      {
        for(int j=0;j<50;j++)
        {
          if(strcmp(SSNameOpen,Listname[j].Name)==0)
          {
            checkName=1;
            break;
          }
        }
        if(checkName==0)
        {
          strcpy(Listname[i].Name,SSNameOpen);
        index=i;
        break;
        }
       
      }
    }
      Sensor_id = index;
      check[Sensor_id] = 1;
       
    }
    
   
    if (check[Sensor_id] == 1 &&Listname[Sensor_id].Open==0)
    {
      Listname[Sensor_id].Open=1;
      pthread_create(&sensor[Sensor_id], NULL, SendCoords, &sock);
    }
     
    for (int i = 0; i < 50; i++) {
      if (check[i] == 0)
        pthread_cancel(sensor[i]);
    }
  }
  return 0;
}
