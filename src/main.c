#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <assert.h>

#include "stats.h"


int main(int argc, char *argv[])
{
	
	int pipefd[2];
	pid_t cpid;


	if(pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	cpid = fork();

	if(cpid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if(cpid == 0)
	{ 		
		//child reads from pipe
		close(pipefd[1]); //closing unused write end
		
		char str[2]; 
		char buf[256];
		double* vectors[256];
		int i = 0; int j = 0;
		vectors[i]= (double*) malloc (sizeof(double)*256);
		int n_el_in_row[256];				
				
		/*Critical part*/
		
		while(read(pipefd[0], &str, 1)>0)
		{ 
			//read 1 byte =read 1 char
			
			if(strcmp(str, "\n") == 0)
			{
				//printf("saw a new line\n");
				double t = atof(buf);
				//printf("number collected: %lf\n", t);
				strcpy(buf, "");
				//printf("buf is empty\n");
				n_el_in_row[i] = j+1;
				vectors[i++][j]= t;
				
				//printf("value put in vectors: %lf\n", vectors[i-1][j]);				
				j = 0;
				vectors[i] = (double*) malloc (sizeof(double)*256);
				
							
			} 
			
			else if(strcmp(str, " ") == 0)
			{
				//printf("saw a space\n");
				if(strcmp(buf, "")!=0)
				{
					double t;
					t = atof(buf);
					//printf("number collected: %lf\n", t);
					strcpy(buf, "");
					//printf("buf is empty\n");	
					vectors[i][j++]= t;		
					//printf("value put in vectors: %lf\n", vectors[i][j-1]);
				}		
			} 
			
			else 
			{
				strcat(buf, str);			
			}	
		}
		
		/*after this while loop, i is the number of rows*/
		
		
		/*
		int idx = 0;
		while(idx < i){
			printf("no of elements in row %d: %d\n", idx, n_el_in_row[idx]);
			idx++;
		}
		*/
		
		double transpose[256][256];
		
		for(int x = 0; x <= i; ++x)
		{
			for(int y = 0; y <= n_el_in_row[x] ; ++y)
				transpose[y][x] = vectors[x][y];
		}
		
		/*FOR LOOP TO PARSE COMMAND LINE ARGUMENTS*/
		stats_t* res = (stats_t*) malloc(sizeof(stats_t));
		
		for(int a=1;a<argc;a++)
		{
			if(strcmp(argv[a], "max") == 0)
			{
				//printf("calculate %s of column vector %d\n", argv[a], atoi(argv[a+1]));
				stats(transpose[atoi(argv[a+1])], i, res);
				printf("%lf ", res->max);
				fflush(stdout);
			}
			
			else if(strcmp(argv[a], "min") == 0)
			{
				//printf("calculate %s of column vector %d\n", argv[a], atoi(argv[a+1]));
				stats(transpose[atoi(argv[a+1])], i, res);
				printf("%lf ", res->min);
				fflush(stdout);
			} 
			
			else if(strcmp(argv[a], "mean") == 0)
			{
				//printf("calculate %s of column vector %d\n", argv[a], atoi(argv[a+1]));
				stats(transpose[atoi(argv[a+1])], i, res);
				printf("%lf ", res->mean);
				fflush(stdout);
			} 
			
			else if(strcmp(argv[a], "median") == 0)
			{
				//printf("calculate %s of column vector %d\n", argv[a], atoi(argv[a+1]));
				stats(transpose[atoi(argv[a+1])], i, res);
				printf("%lf ", res->median);
				fflush(stdout);
			} 
		
			else 
				continue;	
		}
		
		printf("\n");
		
		/*END FOR LOOP TO PARSE COMMAND LINE ARGUMENTS*/
		
		
		/*End CP*/
			
		close(pipefd[0]);
		_exit(EXIT_SUCCESS);
	
		
	    else 
        { 
            //parent only writes to pipe

		    close(pipefd[0]); //close unused read end
		    char *ch;
		    while (read(STDIN_FILENO, &ch, 1)>0)
            {
			    if (write(pipefd[1], &ch, 1) == -1)
                {
				    perror("write");
				    exit(EXIT_FAILURE);		
			    }
		}
		
        close(pipefd[1]);
		wait(NULL); //wait for child
		exit(EXIT_SUCCESS);
	}


    return EXIT_SUCCESS;
}   
