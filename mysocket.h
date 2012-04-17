/*
 * mysocket.h
 *
 *  Created on: Apr 16, 2012
 *      Author: anchu
 */

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

	LOG("hostname=%s\n", host);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server = gethostbyname(host);

	if (server == NULL) {
		LOG(" No such host\n ", NULL);
		exit(0);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr,
			server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))
			< 0) {
		LOG("Error Connecting\n", NULL);
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
		LOG("ERROR opening socket\n", NULL);
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		LOG("ERROR on binding\n", NULL);
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
		LOG("ERROR on accept\n", NULL);
	}
	return newsockfd;
}
