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

#define MAX_LINE 511

int tcp_connect(int af, char * servip, unsigned short port);
void errquit(char * msg){
	perror(msg);
	exit(1);
}

int main(int argc, char ** argv){
	int maxfdp1;
	int s, fd, nread;
	char buf[MAX_LINE + 1];
	fd_set read_fds;

	if(argc != 4){
		printf("Usage : %s server_ip port name \n", argv[0]);
		exit(1);
	}

	s = tcp_connect( AF_INET, argv[1], atoi(argv[2]) );

	if(s == -1){
		errquit("tcp_connect fail");
	}
	puts("connect to server");
	maxfdp1 = s + 1;
	
	FD_ZERO(&read_fds);
	
	while(1){
		printf("loop station\n");
		FD_SET(0, &read_fds);
		FD_SET(s, &read_fds);

		if( select(maxfdp1, &read_fds, NULL, NULL, NULL) < 0 ){
			errquit("select fail");
		}	
		
		if( FD_ISSET(s, &read_fds) ){
			if( (nread = recv(s, buf, MAX_LINE, 0)) > 0 ){
				buf[nread] = 0;
				fputs(buf, stdout);
				memset(buf, 0, sizeof(buf));
			}
		}

		if( FD_ISSET(0, &read_fds) ){
			fgets(buf, MAX_LINE, stdin);
			buf[strlen(buf) - 1] = '\0';
			printf("buf_test : %s\n", buf); //test
	
			if( (fd = open(buf, O_RDONLY)) < 0 ){
				perror("fail to open");
				exit(1);
			}

			memset(buf, 0, sizeof(buf));

			while( (nread = read(fd, buf, MAX_LINE) ) > 0 ){
				write(s, buf, nread);
				memset(buf, 0, sizeof(buf));
			}
			printf("done to send\n");
		}
	}
	return 0;
}

int tcp_connect(int af, char * servip, unsigned short port){
	struct sockaddr_in servaddr;
	int s;

	if( (s = socket(af, SOCK_STREAM, 0)) < 0 ){
		return -3;
	}
	
	memset( (char*)&servaddr, 0, sizeof(servaddr) );
	servaddr.sin_family = af;
	inet_pton(AF_INET, servip, &servaddr.sin_addr);
	servaddr.sin_port = htons(port);

	if( connect(s, (struct sockaddr * )&servaddr, sizeof(servaddr)) < 0 ){
		return -4;
	}

	return s;
}
