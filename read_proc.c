#include<stdlib.h>
#include<string.h>
#include<stdio.h>
//#include "struct.h"
#include"read_proc.h"
char Name[] = "Name:";
char Cpus_all[] = "Cpus_allowed:";
char Cpus_all_list[] = "Cpus_allowed_list:";
char mems_all[] = "Mems_allowed:";
char mems_all_list[] = "Mem_allowed_list:";
char Pid[] = "Pid:";
void Get_Proc(char* Path)
{
    int escape =1;
    char null[buffer],name[buffer],Cpus_allowed[buffer], Cpus_allowed_list[buffer];
    char mems_allowed[100], mems_allowed_list[buffer],PID[buffer];
    char* pth = (char*)malloc(90*sizeof(unsigned char));
    strcpy(pth,Path);
    strcat(pth,"status");

    printf("%s \n",pth);
    FILE* file = fopen(pth,"r");
    if(!file)
    {
        printf("file can not be opened \n");
        exit(2);
    }
    else
    {
        while(escape)
        {
            fscanf( file, "%s ", null);
            if(strcmp(null,Name)==0)
            {
                printf("%s",null);
                fscanf(file,"%s",name);
                printf("%s \n",name);
            }	
            if(strcmp(null,Pid)==0)
            {
                printf("%s",null);
                fscanf(file,"%s",PID);
                printf("%s \n",PID);
            }
            if(strcmp(null,Cpus_all)==0)
            {
                printf("%s ",null);
                fscanf(file,"%s",Cpus_allowed);
                printf("%s\n",Cpus_allowed);
            }
            if(strcmp(null,Cpus_all_list)==0)
            {
                printf("%s",null);
                fscanf(file,"%s",Cpus_allowed_list);
                printf("%s \n",Cpus_allowed_list);
            }
            if(strcmp(null,mems_all)==0)
            {
                printf("%s",null);
                fscanf(file,"%s",mems_allowed);
                printf("%s \n",mems_allowed);
            }
            if(strcmp(null,mems_all_list)==0)
            {
                printf("%s",null);
                fscanf(file,"%s",mems_allowed_list);
                printf("%s \n",mems_allowed_list);
                escape=0;
            }
        }
    }
    fclose(file);
    free(pth);
    printf("%s",name);

}
