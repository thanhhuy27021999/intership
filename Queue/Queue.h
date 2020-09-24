#include<iostream>
using namespace std;


struct Queue
{
	int* data;
	int capacity;
	int front;
	int rear;
	int count;
};
void CreateQueue(Queue& q, int size);
int GetSize(Queue& q);
bool IsFull(Queue& q);
bool IsEmpty(Queue& q);
int EnQueue(Queue& q, int x);
int DeQueue(Queue& q);
int Peek(Queue& p);
void Print(Queue q);
void Sort(Queue& q);