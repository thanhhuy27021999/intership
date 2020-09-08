#include <iostream>
#include "linked_list.h";
using namespace std;
//creat a Node
Node *CreatNode(Data data_in)
{
    Node *temp = new Node;
    temp->data = data_in;
    temp->pnext = NULL;
    return temp;
};

void Init (List& lst)
{
    lst.phead = lst.pTail = NULL;
};
bool IsEmpty(List& lst)
{
    if (lst.phead == NULL)
    return true ;
    else return false ;
};
void AddHead (List& lst, Node* node)
{
    if (IsEmpty(lst)==true)
    {
        lst.phead = node;
        lst.pTail = node;
    }
    else 
    {
        node->pnext = lst.phead;
        lst.phead = node;
    }
};
void AddTail(List& lst, Node* node)
{
    if (IsEmpty(lst)==true)
    {
        lst.phead = node;
        lst.pTail = node;
    }
    else
    {
        lst.pTail->pnext = node;
        lst.pTail = node;
    }
};

void InsertAfterIndex(List& lst, Node* p, Node* q)
{
	if (q != NULL)
	{
		p->pnext = q->pnext;
		q->pnext = p;
		if (lst.pTail == q)
        {
			lst.pTail = p;
        }
	}
	else
		AddHead(lst, p);
};
void RemoveHead(List& lst, int&x)
{
    if(lst.phead!=NULL)
    {
        Node* node = lst.phead;
        x = node->data.a;
        lst.phead = node->pnext;
        delete node;
    }
};

Node* Getnode(List& lst, int index)
{
    Node* node = lst.phead;
    int i= 0;
    while(node!=NULL && i!=index)
    {
        i++;
        node = node->pnext;
    }
    if(i!=index)
    {
        cout <<"gia tri index khong phu hop" <<"\n";
    }
    else
    {
        return node;
    }
}

void PrintList(List& lst)
{
    if(lst.phead!=NULL)
    {
        Node* node = lst.phead;
        while (node!=NULL)
        {
            cout << node->data.a <<"  ";
            node = node->pnext;
        }
    }
}
Node* Search(List& lst, int x)
{
	Node* node = lst.phead;
	while (node != NULL && node->data.a != x)
		node = node->pnext;
	if (node != NULL)
		return node;
	return nullptr;
}
 
void RemoveAfterIndex(List& list, Node* q, int& x)
{
    if(q!=NULL)
    {
        Node* temp;
        if(q->pnext!=NULL)//the last member.
        {
            temp = q->pnext;
            q->pnext =  temp->pnext;
            x = temp->data.a;
            delete temp;
        }
        else
        {
            cout <<"day la gia tri cuoi cung cua list" <<"\n";
        }
        
    }
    else
    {
        cout << "kiem tra lai chuoi" <<"\n";
    }
};

int GetSize(List& lst)
{
    Node* temp; 
    int i = 1;
    temp = lst.phead;
    while (temp->pnext!= NULL)
    {
        i++;
        temp = temp->pnext;
    }
    return i;
}

List SortListAsending(List lst)
{
    int k = GetSize(lst);
    if (k==0)
    {
        cout << "chieu dai cua chuoi khong hop le" << "\n";
    }
    else
    {
        Data a[k];
    List ListOut;
    int temp = 0;
    Node* node = lst.phead;
    for(int i=0; i<k; i++)
    {
        a[i] = node->data;
        node = node->pnext;
    }

    for(int i = 0; i<k; i++)
    {
        for(int j=0; j<k; j++)
        {
            if(a[i].a > a[j].a)
            {
                 temp = a[j].a;
                 a[j].a = a[i].a;
                 a[i].a = temp;
            }
        }
    }

    for(int i = 0; i<k; i++)
    {
        node = CreatNode(a[i]);
        AddTail(ListOut,node);
    }
    return ListOut;
    }
}
List SortListDesending(List lst)
{
    int k = GetSize(lst);
    if(k==0)
    {
        cout << "Do dai chuoi khong hop le" <<"\n";
    }
    else
    {
        Data a[k];
    List ListOut;
    int temp = 0;
    Node* node = lst.phead;
    for(int i=0; i<k; i++)
    {
        a[i] = node->data;
        node = node->pnext;
    }

    for(int i = 0; i<k; i++)
    {
        for(int j=0; j<k; j++)
        {
            if(a[i].a < a[j].a)
            {
                 temp = a[i].a;
                 a[i].a = a[j].a;
                 a[j].a = temp;
            }
        }
    }

    for(int i = 0; i<k; i++)
    {
        node = CreatNode(a[i]);
        AddTail(ListOut,node);
    }
    return ListOut;
    }
}
void DestructList(List& lst)
{
    Node* node;
    Node* temp;
    node = lst.phead;
    while(node->pnext!=NULL)
    {
        temp = node;
        delete(temp);
        node = node->pnext;
    }
    delete(node);
}