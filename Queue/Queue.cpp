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


int GetSize(Queue& q)
{
	return q.count;
}



bool IsFull(Queue& q)
{
	return (GetSize(q) == q.capacity);
}

bool IsEmpty(Queue& q)
{
	return (GetSize(q) == 0);
}

int  EnQueue(Queue& q, int x)
{
	if (IsFull(q))
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
	if (IsEmpty(q))
	{
		cout << "Queue is Empty";
		return -1;
	}
	q.front = (q.front + 1) % q.capacity;
	q.count--;
}
int Peek(Queue& q)
{
	if (IsEmpty(q))
	{
		cout << "Queue is Empty" << endl;
		return -1;
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
	if (q.capacity == 0 || q.capacity == 0 || q.count == 0 || q.count == 1)
	{
		return;
	}
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
