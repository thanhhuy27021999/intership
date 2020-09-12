void get_job(char * path)
{
	#ifdef DEBUG
	printf("DEBUG: running get_job(%s)\n",path);
	#endif
	char * pth=malloc(sizeof(char)*90);
	char 
	name[BUF_LEN],	// the name of the job
	/*
	   there are descriptions for human readers
	   we throw them away with this.
	 */
        null[BUF_LEN],	
	state,
        tgid[BUF_LEN],	// these informations we throw away, too
        ngid[BUF_LEN],
        tracerpid[BUF_LEN];

	unsigned int pid,ppid,uid;
	

	strcpy(pth,path);
	strcat(pth,"status");
	FILE * file=fopen(pth,"r");
	if(!file)
	{
		printf("ERROR: FATAL: cannot open %s for read\n",pth);
		exit(-2);
	}

	fscanf( file, "%s ", null);
        fscanf( file, "%s ",name);
        fscanf( file, "%s ",null);
        fscanf( file, "%c ",&state);
        fscanf( file, "%s ",null);
        fscanf( file, "%s ",null);
        fscanf( file, "%s ",tgid);
        fscanf( file, "%s ",null);
        fscanf( file, "%s ",ngid);
        fscanf( file, "%s ",null);
        fscanf( file, "%u ",&pid);
        fscanf( file, "%s ",null);
        fscanf( file, "%u ",&ppid);
        fscanf( file, "%s ",null);
        fscanf( file, "%s ",tracerpid);
	fscanf( file, "%s ",null);
	fscanf( file, "%u ",&uid);
	fclose(file);

	#ifdef DEBUG
	printf("%s %c %u %u %u\n",name,state,pid,ppid,uid);
	#endif
	/*
	struct Job * jb=newJob(name,pid,ppid,uid,state);
	free(pth);
	return jb;
    */
}
