
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

/** Given a host and a port number, connect as a client to that host/port
 Return a file descriptor number corresponding to the new socket
 The socket fd number (returned) can be used as follows:
 @param host
 @param portno
 @return socketfd

 */

int connectTo(char* host, int portno) {
	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	printf("hostname=%s\n", host);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server = gethostbyname(host);
    
	if (server == NULL) {
		printf(" No such host\n ", NULL);
		exit(0);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr,
			server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))
			< 0) {
		printf("Error Connecting\n", NULL);
	}
	return sockfd;
}

/**
 * Form a server listener that waits for new connections
 One listener can answer many connections
 This returns a file descriptor number that can be used
 to listen for "new connections," and perhaps spawn
 threads corresponding to those connections.
 * @param portno
 * @return
 */
int makeListener(int portno) {
	int sockfd;
	struct sockaddr_in serv_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		printf("ERROR opening socket\n", NULL);
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		printf("ERROR on binding\n", NULL);
		exit(0);
	}

	listen(sockfd, 15);

	return sockfd;
}

/**
 *
 *  Given an fd number provided by makeListener,
 block until a new connection is received.
 Return the file descriptor number for that
 new connection 
 @param sockfd
 @return
 */

int listenFor(int sockfd) {
	int newsockfd;
	socklen_t clilen;
	struct sockaddr_in cli_addr;

	clilen = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) {
		printf("ERROR on accept\n", NULL);
	}
	return newsockfd;
}


void * reciveData(void *connection_fd){

int * file_descriptor = (int*)connection_fd;
char buffer[1024];
int n = 0;
bzero(buffer, 1024);
while(1){
	
	n =read(*file_descriptor, buffer, sizeof(buffer));
	if(n== -1){
		printf("error");
	return;
}
	
	
	printf(" %s\n", buffer);
	 if ((strcmp(buffer , "q") == 0) || strcmp(buffer , "Q") == 0){
		write(*file_descriptor, buffer, strlen(buffer));
		return;
}
	
bzero(buffer, 1024);
}
	

}


void * sendData(void * connection_fd){

int * file_descriptor = (int*)connection_fd;

char send_data[1024];
char buffer[1024];
bzero(buffer, 1024);

while(1){

	printf("Type Something (q or Q to quit):");
	gets(send_data);
	
	 if ((strcmp(send_data , "q") == 0) || strcmp(send_data , "Q") == 0){
     		
		return;
	}
	else{

	
	strncpy(buffer, send_data, strlen(send_data)+1);
	}
	 write(*file_descriptor, buffer, strlen(buffer));
	 bzero(buffer, 1024);
	
}

}
int main(int argc, char** argv) {
	int  connection_fd;
	char buffer[1024];
	int n;
	pthread_t recieveDataThread, sendDataThread;
	
	if(argc < 3) {
	   printf("Usage: %s port\n", argv[0]);
	   exit(0);
	
	}
	
	connection_fd = connectTo(argv[2], atoi(argv[1]));
        
	pthread_create(&recieveDataThread, NULL ,reciveData,&connection_fd);
	pthread_create(&sendDataThread,NULL,sendData,&connection_fd);
	
	
	pthread_join(recieveDataThread,NULL);
	
	
	
	close(connection_fd);
	
}



