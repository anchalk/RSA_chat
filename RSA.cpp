#include<stdio.h>
#include<conio.h>


int euclid(int num, int denom) {
	int denominator, numerator, remainder;
	denominator = denom;
	numerator = num;

	while (denominator != 0) {
		remainder = numerator % denominator;
		numerator = denominator;
		denominator = remainder;
	}

}

int coprime(int array[]) {

	int array_length, i, j;
	array_length = sizeof(array) / 4;
	for (i = 0; i < array_length; i++) {
		for (j = 0; j < array_length; j++) {
			// checking whether all the elements in the array are coprime or not
			if ((i != j) && (euclid(array[i], array[j]) != 1)) {
				return 0; // elements are not coprime
			}
		}
	}
	return 1; // all the elements are coprime to each other

}

int extendedEuclid(int num,int denom){
	// Find out the greater and smaller of the two numbers
	int numerator = num;
	int denominator = denom;
	int quotient,remainder;
	if(denominator == 0){
		//?
	}
	else{
		quotient = numerator / denominator;
		remainder = numerator % denominator;

	}
}

/*

 def extendedEuclid(a,b):
 #Find out the greater and smaller number of the two numbers
 if b == 0:
 return (a,1,0)
 else:
 q = a//b
 r = a%b
 (gcd,s, t) = extendedEuclid(b,r)
 return ( gcd,t,s - q * t,)


 def modInv(a,m):
 # checking whether the inverse of a exists in mod m or not
 if (euclid(a,m)==1):
 print(a,"has a multiplicative inverse in mod",m)
 else:
 return 0

 (gcd,c,d)=extendedEuclid(a,m)
 if (c < 0):
 c = c % m
 return c

 def modInv2(a,m):
 # checking whether the inverse of a exists in mod m or not
 if (euclid(a,m)==1):
 print(a,"has a multiplicative inverse in mod",m)
 else:
 return 0

 (gcd,c,d)=extendedEuclid(a,m)
 return c


 def crt(L=[]):
 N = 1
 divisorList=[]
 remainderList=[]
 length = len(L)

 for i in range (0, length):
 divisorList.append(L[i][1]) # Creates a list of divisors from the list of tuples
 remainderList.append(L[i][0])#Creates a list of remainders from the list of tuples

 #checking whether all the elements in the divisor list are coprime or not
 result= coprime(divisorList)

 if(result==False):
 print("Error!")
 return -1
 else:
 print("Numbers are coprime")

 # Finding the value of N that is,the product of all divisors
 for j in range(0,length):
 N = N * divisorList[j]
 x = 0

 for i in range( 0, length):
 x = x+remainderList[i]*(N//divisorList[i])* modInv2(N//divisorList[i],divisorList[i])

 if (x < 0):
 x = x % N
 return x
 */
