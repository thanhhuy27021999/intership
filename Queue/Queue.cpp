#include"Queue.h"
using namespace std;

void CreateQueue(Queue &q,int size)
 {
    q.data = new int[size];
	q.capacity = size;
	q.front = 0;
	q.rear = -1;
	q.count = 0;
}


int size(Queue &q)
{
return q.count;
}



bool isFull(Queue &q)
{
 return (size(q) == q.capacity);
}

 bool isEmpty(Queue &q)
 {
     return (size(q) == 0);
 }

 int EnQueue(Queue &q, int x) 
 {
if (isFull(q))
	{
		cout << "Queue is Full";
		
	}
	q.rear = (q.rear + 1) % q.capacity;
	q.data[q.rear] = x;
	q.count++;
}


 int DeQueue(Queue &q, int &x) 
 {
if (isEmpty(q))
	{
		cout << "Queue is Empty";
		
	}
	q.front = (q.front + 1) % q.capacity;
	q.count--;
}
 int Peek(Queue &q)
 {
     if (isEmpty(q))
	{
		cout<<"Queue is Empty"<<endl;
	}
	return q.data[q.front];
 }
