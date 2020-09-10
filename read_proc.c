#include<stdlib.h>
#include<string.h>
#include<stdio.h>
//#include "struct.h"
#include"read_proc.h"

void Get_Proc(char* Path)
{
    int escape =1;
    char null[buffer],name[buffer],Cpus_allowed[buffer], Cpus_allowed_list[buffer];
    char* pth = (char*)malloc(90*sizeof(unsigned char));
    strcpy(pth,Path);
    strcat(pth,"status");

    printf("%s",pth);
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
            if(null=="Name:")
            {
                fscanf(file,"%s",name);
            }	
            if(null=="Cpus_allowed:")
            {
                fscanf(file,"%s",Cpus_allowed);
            }
            if(null=="Cpus_allowed_list:")
            {
                fscanf(file,"%s",Cpus_allowed_list);
                escape =0;
            }
        }
    }
    fclose(file);
    free(pth);
    printf("%s",name);

}
