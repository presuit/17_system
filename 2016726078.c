#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>

#define BUF_SIZE 512
#define PERM 0666

int copyDouble(const char *val1, const char *val2);
int checkFile(const char *val);

int main(){
	copyDouble("sosil.txt", "target.txt");
	checkFile("sosil.txt");
	checkFile("target.txt");
	return 0;
}

int copyDouble(const char *val1, const char *val2){
	int sosil, target, total;
	char buf[BUF_SIZE];
	ssize_t nread;	
	
	if( (sosil = open(val1, O_RDONLY)) == -1 ){
		printf("Error : not valid open file.\n");
		return -1;
	}

	if( (target = open(val2, O_WRONLY | O_CREAT | O_TRUNC, PERM)) == -1 ){
		printf("Error : not valid open file.\n");
		close(sosil);
		return -1;
	}

	while( (nread = read(sosil, buf, BUF_SIZE)) > 0 ){
		if( write(target, buf, nread) < nread ){
			printf("Error : can't write to target file\ncheck sosil file\n");
			close(sosil);
			close(target);
			return -1;
		}
	}

	printf("Perfectly copy sosil to target.\ndouble copy process initiate.\n");
//double copy

	lseek(sosil, (off_t)0, SEEK_SET);
	lseek(target, (off_t)0, SEEK_END);

	nread = read(sosil, buf, BUF_SIZE);
	write(target, buf, nread);


	printf("double copy process end.\n\n");

//return memory

	close(sosil);
	close(target);

//check error
	
	if(nread == -1){
		printf("Error : not valid nread. check files.\n");
		return -1;
	}
	else {
		return 0;
	}
}

int checkFile(const char *val){
	int fd;
	ssize_t nread;
	char buf[BUF_SIZE];

	if( (fd = open(val, O_RDONLY)) == -1 ){
		return -1;
	}

	while( (nread = read(fd, buf, BUF_SIZE)) > 0 ){
		printf("%s : \n", val);
		for(int i = 0; i < nread; i++){
			printf("%c", buf[i]);
			buf[i] = ' ';
		}
	}

	printf("\n\n");

	close(fd);

	return 0;
}
