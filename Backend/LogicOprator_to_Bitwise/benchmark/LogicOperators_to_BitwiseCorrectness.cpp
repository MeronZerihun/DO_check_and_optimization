#include<iostream>

using namespace std;

int main(){
    int a = 0,b = -1,c = 10,d = 11;

    bool b1 = a > d;
    bool b2 = c >= b;

    bool final_1 = b1 && b2; 
    bool final_2 = b1 || b2;
    int final_1i = final_1;
    int final_2i = final_2;

    cout<<"Test 1: Logical AND "<<final_1<<"\n";
    cout<<"Test 1: Logical OR "<<final_2<<"\n";
    cout<<"Test 2: Bitwise AND "<<(final_1i & final_2i)<<"\n";
    cout<<"Test 2: Bitwise OR "<<(final_1i | final_2i)<<"\n";



    return 0;
}