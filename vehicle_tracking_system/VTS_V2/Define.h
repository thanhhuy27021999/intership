#define     TRUE    1
#define     FALSE   0
#define     PORT    12345 // recieve demand from user
#define     PORT_1  12346 // send request to local
#define     PORT_2  12347 // recieve data from local
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
void *My_thread_1 (void *arg);
void *End_user (void *arg); //thread1
void *End_user_1(void *arg);
void *Admin_th (void *arg); //thread2
class DataStruct
{
    private:
        char Name[20];
        int ID ;
        int lagi, longi;
    public:
        void SetName(char *arg);
        void SetID (int *arg);
        void SetCoordinate ();
        void Xuat();
};