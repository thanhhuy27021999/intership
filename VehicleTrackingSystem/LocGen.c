#include <arpa/inet.h>
#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#define PORT 7473
int CheckStringIsNumber(char *a) {
  int c = 1;
  for (int i = 0; i < strlen(a - 1); i++) {
    if (isdigit(a[i]) == 0) {
      c = 0;
      break;
    }
  }
  return c;
}
int sensors_list[50];
struct sockaddr_in serv_addr;
pthread_t sensor[50];
int check[50];
int Sensor_id;
char buff[100];
struct SensorData {
  int SensorId, x1, x2, y1, y2;
};
FILE *XML_file;
unsigned char *SerializeInt(unsigned char *buffer, int value) {

  buffer[0] = value >> 24;
  buffer[1] = value >> 16;
  buffer[2] = value >> 8;
  buffer[3] = value;
  return buffer + 4;
}
unsigned char *SerializeCoord(unsigned char *buffer, struct SensorData value) {
  buffer = SerializeInt(buffer, value.SensorId);
  buffer = SerializeInt(buffer, value.x1);
  buffer = SerializeInt(buffer, value.x2);
  buffer = SerializeInt(buffer, value.y1);
  buffer = SerializeInt(buffer, value.y2);
  return buffer;
}
void delay(int number_of_seconds) {

  int milli_seconds = 1000 * number_of_seconds;
  clock_t start_time = clock();
  while (clock() < start_time + milli_seconds)
    ;
}
void *SendCoords(void *arg) {
  int sock = *((int *)arg);
  struct SensorData coord1;

  fprintf(XML_file, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
  coord1.SensorId = Sensor_id;

  int i = 0;
  while (1) {

    delay(6000);
    coord1.x1 = rand() % 100 + 1;
    coord1.x2 = rand() % 100 + 1;
    coord1.y1 = rand() % 100 + 1;
    coord1.y2 = rand() % 100 + 1;
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    fprintf(XML_file, "<Sensor%d>\n", coord1.SensorId);
    fprintf(XML_file, "<Location>\n");
    fprintf(XML_file, "<Time>%s</Time>\n", asctime(timeinfo));
    fprintf(XML_file, "<X1>%d</X1>\n", coord1.x1);
    fprintf(XML_file, "<X2>%d</X2>\n", coord1.x2);
    fprintf(XML_file, "<Y1>%d</Y1>\n", coord1.y1);
    fprintf(XML_file, "<Y2>%d</Y2>\n", coord1.y2);
    fprintf(XML_file, "</Location>\n");
    fprintf(XML_file, "</Sensor%d>\n", coord1.SensorId);
    unsigned char buffCoord[32], *ptr;
    ptr = SerializeCoord(buffCoord, coord1);
    send(sock, buffCoord, ptr - buffCoord, 0);
  }
}

int main(int argc, char const *argv[]) {
  XML_file = fopen("Loc_Gen_XML.xml", "w");
  int sock = 0, valread;
  for (int i = 0; i < 50; i++) {
    check[i] = 0;
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
  int n;
  int i = 0;
  for (;;) {
    bzero(buff, sizeof(buff));
    read(sock, buff, sizeof(buff));
    printf("From VTS : %s", buff);
    for (int i = 0; i < strlen(buff); i++) {
      id_check_close[i] = buff[i + 5];
    }
    if (buff[0] == 'c' && buff[1] == 'l' && buff[2] == 'o' && buff[3] == 's' &&
        buff[4] == 'e' && CheckStringIsNumber(id_check_close) == 1) {
      Sensor_id = atoi(id_check_close);
      check[Sensor_id] = 0;
    }
    for (int i = 0; i < strlen(buff); i++) {
      id_check_add[i] = buff[i + 3];
    }

    if (buff[0] == 'a' && buff[1] == 'd' && buff[2] == 'd' &&
        CheckStringIsNumber(id_check_add) == 1) {
      Sensor_id = atoi(id_check_add);
      check[Sensor_id] = 1;
    }
    if (check[Sensor_id] == 1)
      pthread_create(&sensor[Sensor_id], NULL, (void *)SendCoords, &sock);
    for (int i = 0; i < 50; i++) {
      if (check[i] == 0)
        pthread_cancel(sensor[i]);
    }
  }
  return 0;
}
