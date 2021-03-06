#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define maxline  511
#define max_sock 5

char* EXIT_STRING = "exit";
char* START_STRING = "connected to char server.\n";

int maxfdp1;
int num_chat = 0;
int clisock_list[max_sock];
int listen_sock;

void addclient(int s, struct sockaddr_in* newcliaddr);
int getmax();
void removeclient(int s);
int tcp_listen(int host, int port, int backlog);
void errquit(char* mseg){
	perror(mseg);
	exit(1);
}


int main(int argc, char* argv[]){
	struct sockaddr_in cliaddr;
	char buf[maxline + 1];
	char filename[maxline];
	int i, j, nbyte, accp_sock, fd;
	int addrlen = sizeof(struct sockaddr_in);
	fd_set read_fds;

	if(argc != 2){
		printf("Usage : %s port \n", argv[0]);
		exit(1);
	}

	listen_sock = tcp_listen( INADDR_ANY, atoi(argv[1]), 5 );
	
	while(1){
		FD_ZERO(&read_fds);
		FD_SET(listen_sock, &read_fds);
		FD_SET(0, &read_fds);
		for(i = 0; i < num_chat; i++){
			FD_SET(clisock_list[i], &read_fds);
		}

		maxfdp1 = getmax() + 1;
		puts("wait for client");
		
		if( select(maxfdp1, &read_fds, NULL, NULL, NULL) < 0 ){
			errquit("select fail");
		}

		if( FD_ISSET(listen_sock, &read_fds) ){
			accp_sock = accept( listen_sock, (struct sockaddr *)&cliaddr, &addrlen );
			if(accp_sock == -1){
				errquit("accept fail");
			}
			addclient(accp_sock, &cliaddr);
			send(accp_sock, START_STRING, strlen(START_STRING), 0);
			printf("%d th user addition\n", num_chat);
		}	

		for(i = 0; i < num_chat; i++){
			
			if( FD_ISSET(clisock_list[i], &read_fds) ){
				
				nbyte = recv(clisock_list[i], buf, maxline, 0);

				if(nbyte == 0){
					removeclient(i);
					continue;
				}

				for(j = 0; j < num_chat; j++){
					send(clisock_list[j], buf, maxline, 0);
				}
				buf[nbyte] = 0;
				printf("%s", buf);
				memset(buf, 0, nbyte);
			}
		}
	}
	return 0;
}


void addclient( int s, struct sockaddr_in* newcliaddr ){
	char buf[20];
	inet_ntop( AF_INET, &newcliaddr->sin_addr, buf, sizeof(buf) );
	printf("new client : %s\n", buf);
	clisock_list[num_chat] = s;
	num_chat++;
}

void removeclient(int s){
	close(clisock_list[s]);

	if( s != num_chat - 1 ){
		clisock_list[s] = clisock_list[num_chat - 1];
	}
	num_chat--;
	printf("Logouted one of the chatting user, number of chatting user %d\n", num_chat);
}

int getmax(){
	
	int max = listen_sock;
	int i;

	for(i = 0; i < num_chat; i++){
		
		if( clisock_list[i] > max ){
			
			max = clisock_list[i];
		}
	}
	return max;
}

int tcp_listen(int host, int port, int backlog){
	int sd;
	struct sockaddr_in servaddr;

	if( (sd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
		perror("socket fail");
		exit(1);
	}

	bzero((char*)&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(host);
	servaddr.sin_port = htons(port);

	if( bind(sd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0 ){
		perror("bind fail");
		exit(1);
	}

	listen(sd, backlog);
	return sd;
}
