#define     TRUE    1
#define     FALSE   0
#define     PORT    8888
#define     MAX     100
#include <sys/socket.h> 
#include <arpa/inet.h> 
extern int  client_socket[30];
extern struct sockaddr_in address;   
extern struct timeval timeout;
extern int cnt;
extern int escape;
void func(int sockfd);
void *My_thread(void *arg); 
void *End_user (void *arg); //thread1
void *Admin_th (void *arg); //thread2