#include <iostream>
#include "linked_list.h";
using namespace std;
//creat a Node
Node *CreatNode(Data data_in)
{
    Node *temp = new Node;
    if (temp!=NULL)
    {
    temp->data = data_in;
    temp->pnext = NULL;
    return temp;
    }
    else return NULL;

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
void AddHead (List& lst, Data value)
{
    Node* temp = CreatNode(value);
    if (Isempty(lst)==true)
    {
        lst.phead = temp;
        lst.pTail = temp;
    }
    else 
    {
        temp->pnext = lst.phead;
        lst.phead = temp;
    }
}
void AddTail(List& lst, Data value)
{
    Node* temp = CreatNode(value);
    if (Isempty(lst)==true)
    {
        lst.phead = temp;
        lst.pTail = temp;
    }
    else{
        lst.pTail->pnext = temp;
        lst.pTail = temp;
    }
}

void Insert(List& lst, Data value, int posision)
{
    if(posision ==0 || lst.phead == NULL)
    {
        AddHead(lst,value );
    }
    else 
    {
        int k =1 ;
        Node *p = lst.phead;
        while (p!=NULL &&k!=posision)
        {
            p = p->pnext;
            k++;
        }
        if (k!=posision)
        {
            AddHead(lst, value); 
        }
        else{
            Node* temp = CreatNode(value);
            temp->pnext = p->pnext;
            p->pnext = temp;
        }
    }
}