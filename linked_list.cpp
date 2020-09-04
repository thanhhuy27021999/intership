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
void init (List& lst)
{
    lst.phead = lst.pTail = NULL;
};
bool Isempty(List& lst)
{
    if (lst.phead == NULL)
    return true ;
    else return false ;
};
void AddHead (List& lst, Node* node)
{
    if (Isempty(lst)==true)
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
    if (Isempty(lst)==true)
    {
        lst.phead = node;
        lst.pTail = node;
    }
    else{
        lst.pTail->pnext = node;
        lst.pTail = node;
    }
};

void InsertAfterQ(List& lst, Node* p, Node* q)
{
	if (q != NULL)
	{
		p->pnext = q->pnext;
		q->pnext = p;
		if (lst.pTail == q)
			lst.pTail = p;
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

void PrinfList(List& lst)
{
    if(lst.phead!=NULL)
    {
        Node* node = lst.phead;
        while (node!=NULL)
        {
            cout << node->data.a;
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
	return NULL;
}
 
