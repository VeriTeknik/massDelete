/* massDelete to delete everything inside a folder

- NO argument list too long
- NO waiting

@Author: Cem Karaca
Copyright 2016

Version 0.3

USE AT YOUR OWN RISK

*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>

int i=0;

void CTRLC(int deleted) {
    printf("\n\tdeleted: %d files\n\n",i);
    exit(1);
}


int main (int argc, char **argv)
{
	signal(SIGINT, CTRLC);

	int verbose=0;
	int options;
	char *directory = NULL;
	int dflag = 0;
	int sleep = 0;
	unsigned int sleepduration = 10;
	DIR *dir;
	struct dirent *dp;
	static char usage[] = "usage: ./massDelete -d <DIRECTORY> -v -s 2000 \n\t-d <path to folder to delete files inside with trailing slash.\n\t-v verbose.\n\t-s sleep between each delete in microseconds\n\nExample: ./massDelete -d /var/lib/session -v -s 2000\n";
	while ((options = getopt(argc, argv, "d:v::s:")) != -1)
	switch(options)
	{
		case 'd':
			dflag = 1;
			directory = optarg;
			break;
		case 'v':
			verbose = 1;
			break;
		case 's':
			sleep = 1;
			sleepduration =  atoi(optarg);
			break;
		case '?':
//			if (optopt == 'd')
//         fprintf (stderr, "Option -%c requires an argument.\n", optopt);
      return 1;
    default:
 //   	printf('%s',usage);
    	abort();
			
	}
	
	if (dflag == 0) 
	{
		fprintf(stderr, "%s: missing -d option\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}
	
	char p = directory[strlen(directory)-1];
	
	printf("Directory= \"%s\"\n", directory);
	verbose==1?printf("Verbosity enabled\n\n"):printf("Verbosity disabled\n\n");
	if(sleep==1)printf("Sleep duration %u microseconds\n",sleepduration);

	if ((dir = opendir (directory)) == NULL) 
	{
        fprintf (stderr, "\n\nCannot open directory %s\n\n",directory);
        exit (1);
  }
  
	char *path = (char*)malloc(255*sizeof(char));
  while ((dp = readdir (dir)) != NULL) 
  {
  	if(dp->d_name[0] != '.')
  	{
  		i++;
  		if(p!='/') sprintf(path,"%s/%s", directory,dp->d_name);else sprintf(path,"%s%s", directory,dp->d_name);
  		if(verbose==1) printf("deleted: %s\n",path);
  		if(i%1000 ==0) printf("\n\tdeleted: %d files\n\n",i);
  		remove(path);
  		if(sleep==1)usleep(sleepduration);
  	}
  	
	
	}
	printf("\n\tDeleted a total of %d files\n\n",i);
	return 0;
}