#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int num, status, i;
	pid_t* pid;

//check arguments    
    if(argc < 2){
	fprintf(stderr, "Usage: %s command arg1 arg2...\n",argv[0]);
	return 1;
    }
//cast char type to int type
    num = atoi(argv[1]);
//set memory 	
	pid = (pid_t *)malloc( sizeof(pid_t) * num );
//create child process num th times 
	for(i = 0; i < num; i++){
		if( (pid[i] = fork()) == 0 ) {
			//child
			printf( "[%d] Parent : %d / Child : %d\n", i, getppid(), getpid() );
			exit(0);
		}
	}
//wait and clean zombie process
	for(i = 0; i < num; i++){
		waitpid( pid[i], &status, 0 );
		if(i == num - 1){
			printf( "Parent %d is running.\n", getpid() );
		}
	}
    return 0;
}   
