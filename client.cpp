#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include"mysocket.h"
/**
 * Recieves data from the server
 */
void * reciveData(void *connection_fd){

int * file_descriptor = (int*)connection_fd;
char buffer[1024];

bzero(buffer, 1024);
while(1){

	n = read(file_descriptor, buffer, sizeof(buffer));

	printf("Received: %s\n", buffer);
	 if ((strcmp(buffer , "q") == 0) || strcmp(buffer , "Q") == 0){

		return;
}
	else{

	n = write(file_descriptor, buffer, strlen(buffer));


	}
}

bzero(buffer, 1024);
}

/**
 * send data to the server
 */
void * sendData(void * connection_fd){

int * file_descriptor = (int*)connection_fd;

char send_data[1024];
char buffer[1024];
bzero(buffer, 1024);

while(1){


	n = printf("Type Something (q or Q to quit):");
	gets(send_data);
	bzero(buffer, 1024);
	 if ((strcmp(send_data , "q") == 0) || strcmp(send_data , "Q") == 0){

		return;
	}
	else{


	strncpy(buffer, send_data, strlen(send_data)+1);
	}
	n = write(file_descriptor, buffer, strlen(buffer));

}
bzero(buffer, 1024);
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

	pthread_create(&recieveDataThread,NULL,recieveData,&connection_fd);
	pthread_create(&sendDataThread,NULL,sendData,&connection_fd);

	// pthreadjoin()



	close(connection_fd);

}




