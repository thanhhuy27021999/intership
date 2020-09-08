#include<iostream>
#include "linked_list.h"
using namespace std;

int main()
{   
    Data value;
    List list1;
   // List list2, list3;
    Init(list1);
    Node* node;
    int n;
    cout <<"moi nhap so luong so n= ";
    cin >> n;
    for (int i=0; i<n; i++)
    {
        value.a = rand() % (15 - 1 + 1) + 1;// random value from 1 - 15
        node = CreatNode(value);
        AddTail(list1,node);
    }
    //PrintList(list1);
   //DestructList(list1);
    //cout <<"\n";
    //list2 = SortListAsending(list1);
    //PrintList(list2);
    //cout<< "\n";
    //list3 = SortListDesending(list1);
    //PrintList(list3);
    //cout<<"\n";
    // cout << node->data.a <<"\n";
    // node = Getnode(list2,2);
    // cout << node->data.a <<"\n";
    // node = Getnode(list3,2);
    // cout << node->data.a <<"\n";
//  node = Getnode(list2,2);
//  cout<<node->data.a<<"\n";
//  node = Getnode(list1,2);
//  cout<<node->data.a<<"\n";
//  PrintList(list1);
//  cout <<"\n";
//  node = Getnode(list1,2);
//  cout << node->data.a <<"\n";
//  Node* node_20 = CreatNode(value_20);
//  Node* node4 = Search(list1, 4);
//  cout <<node4->data.a << "\n";
//  InsertAfterIndex(list1,node_20,node4);
//  PrintList(list1);
//  int x;
//  RemoveAfterIndex(list1,node4, x);
//  cout<<"\n" << x <<"\n";
//  PrintList(list1);
//  x = GetSize(list1);
//  cout << "\n" <<x;
 }
