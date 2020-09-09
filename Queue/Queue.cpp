#include"Queue.h"
using namespace std;

void CreateQueue(Queue& q, int size)
{
	q.data = new int[size];
	q.capacity = size;
	q.front = 0;
	q.rear = -1;
	q.count = 0;
}


int size(Queue& q)
{
	return q.count;
}



bool isFull(Queue& q)
{
	return (size(q) == q.capacity);
}

bool isEmpty(Queue& q)
{
	return (size(q) == 0);
}

int  EnQueue(Queue& q, int x)
{
	if (isFull(q))
	{
		cout << "Queue is Full";
		return 0;
	}
	q.rear = (q.rear + 1) % q.capacity;
	q.data[q.rear] = x;
	q.count++;
}


int DeQueue(Queue& q)
{
	if (isEmpty(q))
	{
		cout << "Queue is Empty";
		return 0;
	}
	q.front = (q.front + 1) % q.capacity;
	q.count--;
}
int Peek(Queue& q)
{
	if (isEmpty(q))
	{
		cout << "Queue is Empty" << endl;
	}
	else
	{
		return q.data[q.front];
	}
	
}
void Print(Queue q)
{
	for (int i = 0; i < q.count; i++)
	{
		int index = (q.front + i) % q.capacity;
		cout << q.data[index] << endl;
	}

}
//selection sort
void Sort(Queue& q)
{
	int i, j, min_idx;
	for (i = q.front; i < q.rear - 1; i++)
	{
		min_idx = i;
		for (j = i + 1; j < q.rear; j++)
		{
			if (q.data[min_idx] > q.data[j])
				min_idx = j;
		}
		swap(q.data[i], q.data[min_idx]);
	}

}
