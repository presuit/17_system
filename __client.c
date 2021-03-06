#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>

#define maxline 1000
//#define name_len 20

char* EXIT_STRING = "exit";

int tcp_connect(int af, char* servip, unsigned short port);
void errquit(char* msg){perror(msg); exit(1);}

int main(int argc, char* argv[]){
	char buf[maxline];
	char filename[maxline];
	char bufmsg[maxline];
	int maxfdp1;
	int s;
	int fd;
	int nread;
//	int namelen;

	fd_set read_fds;

	if(argc != 4){
		printf("Usage : %s server_ip port name \n", argv[0]);
		exit(1);
	}

//	sprintf(bufall, "[%s] : ", argv[3]);
//	namelen = strlen(bufall);
//	bufmsg = bufall + namelen;

	s = tcp_connect(AF_INET, argv[1], atoi(argv[2]));
	
	if(s == -1){
		errquit("tcp_connect fail");
	}
	puts("connect to server");
	maxfdp1 = s + 1;
	FD_ZERO(&read_fds);

	while(1){
		FD_SET(0, &read_fds);
		FD_SET(s, &read_fds);

		if( select(maxfdp1, &read_fds, NULL, NULL, NULL) < 0 ){
			errquit("select fail");
		}

		if(FD_ISSET(s, &read_fds)){
			int nbyte;

			if( (nbyte = recv(s, bufmsg, maxline, 0)) > 0 ){
				bufmsg[nbyte] = 0;
				printf("%s\n", bufmsg);
				memset(bufmsg, 0, strlen(bufmsg));
			}

		}

		if(FD_ISSET(0, &read_fds)){
		
			if(fgets(buf, maxline, stdin)){
				
				buf[strlen(buf) - 1] = 0;

				if(strstr(buf, EXIT_STRING) != NULL){
					puts("Good bye");
					close(s);
					exit(0);
				}//error check
				
				if( (fd = open(buf, O_RDONLY)) < 0 ){
					perror("fail to open");
					close(s);
					exit(0);
				}//file open

				memset(buf, 0, strlen(buf));

				while((nread = read(fd, buf, maxline)) > 0 ){
					
					if( send(s, buf, nread, 0) != nread ){
						printf("error : nread are not same to write\n");
					}
				}
	
			//	if( send(s, bufall, namelen + strlen(bufmsg), 0) < 0 ){
			//		puts("Error : write error on select.");
			//	}

			}
		}
	}
}

int tcp_connect(int af, char* servip, unsigned short port){
	
	struct sockaddr_in servaddr;
	int s;

	if( (s = socket(af, SOCK_STREAM, 0)) < 0 ){
		return -1;
	}

	bzero((char*)&servaddr, sizeof(servaddr));
	servaddr.sin_family = af;
	inet_pton(AF_INET, servip, &servaddr.sin_addr);
	servaddr.sin_port = htons(port);

	if( connect(s, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){
		return -1;
	}

	return s;
}
