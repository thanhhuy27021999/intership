#include <iostream>
using namespace std;
struct Data
{
    int a;
};
struct Node
{
    Data data;
    Node *pnext;
};
struct List
{
    Node *phead, *pTail;
};
Node *CreatNode(Data data_in); // Create a Node
void init (List& lst); // make a initial
bool Isempty(List& lst);
void AddHead (List& lst, Node* node);
void AddTail(List& lst,Node* node);
void RemoveHead(List& lst, int&x);
Node* Getnode(List& lst, int index);
void InsertAfterQ(List& lst, Node* p, Node* q);
void PrinfList(List& lst);
Node* Search(List& lst, int x);








