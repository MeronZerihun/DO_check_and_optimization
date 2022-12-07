#include<stdio.h>
#include<stdbool.h>
#include<time.h>

int main(){
	clock_t begin = clock();
    int A[40] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,0, 1, 2, 3, 4, 5, 6, 7, 8, 9,0, 1, 2, 3, 4, 5, 6, 7, 8, 9,0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int B[40] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,};
	int c[40];
	
	for(int i = 0; i < 40; i++) {
  		c[i] = A[i] + B[i];
	}
  clock_t end = clock();
	printf("%f ", (double)(end - begin) / CLOCKS_PER_SEC);  
	return 0;
}