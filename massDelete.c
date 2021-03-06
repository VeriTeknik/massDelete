/* massDelete to delete everything inside a folder

- NO argument list too long
- NO waiting

@Authors: Cem Karaca, M. Emre Aydin
Copyright 2018

Version 0.5

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
#include <malloc.h>
#include <sys/time.h>
#include <errno.h> 


/* GLOBAL DECLARATIONS */
int i=0;
double cpu_time_used = 0;


void CTRLC(int deleted) {
    printf("\n\tdeleted: %d files in %d seconds\n\n",i,(int)cpu_time_used);
    exit(1);
}



int main (int argc, char **argv)
{
	signal(SIGINT, CTRLC);

	int verbose = 0;
	int options;
	char *directory = NULL;
	int dflag = 0;
	int sleep = 0;
	int preserve = 0;
	unsigned int sleepduration = 10;
	DIR *dir;
	struct dirent *dp;
	static char usage[] = "usage: ./massDelete -d <DIRECTORY> -v -s 2000 \n\t"
					    "-d <path to folder> to delete files inside with trailing slash. Also removes directory afterwards.\n\t"
                        "-p [OPTINAL] preserve directory after deleting everything inside.\n\t"
		                "-v [OPTIONAL] verbose.\n\t"
                        "-s [OPTIONAL] sleep between each delete in microseconds\n\n"
				        "Example: ./massDelete -d /var/lib/session -v -s 2000\n"
	                    "For further details and updates, check https://github.com/Veriteknik/massDelete\n";
	while ((options = getopt(argc, argv, "d:v::s:p")) != -1)
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
	    case 'p':
	        preserve = 1;
	        break;
		case '?':
	      return 1;
    default:
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
	struct timeval start, end;

	int averagedeletespersec = 0;
	int rmerr = 0;
	int rmdir_err = 0;

	gettimeofday(&start, NULL);
	
  while ((dp = readdir (dir)) != NULL) 
  {
  	if(dp->d_name[0] != '.')
  	{
  		i++;
  		if(p!='/') sprintf(path,"%s/%s", directory,dp->d_name);else sprintf(path,"%s%s", directory,dp->d_name);
  		if(verbose==1) { fprintf(stderr,"deleted: %s\n",path);  }
  		if(i%1000 ==0) 
			{ 
				 gettimeofday(&end, NULL);

				cpu_time_used = ((double) (end.tv_sec - start.tv_sec));
				averagedeletespersec = i/cpu_time_used;
				fprintf(stderr, "\r deleted: %d files | Average: %d/sec",i,averagedeletespersec);
			}
  		rmerr = remove(path);
  		if(rmerr!=0)
  		{
  			switch(rmerr)
  			{
  				case EACCES:
  					fprintf(stderr,"\ncannot delete %s, ACCESS DENIED\n",path);
  					break;
  				case EBUSY:
  					fprintf(stderr,"\ncannot delete %s, FILE IN USE\n",path);
  					break;
  				case ENOENT:
  					fprintf(stderr,"\ncannot delete %s, FILE DOESNT EXISTS\n",path);
  					break;
  				case EPERM:
  					fprintf(stderr,"\ncannot delete %s, ACCESS DENIED\n",path);
  					break;
  				case EROFS:
  					fprintf(stderr,"\ncannot delete %s, DIR READ ONLY\n",path);
  					break;
  				case ENAMETOOLONG:
  					fprintf(stderr,"\ncannot delete %s, NAME TOO LONG\n",path);
  					break;
  			}
  		}
  		if(sleep==1)usleep(sleepduration);
  	}
  	
	
	}

	if (preserve != 1) {
        rmdir_err = rmdir(directory);
        if (rmdir_err != 0) {
            printf("\nUnable to remove directory %s\n", directory);
        } else {
            printf("\nRemoved directory %s\n", directory);
        }
    }

	printf("\n\tDeleted a total of %d files in %d seconds\n\n",i,(int)cpu_time_used);
	return 0;
}