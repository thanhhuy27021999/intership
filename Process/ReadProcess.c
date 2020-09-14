#include"ReadProcess.h"
void GetJob(char * path)
{
	char * pth=malloc(sizeof(char)*90);
	char name[30],Cpus_allowed[30],Mems_allowed[300],null[30];
	strcpy(pth,path);
	strcat(pth,"status");
	FILE * file=fopen(pth,"r");
	if(!file)
	{
	  printf("cann't open file with path");
	}
	  while(1)
	  {
		  fscanf( file, "%s ", null);
		if(strcmp(null,"Name:")==0)
		   {
			fscanf( file, "%s ", name);
		   }
		if(strcmp(null,"Cpus_allowed:")==0)
		   {
			 fscanf( file, "%s ", Cpus_allowed);
                        
		   }
	      	if(strcmp(null,"Mems_allowed:")==0)
	      	    {
	     		 fscanf( file, "%s ", Mems_allowed);
			  break;
	     	    }
	   }
	      fclose(flie);
	       printf("Name : %s\nCpus_allowed : %s\nMems_allowed : %s \n",name,Cpus_allowed,Mems_allowed);
	     
}
void ReadProc(void)
{
	char * dirname_buf, * curr_path=malloc(sizeof(char)*10);
	DIR * directory;
	struct dirent * dir;
	directory=opendir("/proc");
	if(directory==NULL)
	{
	  printf("Unable to open /proc for read");
	}
	while((dir=readdir(directory))!=NULL)
	{
		dirname_buf=dir->d_name;
		if(atoi(dirname_buf)==0)
		{
		  continue;
		}
		strcpy(curr_path,"/proc/");
		strcat(curr_path,dirname_buf);
		strcat(curr_path,"/");
		printf("Path name : %s\n",curr_path);
                GetJob(curr_path);
		
	}
	free(curr_path);
	
}

