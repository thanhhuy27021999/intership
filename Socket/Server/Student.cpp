#include<iostream>
#include"string.h"
#include "Student.h"
using namespace std;

void Student::Nhap()
{
    cout <<"Id= ";
    cin>>Id;
    cout <<"Name: ";
    cin >>Name;
    cout <<"Class: ";
    cin>>Class;
   

}
void const Student:: Xuat()
{
    cout << "Name: " << Name<<"\n";
    cout << "Id: " <<Id<<"\n";
    cout<< "Class: "<< Class <<"\n";
}
