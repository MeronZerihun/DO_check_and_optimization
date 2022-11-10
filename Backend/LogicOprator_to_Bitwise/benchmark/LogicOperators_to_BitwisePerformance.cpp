#include<iostream>
#include<cstdlib>
using namespace std;

int main(){

    int N = 200;
    int A1[N][N], A2[N][N];
    int range_min = -10, range_max=100;

    for(int i=0; i<N; i++){
        for(int j=0;j<N;j++){
            int temp = rand();
            if(temp>range_min && temp<range_max){
                A1[i][j] = temp;
            }
            else{
                A1[i][j] = -1;
            }
            if(temp>range_min || temp<range_max){
                A2[i][j] = temp;
            }
            else{
                A2[i][j] = 1;
            }
        }
    }
    return 0;
}
