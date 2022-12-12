#include<stdio.h>
#include<stdbool.h>
#include <time.h>

int main(){
    int A[40] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,0, 1, 2, 3, 4, 5, 6, 7, 8, 9,0, 1, 2, 3, 4, 5, 6, 7, 8, 9,0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int B[40] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,};
	int c[40];
	int count = 0;
	clock_t begin = clock();
	for(int i = 0; i < 40; i++) {
  		c[i] = A[i] + B[i];
		count=count+c[i];
	}
	clock_t end = clock();
	printf("%d ",count);
    // for(int i = 0; i < 10; i++) {
  	// 	printf("%d ,", c[i]);
    //     	}
	return 0;
}