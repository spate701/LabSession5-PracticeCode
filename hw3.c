#define MAXARGS 128
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h> 
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h> 
#include <sys/types.h>
#include <sys/resource.h>






//This is already defined within the C standard library
extern char **environ;



//MAXLINE constant
#define MAXLINE  8192 



       
/*Handler will go through conditionals from signal passed in and will look to recognize the type of signal that has been passed on and handle accordingly..
for this assignment, we are looking at 2 signal types, SIGINT AND SIGTSTP*/



void signal_Handler(int signal)
{


	if(signal == SIGINT)
		printf("\nHANDLED: SIGINT\n");

	else if(signal == SIGTSTP)
		printf("\nHANDLED: SIGTSTP \n");
}


/*This notifies us in the  case that either signal fails to get captured by the handler once called*/


void signal_shell()
{
	if(signal(SIGINT , signal_Handler) == SIG_ERR){
		printf("\nSIGINT: Handler failed to catch signal!\n");
	}

	if(signal(SIGTSTP , signal_Handler) == SIG_ERR){
		printf("\nSIGTSTP: Handler failed to catch signal!");
	}
}



/*Parsing Check for child process*/

 
void eval(char *cmdline)
{
	int parseTrack;
	char *argv[MAXARGS];
	char buf[MAXLINE];
	//defined in sys/types.h(included above)
	pid_t pid;

	strcpy(buf, cmdline);
	parseTrack = parseline(buf, argv);

	if (argv[0] == NULL) 
		return;
    
    	if (!bin_cmnd(argv)) {
		if ((pid = fork()) == 0) {
			if (execve(argv[0], argv, environ) < 0) {
			   printf("%s: ERROR! COMMAND DOES NOT EXIST.\n", argv[0]);
                	   exit(0);
			}
		}
	
		if (!parseTrack) {
			int status;

			if (waitpid(pid, &status, 0) < 0)
			   printf("Error(waitpid): waitfg");
		}
 
			else
				printf("%d %s", pid, cmdline);
        }
			 
		return;
} 




int parseline(char *buf, char **argv)
{
	int parseTrack;
	char *processVar;
	int argc;


	buf[strlen(buf)-1] = ' ';
	while (*buf && (*buf == ' '))
		buf++;
	
	//build list here for argv
	argc = 0;

	while ((processVar = strchr(buf, ' '))) {
		argv[argc++] = buf;
		*processVar = '\0';
		buf = processVar + 1;
		while (*buf && (*buf == ' '))
			buf++;
	}

	argv[argc] = NULL;

	if (argc == 0)
		return 1;

	if ((parseTrack = (*argv[argc-1] == '&')) != 0)
		argv[--argc] = NULL;
	
	return parseTrack;
} 


int bin_cmnd(char **argv)
{
		if(!strcmp(argv[0], "EXIT\n"))
			exit(0);
		return 0;
}








int main()
{
	int exit_status;
	int status = 0;
	pid_t pid;
	char cmdline[MAXLINE];
	//int command;
	char **arg = malloc(25*sizeof(char*));
	signal_shell();		
	while(1) {
		printf("CS361 >");
		fgets(cmdline, MAXLINE, stdin);
		if(strcmp(cmdline, "exit\n")==0){
			exit(0);
		} 
		if (feof(stdin))
			exit(0);
		
		//if(strcmp(arg[1],"<"))
			//printf("Success"); 
		eval(cmdline);
		pid = fork();
		wait(NULL);
		
		if(pid<0) 
			printf("Error: fork");

		if(pid==0){
            //wait(&status);
            printf("pid 0\n");
            //printf("Child PID:%d\n", pid);        
			execvp(arg[0],arg);
		}
        
        else {
            printf("I am the parent. I am waiting for my child %d to die. \n", pid);
            int status;
            wait(&status);
            printf("My child has died with status %d. :(\n", WEXITSTATUS(status));
             
        }

        

		if ( WIFEXITED(status) ) {
                	exit_status = WEXITSTATUS( status );
                	printf("EXIT: %d\n", exit_status);
            	}
            	
            }
	}


