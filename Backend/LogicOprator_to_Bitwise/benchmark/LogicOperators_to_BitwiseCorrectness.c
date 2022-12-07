#include<stdio.h>
#include<stdbool.h>


int main(){
    int a = 0,b = -1,c = 10,d = 11;

    bool b1 = a > d;
    bool b2 = c >= b;
    bool c2 = b < d;

    bool final_1 = b1 && b2 && c2; 
    bool final_2 = b1 || b2 && c2;
    int final_1i = final_1;
    int final_2i = final_2;

   printf("%dTest 1: Logical AND \n",final_1);
    printf("%dTest 1: Logical OR \n",final_2);
    printf("%dTest 2: Bitwise AND \n",(final_1i & final_2i));
    printf("%dTest 2: Bitwise OR \n",(final_1i | final_2i));



    return 0;
}