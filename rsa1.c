/* code written by Arpit Jain and Anchal Khanna */
/* A few variables used here are like e , d , p , q , n are generally used to denote variables 
related to RSA and hence we have made use of these variable names.*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include<string.h>

long gcd(long a, long b){
//This Function calculates the GCD of 2 numbers and returns the appropriate GCD
    long first = a;
    long second  = b;
    long remainder;
    if(second == 0) return first;//if second number is 0 then return the first number
    if(second > 0){
        while(second != 0){
            remainder = first % second;
            first = second;
            second = remainder;
        }
    }
    return first;
}   

long is_Prime(long a){
//This function checks whether the number is prime or not.
    int i = 0;
    long number = a;
    for(i = 2; i < sqrt(number); i++) {
        if((number % i) == 0){
        //printf("%ld  %d = 0\n",a,i);
            return 0;
            }
    }
    return 1;
}


/*long coprime(long number){
//CoPrime numbers are those numbers which are mutually 	prime even if they are not actually prime. example 13 15 are mutually prime
	long start = 1;
	long e;//e represents coprime number
	do{
		start = start +1;
		e = gcd(start,number);
		//		printf("start = %ld \n e = %ld\n",start,e);	
	}while(e != 1);
	return start;
}*/

long coprime(long n) {
    long random;
    do{
    		random = rand() % n + 2;
    	}while(gcd(n, random) != 1);
    return random;

}


long modulo_inverse(long m, long e){
	long n = -1, moduloInv,d;
	do{
		n = n+1;
		d = 1 +(n*m);
		moduloInv = d/e;
	}while(d%e != 0);
	return moduloInv;
}


long long2BaseTwo(long x, long *booleanArray){
//this function convers long to number to the base 2 i.e binary
	long remainder;
	int i =0;

	if(booleanArray == NULL)
		return 0;
	while(x>1){

		remainder = x%2;
		booleanArray[i] = remainder;
		i++;
		x =x/2;
	}
	if(x == 1){
		booleanArray[i] = x;
	}
	return 1;
}

long modulo(long a,long d,long n){
	int i;
	long *binaryValuesArray ;
	long p;
	long value =1;
	long remainder =1;
	long numOfDigits = 1 + log2(d);
	long power;
	binaryValuesArray = (long*)malloc((numOfDigits)*(sizeof(long)));
	long2BaseTwo(d, binaryValuesArray);
	for(i =0;i<numOfDigits;i++){
		if(binaryValuesArray[i] !=0){
			remainder = a%n;
			p = 0;
			while(p<i){
				power = pow(remainder,2);
				remainder = power % n;
				p += 1;
			}
			value = ((value % n)*(remainder %n))%n;
		}
	} 
	free(binaryValuesArray);
	return value;
}

long encrypt( long input, long e, long c) {
	long retValue;
	retValue = modulo(input, e,c);
	return retValue;
}

long decrypt( long input, long d, long c) {
	long retValue = modulo(input, d, c);
	return retValue;
}

/*this function generates random number uptil the range of 1000 numbers. 
The number is then checked whether its prime or not, if its not prime then 
a new number is generated until we get a prime number*/
long generateRandom(){
	long number;
	start:
		number = rand()%1000+11;
	if(is_Prime(number) == 0){
		goto start;
	}
	else{
		return number;
	}
}

/*This function is used to generate keys */
void generateKeys(long p, long q, long *number12, long *coPrime, long *modInv){
	long number1,number2;
	long n;
	long cop;
	*number12 = p * q;
	number1 = p-1;
	number2 = q-1;
	n = number1 * number2;
	*coPrime = coprime(n);
	cop = coprime(n);
	*modInv = (n,cop);
}
/*This method uses bruteforce to break the encyption. we find 2 prime numbers such that 
there product is same as that of n. if we find such numbers then it means that we can 
crack the encyrpted code*/
long crackRSA(long pq, int public_key){
	long i = 3;
	long count = 1;
	long j = 0;
	long p;
	long q;
	long *primeNumbers = ( long *)malloc(pq * sizeof( long));
	primeNumbers[0] = 2;
	long private_key;
	while(i < pq){
		if(is_Prime(i) == 1){
			count++;
			primeNumbers[count] = i;
			}
		i++;
	}
	
	for(i = 0; i < count ; i++){
		for(j = 0; j < count; j++){
			if(primeNumbers[i] * primeNumbers[j] == pq){
				long n;
				
				p = primeNumbers[i];
				q = primeNumbers[j];
				n = (p-1)*(q-1);
				if (gcd(n, public_key) != 1) continue;		
				private_key = modulo_inverse(n, public_key);
				break;
				}
			}
		}
	free(primeNumbers);
	return private_key;
}

int main(){
	char input[1024];
	long public_key;
	long product;
	long private_key;
	do{
		
		
		printf("Enter product or q to quit:");
		gets(input);
		if (strstr(input, "q") != NULL) break;
		
		sscanf(input, "%ld", &product);
		printf("Enter public key or q to quit:");
		gets(input);
		if (strstr(input, "q") != NULL) break;
		sscanf(input, "%ld", &public_key);
		
		printf("Enter encrypted string separated by ' ' or q to quit:");
		gets(input);
		
		if (strstr(input, "q") != NULL) break;
		printf("Cracking cypher\n");
		private_key = crackRSA(product, public_key);
		printf("Possible private key = %ld\n", private_key);
		char * str = strtok(input, " \n");
		printf("Possible decrypted text:");
		while(str != NULL){
			long encrypt;
			sscanf(str, "%ld", &encrypt);
			putchar(decrypt(encrypt, private_key, product));
			str = strtok(NULL, "\n ");
		}
		printf("\n");
	} while(1);
	return 0;
}	

