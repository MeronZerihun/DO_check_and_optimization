#include <stdio.h>
#include <stdlib.h>

int main() {
	int A[1000];
	int B[1000];
	int C[1000];
	int i, j;
	for(i = 0; i < 1000; i++){
		A[i] = i * 2;
		B[i] = 0;
		C[i] = i * 1;
	}
	srand(2);

	j = 5;
	for(i = 0; i < 1000000000; i++) {
  		int temp = (C[j] * 3 + 3948) / 27;
  		int temp2 = temp * (temp / 93) * 2;
  		B[i % 1000] = (28* A[j] / 3) * (temp2 / 45) * 4+ 279 + i;
  		if(i % 100000000 == 0) 
  			j = rand() % 1000;
	}

	for(i = 0; i < 1000; i++)
		printf("%d\n", B[i]);
	
	return 0;
}
