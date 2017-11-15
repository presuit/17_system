#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>

#define BUF_SIZE 256
#define FIFO_PERM (S_IRUSR | S_IWUSR)

int main(int argc, char* argv[]){
	char buf[BUF_SIZE];
	int fd;
	int rval;

	//chech arguments validation
	if(argc < 2){
		fprintf(stderr, "Usage : %s\n", argv[0]);
		return 1;
	}

	//create FIFO file
	//open file
	if( (fd = open(argv[1], O_RDONLY)) == -1 ){
		fprintf(stderr, "can't open FIFO file.");
		return 1;
	}

	while(1){
		if( rval = read(fd, buf, BUF_SIZE) < 0 ){
			fprintf(stderr, "read data error");
			return 1;
		}

		if(!strcmp(buf, "quit")){
			break;
		}
		else {
			printf("[recv] : ");
			for(int i = 0; i < strlen(buf); i++){
				printf("%c", buf[i]);
			}
			printf("\n");
			fflush(stdout);
			memset(buf, ' ', strlen(buf) + 1);
		}
	}
	return 0;
}
