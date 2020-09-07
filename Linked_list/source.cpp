#include<iostream>
#include "linked_list.h"
using namespace std;

int main()
{   
    Data value,value_20;
    value_20.a= 20;
    List list1;
    init(list1);
    Node* node;
    for (int i=0; i<10; i++)
    {
        value.a = i;
        node = CreatNode(value);
        AddTail(list1,node);
    }
    PrinfList(list1);
    cout <<"\n";
    node = Getnode(list1,2);
    cout << node->data.a <<"\n";
    Node* node_20 = CreatNode(value_20);
    Node* node4 = Search(list1, 4);
    cout <<node4->data.a << "\n";
    InsertAfterQ(list1,node_20,node4);
    PrinfList(list1);
    int x;
    RemoveAfterQ(list1,node4, x);
    cout<<"\n" << x <<"\n";
    PrinfList(list1);
    x = GetLeghtList(list1);
    cout << "\n" <<x;
}
