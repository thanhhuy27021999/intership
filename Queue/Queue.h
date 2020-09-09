#include<iostream>
using namespace std;

//#define MAX 100
struct Queue 
{
int *data;   	
	int capacity;  
	int front;  	
	int rear;   	
	int count;  	
};
void CreateQueue(Queue &q,int size);
int size(Queue &q);
bool isFull(Queue &q);
bool isEmpty(Queue &q);
int EnQueue(Queue &q, int x) ;
int DeQueue(Queue &q, int &x) ;
int Peek(Queue &p);