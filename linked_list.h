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
void AddHead (List& lst, Data value);
void AddTail(List& lst, Data value);
void Insert(List& lst, int value, int posision);


