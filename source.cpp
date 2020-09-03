#include<iostream>
#include "linked_list.h"
using namespace std;

int main()
{   
    Data data_node1;
    List lst;
    data_node1.a= 100;
    Node *node1= CreatNode(data_node1);
    cout <<"gia tri cua node1 la: " << node1->data.a << "\n";
    cout <<"gia tri cua pnext la: "<<node1->pnext <<"\n";
    init(lst);
    if (Isempty(lst))
    cout << " Danh sach la rong " <<"\n";
    else cout << "Danh sach la khong rong " <<"\n";

}
