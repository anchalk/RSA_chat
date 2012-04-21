#include "mysocket.h"
#include <time.h>
#include<pthread.h>

	long *publicKeyServer;
	long *privateKeyServer;
	long *publicKeyClient;
	long *productOfPrimes_C;
	long *productOfPrimes_S ;
/*
/*
@param connection_fd - socket fd of the connection
Recieves data from the client
 */
void * reciveData(void *connection_fd) {

	int *file_descriptor = (int*) connection_fd;
	int result;
	int n;
	int i = 0;
	long decrypt1=1;
	long decrypt2 = 0;
	while (1) {
	
		result = read(*file_descriptor, &n,sizeof(int));
		if(result <= 0){
		break;
		}
		i = 0;
		while(i < n){
			
			result = read(*file_descriptor, &decrypt1,sizeof(long));
			if(result <= 0){
		break;
		}
			printf("%ld\n", *productOfPrimes_S);
			decrypt2 = decrypt(decrypt1,*privateKeyServer,*productOfPrimes_S);
			putchar(decrypt2);
			i++;
		}
		printf("\n");
	}
	close(*file_descriptor);
	exit(0);
	
}

/*
@param connection_fd - socket fd of the connection
Send data from the client
 */
void * sendData(void * connection_fd) {

	long * file_descriptor = (long*) connection_fd;

	int n;
	int i = 0
	int value = 0;
	long encrypt1[1000];
	long ch = 0;
	int result = 0;
	printf("\n\t\t\t-------------------------\n");
	printf("\t\t\tWelcome to the chat room\n");
	printf("\t\t\t-------------------------\n");
	printf("To quit press q or Q\n");
	while(1){
		char buffer[1000] = {0};
		
		while((ch = getchar())!='\n'){
	    		encrypt1[i] = encrypt(ch, *publicKeyClient,*productOfPrimes_C);
			i++;
			buffer[i] = ch;
		}
		if (strcasecmp(buffer,"\\q") == 0){
			break;
		}
		result = write(*file_descriptor, &i, sizeof(i));
		if (result <= 0) break;
		n = 0;
		while(n < i){
			printf("encrypt1 = %ld\n",encrypt1[n]);
			result = write(*file_descriptor, &encrypt1[n], sizeof(encrypt1[n]));
			if (result <= 0){
				break;
			}
			n++;
		}
		if (result <= 0) {
			break;
		}
		i = 0;
		
	}
	close(*file_descriptor);
}	

// sends public key to the client
void *sendPublicKeyOfServer(long *publicKeyServer,long *productOfPrimes_S,void *connection_fd){
	int * file_descriptor = (int*) connection_fd;
	int length = sizeof(publicKeyServer);
	int n;
	n = write(*file_descriptor,publicKeyServer,length);
	n = write(*file_descriptor,productOfPrimes_S,sizeof(productOfPrimes_S));
}

void *receivePublicKeyOfClient(long *publicKeyClient,long *productOfPrimes_C,void *connection_fd){
	int * file_descriptor = (int*) connection_fd;
	int length = sizeof(publicKeyClient);
	int n;
	n = read(*file_descriptor,publicKeyClient,length);
	n = read(*file_descriptor,productOfPrimes_C,length);
	//printf("Public key of server %ld",*publicKeyClient);
	//printf("Public key of server %ld",*productOfPrimes_C);
	printf("Public key of Client ( %ld , %ld)",*publicKeyClient ,*productOfPrimes_C);
}

int main(int argc, char** argv) {

	unsigned int iseed = (unsigned int) time(NULL);
	srand(iseed);
	int srvfd, connection_fd;
	char buffer[2048];
	long primeNumber1 ,primeNumber2;
	pthread_t recieveDataThread, sendDataThread;


	publicKeyServer = (long*)malloc(sizeof(long));
	privateKeyServer = (long*)malloc(sizeof(long));
	publicKeyClient = (long*)malloc(sizeof(long));
	productOfPrimes_C = (long*)malloc(sizeof(long));
	productOfPrimes_S = (long*)malloc(sizeof(long));


	if (argc < 2) {
		printf("Usage: %s port\n", argv[0]);
		exit(0);
	}
	//Generating prime numbers
	primeNumber1 = generateRandomPrimeNum();
	primeNumber2 = generateRandomPrimeNum();
	//generating public and private keys
	generateKeys(primeNumber1,primeNumber2,productOfPrimes_S,publicKeyServer,privateKeyServer);
	printf("Public server key %ld\n",*publicKeyServer);
	printf("Private server key %ld\n",*privateKeyServer);
	printf("Public server pq %ld\n",*productOfPrimes_S);
	//Making a connection
	srvfd = makeListener(atoi(argv[1]));
	connection_fd = listenFor(srvfd);

	//sending public key to the client
	sendPublicKeyOfServer(publicKeyServer,productOfPrimes_S, &connection_fd);
	receivePublicKeyOfClient(publicKeyClient,productOfPrimes_C, &connection_fd);
	//creating threads for asynchronous communication
	pthread_create(&recieveDataThread, NULL, reciveData, &connection_fd);
	pthread_create(&sendDataThread, NULL, sendData, &connection_fd);

	pthread_join(recieveDataThread, NULL);

	//closing all the connetions
	close(connection_fd);
	close(srvfd);
}
