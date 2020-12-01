#include <stdio.h>
#include <iostream>
#include "define.hpp"
using namespace std;
void Xuat(DataSenSor* arg)
{
    cout<< "Name: "<<arg->Name<<"\n";
    cout << "Status: "<<arg->status<<"\n";
    cout<< "ID: "<<arg->ID<<"\n";
    cout <<"Lag: "<<arg->lagi<<"\n";
    cout <<"Logi: "<<arg->longi<<"\n";
}
