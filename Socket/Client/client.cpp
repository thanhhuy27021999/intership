#include <stdio.h>
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include"Student.h"
#define PORT 8080 
using namespace std;

int main()
{
    int n;
    cout <<"Please enter the number of students n = ";
    cin >> n;
    Student SV[n];
    for (int i =0; i<n; i++)
    {   
        cout <<"Student "<< i+1<< ": \n";
        SV[i].Nhap();
    }
    int sock = 0; //creat a socket
    sockaddr_in serv_addr; // creat server_addr
    Student buffer;
    memset(&buffer,0, sizeof(buffer));
    memset(&serv_addr,0,sizeof(serv_addr));
    sock = socket(AF_INET,SOCK_STREAM, 0);
    // bind the add to socket
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        cout<<"\nInvalid address/ Address not supported \n"; 
        return -1; 
    } 
    serv_addr.sin_family = AF_INET; // truong dia chi, client khoi tao dia chi cho cua server de connect toi
    serv_addr.sin_port = htons(PORT);

    // connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        cout<<"\nConnection Failed \n"; 
        return -1; 
    }
    for (int i=0; i<n; i++)
    {
        send (sock,&SV[i],sizeof(SV), 0);
        read(sock,&buffer,sizeof(buffer));
        cout<< "\nStudent from server: \n";
        buffer.Xuat();
    }
    close(sock);
    return 0;
}

