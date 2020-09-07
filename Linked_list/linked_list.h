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
void Init (List& lst); // make a initial
bool IsEmpty(List& lst);
void AddHead (List& lst, Node* node);
void AddTail(List& lst,Node* node);
void RemoveHead(List& lst, int&x);
Node* Getnode(List& lst, int index);
void InsertAfterIndex(List& lst, Node* p, Node* q);
void PrintList(List& lst);
Node* Search(List& lst, int x);
void RemoveAfterIndex(List& list, Node* q, int& x);
int GetSize(List& lst);
List SortListDesending(List lst);
List SortListAsending(List lst);









