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
#ifdef __linux__
#include <malloc.h>
#endif
#include <sys/time.h>
#include <sys/stat.h>
#include <errno.h> 


/* GLOBAL DECLARATIONS */
int i=0;
double cpu_time_used = 0;
int verbose_global = 0;
int sleep_global = 0;
unsigned int sleepduration_global = 10;


void CTRLC(int deleted) {
    printf("\n\tdeleted: %d files in %d seconds\n\n",i,(int)cpu_time_used);
    exit(1);
}

int recursive_delete(const char *path) {
    DIR *dir;
    struct dirent *dp;
    struct stat statbuf;
    char full_path[1024];
    int result = 0;
    
    if (stat(path, &statbuf) != 0) {
        return -1;
    }
    
    if (S_ISDIR(statbuf.st_mode)) {
        // It's a directory, open it and process contents
        dir = opendir(path);
        if (dir == NULL) {
            return -1;
        }
        
        while ((dp = readdir(dir)) != NULL) {
            if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
                continue;
            }
            
            snprintf(full_path, sizeof(full_path), "%s/%s", path, dp->d_name);
            
            // Recursively delete contents
            if (recursive_delete(full_path) != 0) {
                result = -1;
            }
        }
        closedir(dir);
        
        // Now delete the empty directory
        if (rmdir(path) == 0) {
            i++;
            if (verbose_global) {
                fprintf(stderr, "deleted directory: %s\n", path);
            }
        } else {
            result = -1;
        }
    } else {
        // It's a file, delete it
        if (remove(path) == 0) {
            i++;
            if (verbose_global) {
                fprintf(stderr, "deleted file: %s\n", path);
            }
        } else {
            result = -1;
        }
        
        if (sleep_global) {
            usleep(sleepduration_global);
        }
    }
    
    return result;
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
	int recursive = 1;  // Default to recursive mode now
	unsigned int sleepduration = 10;
	DIR *dir;
	struct dirent *dp;
	static char usage[] = "usage: ./massDelete -d <DIRECTORY> -v -s 2000 -r\n\t"
					    "-d <path to folder> to delete files inside with trailing slash. Also removes directory afterwards.\n\t"
                        "-p [OPTINAL] preserve directory after deleting everything inside.\n\t"
		                "-v [OPTIONAL] verbose.\n\t"
                        "-s [OPTIONAL] sleep between each delete in microseconds\n\t"
                        "-r [DEFAULT] recursive deletion of directories (now default behavior)\n\n"
				        "Example: ./massDelete -d /var/lib/session -v -s 2000\n"
	                    "For further details and updates, check https://github.com/Veriteknik/massDelete\n";
	while ((options = getopt(argc, argv, "d:v::s:pr")) != -1)
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
	    case 'r':
	        recursive = 1;
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
	
	// Set global variables for the recursive function
	verbose_global = verbose;
	sleep_global = sleep;
	sleepduration_global = sleepduration;

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
	
	// Process directory contents recursively
	while ((dp = readdir(dir)) != NULL) 
	{
		if(dp->d_name[0] != '.')
		{
			if(p!='/') sprintf(path,"%s/%s", directory,dp->d_name);else sprintf(path,"%s%s", directory,dp->d_name);
			
			// Use recursive deletion
			recursive_delete(path);
			
			if(i%1000 ==0) 
			{ 
				gettimeofday(&end, NULL);
				cpu_time_used = ((double) (end.tv_sec - start.tv_sec));
				averagedeletespersec = i/cpu_time_used;
				fprintf(stderr, "\r deleted: %d items | Average: %d/sec",i,averagedeletespersec);
			}
		}
	}
	
	closedir(dir);

	if (preserve != 1) {
        rmdir_err = rmdir(directory);
        if (rmdir_err != 0) {
            printf("\nUnable to remove directory %s\n", directory);
        } else {
            printf("\nRemoved directory %s\n", directory);
        }
    }

	gettimeofday(&end, NULL);
	cpu_time_used = ((double) (end.tv_sec - start.tv_sec));
	printf("\n\tDeleted a total of %d items in %d seconds\n\n",i,(int)cpu_time_used);
	return 0;
}