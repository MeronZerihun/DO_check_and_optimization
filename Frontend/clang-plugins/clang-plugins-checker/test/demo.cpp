#include "lib/enc_lib.h"
#include <stdio.h>

using namespace enc_lib;

int foo(int x, enc_int baz)
{
    enc_int arr1[3] = {1, 2, 3};
    int arr2[3] = {1, 2, 3};
    int y = baz + 2;
    int z = 0;
    enc_int b = baz + 2;
    for (int i = 0; i < x; i++)
    {
        if (((0 == 0 || baz == 1) || (4 == 4 || baz > 2)) || baz > 3)
        {
            x = baz * 2;
            arr1[z] = 5;
            if (2 == 2)
            {
                baz = 0;
                arr2[baz] = 5;
            }
        }
        if (arr1[z] > 2)
        {
            y = x * 4;
        }
        z++;
    }

    return y;
}

int main()
{
    enc_int r1 = foo(2, 3);
    int r2 = foo(2, 3);
    if (r1 > 3)
    {
        printf("%s\n", "r1: Greater than 3");
    }
    if (r2 < 3)
    {
        printf("%s\n", "r2: Not greater than 3");
    }

    return 0;
}