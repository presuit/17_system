#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>

#define FIFO_PERM (S_IRUSR | S_IWUSR)
#define BUF_SIZE 256

int main(int argc, char* argv[]){
	int fd;
	int wval;
	char buf[BUF_SIZE]; 

	//check arguments validation
	if(argc < 2){
		fprintf(stderr, "Usage : %s pipename\n", argv[0]);
		return 1;
	}

	//open or create FIFO
	if( mkfifo(argv[1], FIFO_PERM ) == -1 ){
		if(errno != EEXIST){
		
			fprintf(stderr, "failed to create FIFO %s\n", strerror(errno));
			return 1;
		}
	}

	//open fifo file
	if((fd = open(argv[1], O_WRONLY)) == -1){
		fprintf(stderr, "open file error\n");
		return 1;
	}

	//send data
	while(1){
		printf("[send] : ");
		fgets(buf, BUF_SIZE, stdin);
		buf[strlen(buf) - 1] = '\0';

		if(!strcmp(buf, "quit")){
			write(fd, buf, strlen(buf) + 1 );
			break;
		}

		if( (wval = write(fd, buf, strlen(buf) + 1)) < 0 ){
			fprintf(stderr, "can't write data to FIFO file\n");
			return 1;
		}
	}
	

	return 0;
}
