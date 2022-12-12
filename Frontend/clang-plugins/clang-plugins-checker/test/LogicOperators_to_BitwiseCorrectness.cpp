#include<stdio.h>
#include<stdbool.h>


int main(){
    int a = 0,b = -1,c = 10,d = 11;
    int x=12,y=0,z=4;
    if((a < d)&&(c >= b)){
        y = x;
    }
    else{
        y = x+1;
    }
    // if((a < d)||(c >= b)){//{&&(b < d)){
    //     y = x;
    // }
    // else{
    //     y = x+1;
    // }
    if((a < d)&(c >= b)){
        y = x;
    }
    else{
        y = x+1;
    }
    if((a < d)|(c >= b)){//{&&(b < d)){
        y = x;
    }
    else{
        y = x+1;
    }
    printf("%d",y);
    return 0;
}