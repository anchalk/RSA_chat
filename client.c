#include "mysocket.h"
#include <time.h>
#include<pthread.h>

long *publicKeyServer;
	long *publicKeyClient;
	long *privateKeyClient;
	long *productOfPrimes_C;
	long *productOfPrimes_S;
/*
@param connection_fd - socket fd of the connection
Recieves data from the server
 */

void * reciveData(void *connection_fd) {

	int *file_descriptor = (int*) connection_fd;
	
	int n;
	int i = 0;
	long decrypt1=1;
	long decrypt2 = 0;
	int result;
	while (1) {
	
		result = read(*file_descriptor, &n,sizeof(int));
		if (result <= 0) break;
		i = 0;
		while(i < n){
			
			result = read(*file_descriptor, &decrypt1,sizeof(long));
			if (result <= 0){
				break;
			}
			decrypt2 = decrypt(decrypt1,*privateKeyClient,*productOfPrimes_C);
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
Sends data from the server
 */

void * sendData(void * connection_fd) {

	long * file_descriptor = (long*) connection_fd;

	//long buffer[2048];
	
	int n;
	int i = 0;
	int result;
	long encrypt1[1000];
	
	long ch = 0;
	printf("\n\t\t\t-------------------------\n");
	printf("\t\t\tWelcome to the chat room\n");
	printf("\t\t\t-------------------------\n");
	printf("To quit press \\q or \\Q\n");

	while(1){
		char buffer[1000] = {0};
		while((ch = getchar())!='\n'){
	    		encrypt1[i] = encrypt(ch, *publicKeyServer,*productOfPrimes_S);
			buffer[i] = ch;
			i++;
		}
		if (strcasecmp(buffer,"\\q") == 0){
			break;
		}
		result = write(*file_descriptor, &i, sizeof(i));
		if (result <= 0){
			break;
		}
		n = 0;
		while(n < i){
			printf("encrypt1 = %ld\n",encrypt1[n]);
			result = write(*file_descriptor, &encrypt1[n], sizeof(encrypt1[n]));
			if (result <= 0){
				break;
			}
			n++;
		}
		
		if (result <= 0){
			break;
		}		
		i = 0;
	}
	close(*file_descriptor);
	exit(0);
}


void *receivePublicKeyOfServer(long *publicKeyServer,long *productOfPrimes_S,void *connection_fd){
	int * file_descriptor = (int*) connection_fd;
	int length = sizeof(publicKeyServer);
	int n;
	n = read(*file_descriptor,publicKeyServer,length);
	n = read(*file_descriptor,productOfPrimes_S,length);
	//printf("Public key of server %ld",*publicKeyServer);
	//printf("Public key of server %ld",*productOfPrimes_S);
	printf("Public key of Server ( %ld , %ld)",*publicKeyServer ,*productOfPrimes_S);
}

// sends public key to the server
void *sendPublicKeyOfClient(long *publicKeyClient,long *productOfPrimes_C,void *connection_fd){
	int * file_descriptor = (int*) connection_fd;
	int length = sizeof(publicKeyClient);
	int n;
	n = write(*file_descriptor,publicKeyClient,length);
	n = write(*file_descriptor,productOfPrimes_C,sizeof(productOfPrimes_C));
}


int main(int argc, char** argv) {
	
	unsigned int iseed = (unsigned int) time(NULL);
	srand(iseed);
	int connection_fd;
	char buffer[1024];
	int n;
	long primeNumber1 ,primeNumber2;
	pthread_t recieveDataThread, sendDataThread;
	
	publicKeyServer = (long*)malloc(sizeof(long));
	publicKeyClient = (long*)malloc(sizeof(long));
	privateKeyClient = (long*)malloc(sizeof(long));
	productOfPrimes_C = (long*)malloc(sizeof(long));
	productOfPrimes_S = (long*)malloc(sizeof(long));



	if (argc < 3) {
		printf("Usage: %s port\n", argv[0]);
		exit(0);

	}

	//Generating prime numbers
	primeNumber1 = generateRandomPrimeNum();
	primeNumber2 = generateRandomPrimeNum();
	//generating public and private keys
	generateKeys(primeNumber1,primeNumber2,productOfPrimes_C,publicKeyClient,privateKeyClient);
	//Connecting to the server
	printf("Public key client %ld\n",*publicKeyClient);
	printf("Private key client %ld\n",*privateKeyClient);
	printf("Public client pq %ld\n",*productOfPrimes_C);
	connection_fd = connectTo(argv[2], atoi(argv[1]));
	receivePublicKeyOfServer(publicKeyServer,productOfPrimes_S,&connection_fd);
	sendPublicKeyOfClient(publicKeyClient,productOfPrimes_C,&connection_fd);

	pthread_create(&recieveDataThread, NULL, reciveData, &connection_fd);
	pthread_create(&sendDataThread, NULL, sendData, &connection_fd);

	pthread_join(recieveDataThread, NULL);

	close(connection_fd);

}
