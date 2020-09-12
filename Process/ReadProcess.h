#include<dirent.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#define BUF_LEN			30
struct Job
{
	char * name;		// the name of the job, as in comm
	unsigned int pid;	// the process' id
	unsigned int ppid;	// the parent's pid
	unsigned int uid;	// the user's id
	char status;		// the status like 'S' for sleep


};

//struct Job * get_job(char * path);
void get_job(char * path);