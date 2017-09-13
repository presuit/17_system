#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>

int main(int argc, char* argv[]){
  int fd1, fd2, nread;
  char* buf;
  struct stat filestat;

//check error

  if( argc < 2 ){
    fprintf(stderr, "Usage : more input need\n");
    return 1;
  }
//original file, copped file open

  if( (fd1 = open(argv[1], O_RDONLY)) == -1 ){
    perror("error open fd1");
    return 1;
  }
  if( (fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0755)) == -1 ){
    perror("error open fd2");
    return 1;
  }

//get information about original file

  stat(argv[1], &filestat);
  buf = (unsigned char*)malloc(filestat.st_size);
  memset(buf, 0x00, filestat.st_size);

//read original file and write to copped file
  
  if( (nread = read(fd1, buf, filestat.st_size )) > 0 ){
    write(fd2, buf, nread);
  }

  close(fd1); 
  close(fd2); 

  return 0;
