#include<dirent.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <ctype.h> // isdigit() function
void get_job(char * path);//get info of one process
void read_proc(void);     //get info of all process
int is_uint(char input[]);//check input is a digit
