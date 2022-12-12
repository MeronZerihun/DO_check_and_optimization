#include "lib/enc_lib.h"

using namespace enc_lib;

void foo();

// enc_int bar(enc_int baz)
// {
//   int arr[3] = {1, 2, 3};
//   // baz = baz + 2;
//   // auto b = baz + 2;
//   enc_int b = baz + 2;
//   if (((0 == 0 || baz == 1) || (4 == 4 || baz > 2)) || baz > 3)
//   {
//     baz = baz + 1;
//     if (2 == 2)
//     {
//       baz = 0;
//       int a;
//       arr[baz] = 5;
//       int x = arr[baz + 1];
//       a = b + 3;
//       if (3 == 3)
//       {
//         baz = -1;
//       }
//     }
//   }
//   else if (baz)
//   {
//     baz = 4;
//   }
//   return baz;
// }

int bar1(enc_int baz)
{
  // Test 1 Expected- IF: NOT OBLIVIOUS {x, baz, x, bar1, w}
  // int x = 2 + 2;
  // x = x + 1 + baz;
  // if (x)
  //   x = x + 3;
  // return x;

  // Test 2 Expected- IF: NOT OBLIVIOUS {baz, y, x, w, bar1}
  // int x = baz + 2;
  // enc_int y = baz + 1;
  // if (x)
  // {
  //   y = y + 1;
  // }
  // return y;

  // Test 3 Expected- IF: NOT OBLIVIOUS {baz, y, x, w, bar1}
  // int y = baz;
  // if (baz)
  // {
  //   y = y + 1;
  // }
  // return y;

  // Test 4 Expected- IF: NOT OBLIVIOUS {baz, y, x, w, bar1}
  // int y = 0;
  // y = baz;
  // if (baz)
  // {
  //   y = y + 1;
  // }
  // return y;

  // Test 5 Expected- IF: NOT OBLIVIOUS {baz, x}
  // int y = 0;
  // if (baz)
  // {
  //   y = y + 1;
  // }
  // return y;

  // Test 6 Expected- IF: NOT OBLIVIOUS {baz, y, x, w, bar1}
  // int y = baz;
  // if (((0 == 0 || baz == 1) || (4 == 4 || baz > 2)) || baz > 3)
  // {
  //   y = y + 1;
  // }
  // return y;

  // Test 7 Expected- Nothing (Is oblivious) {baz, x}
  // int y = 0;
  // if (y)
  // {
  //   y = y + 1;
  // }
  // return y;

  // Test 8 Expected- Nothing (Is oblivious) {baz, y, x, w, bar1}
  // int y = 0;
  // for (int i = 0; i < 1; i++)
  // {
  //   y = baz;
  // }
  // return y;

  // Test 9 Expected- Nothing (Is oblivious) {baz, y, x, w, bar1}
  // int y = baz;
  // for (int i = 0; i < 4; i++)
  // {
  //   y = baz;
  // }
  // return y;

  // Test 10 Expected- FOR: NOT OBLIVIOUS {baz, y, x, w, bar1} -- Check
  // int y = baz;
  // for (int i = 0; i < y; i++)
  // {
  //   y = baz;
  // }
  // return y;

  // Test 11 Expected- FOR: NOT OBLIVIOUS {baz, y, x, w, bar1}
  // int y = 0;
  // for (int i = 0; i < y; i++)
  // {
  //   y = baz;
  // }
  // return y;

  // Test 12 Expected- FOR: NOT OBLIVIOUS {baz, i, y, x, w, bar1}
  // int y = 0 + 1;
  // for (int i = 0; i < baz; i++)
  // {
  //   y = baz;
  // }
  // return y;

  // Test 13 Expected- IF: NOT OBLIVIOUS ARR: NOT OBLIVIOUS { baz, y, b, x, w, bar1}
  // int arr[3] = {1, 2, 3};
  // baz = baz + 2;
  // int y = baz;
  // enc_int b = baz + 2;
  // if (((0 == 0 || baz == 1) || (4 == 4 || baz > 2)) || baz > 3)
  // {
  //   baz = baz + 1;
  //   if (2 == 2)
  //   {
  //     baz = 0;
  //     arr[baz] = 5;
  //   }
  // }
  // return baz;

  // Test 14 Expected- IF: NOT OBLIVIOUS {baz, b, x, w, bar1}
  // int arr[3] = {1, 2, 3};
  // int a = 1;
  // int y = a;
  // int z = a + y;
  // baz = baz + 2;
  // enc_int b = baz + 2;
  // if (((0 == 0 || baz == 1) || (4 == 4 || baz > 2)) || baz > 3)
  // {
  //   baz = baz + 1;
  //   if (2 == 2)
  //   {
  //     baz = 0;
  //     arr[y] = 5;
  //   }
  // }
  // return baz;

  // Test 15 Expected- ARR: NOT OBLIVIOUS {baz, y, x, bar1, w}
  // int arr[3] = {1, 2, 3};
  // int y = 2;
  // if ((0 == 0))
  // {
  //   y = baz + y;
  //   if (2 == 2)
  //   {
  //     arr[y] = 5;
  //   }
  // }
  // return baz;

  //  TEST 16: Expected- ARR: NOT OBLIVIOUS {baz, y, x, w}
  // int arr[3] = {1, 2, 3};
  // int y = 2;
  // if ((0 == 0))
  // {
  //   y = baz + y;
  //   if (2 == 2)
  //   {
  //     arr[baz] = 5;
  //   }
  // }
  // return y;

  // TEST 17: Expected- Nothing (Is Oblivious) {baz, arr, x}
  // enc_int arr[3] = {1, 2, 3};
  // int y = 2;
  // if ((0 == 0))
  // {
  //   if (2 == 2)
  //   {
  //     arr[y] = 5;
  //   }
  // }
  // return y;

  // TEST 18: Expected- Nothing (Is Oblivious) {baz, arr, x}
  // enc_int arr[3] = {1, 2, 3};
  // int y = 2;
  // if ((0 == 0))
  // {
  //   if (2 == 2)
  //   {
  //     arr[y - 1] = 5;
  //   }
  // }
  // return y;

  // TEST 19: Expected- ARR: NOT OBLIVIOUS {baz, arr, x}
  // enc_int arr[3] = {1, 2, 3};
  // int y = 2;
  // if ((0 == 0))
  // {
  //   if (2 == 2)
  //   {
  //     arr[baz + y] = 5;
  //   }
  // }
  // return y;

  // Test 20 Expected- FOR: NOT OBLIVIOUS {baz, i, y, x, w, bar1}
  // int y = 0;
  // for (int i = y; i < 1; i++)
  // {
  //   y = baz;
  // }
  // return y;

  // Test 21 Expected- FOR: NOT OBLIVIOUS {baz, i, y, x, w, bar1}
  // int y = 0;
  // for (int i = 0; i < 1; i += y)
  // {
  //   y = baz;
  // }
  // return y;

  // Test 22 Expected- ARRAY: NOT OBLIVIOUS
  // int y = baz;
  // int arr[y];
  // return baz;

  // Test 23 Expected- IF: NOT OBLIVIOUS {baz, y, x, w, bar1}
  int y = baz;
  // bool cond = baz == 1;
  // if (cond)
  // {
  //   y = y + 1;
  // }
  return y;
}

// Test 1: non-DO
// void check(enc_int secret)
// {
//   enc_int A[10];
//   enc_int i;
//   for (i = 0; i < 10; i++)
//   {
//     A[i] = i * 2;
//   }

//   for (i = 0; i < 10; i++)
//   {

//     if (i == secret)
//       A[i] = 1;
//   }
// }

// Test 2: non-DO
// void check(enc_int secret)
// {
//   enc_int A[10];
//   enc_int i;
//   for (i = 0; i < 10; i++)
//   {
//     A[i] = i * 2;
//   }

//   for (i = 0; i < secret; i++)
//   {
//     A[i] = 1;
//   }
// }

// Test 3: non-DO
// void check(enc_int secret)
// {
//   enc_int A[10];
//   enc_int i;
//   for (i = 0; i < 10; i++)
//   {
//     A[i] = i * 2;
//   }

//   for (i = 0; i < 10; i++)
//   {
//     if (i == secret)
//     {
//       A[i] = 1;
//     }
//   }
// }

// Test 4: non-DO (check for int i )
// void check(enc_int secret)
// {
//   enc_int A[10];
//   enc_int B[10];
//   enc_int i;
//   for (i = 0; i < 10; i++)
//   {
//     A[i] = i * 2;
//     B[i] = i * 2;
//   }

//   for (i = 0; i < 10; i++)
//   {
//     if (i == secret)
//     {
//       A[i] = 1;
//     }
//     else
//     {
//       A[i] = 1;
//     }
//   }
// }

// Test 5: non-DO
// int bar1(enc_int z, int x)
// {
//   int a = x + z;
//   if (a > x)
//   {
//     return a;
//   }
//   else
//   {
//     return 0;
//   }
// }

// Test 6: non-DO
// int bar2(enc_int z, int x)
// {
//   int a = x + z;
//   if (x > 0)
//   {
//     a = CMOV(true, 0, 1);
//     return a;
//   }
//   else
//   {
//     if (z > 10)
//     {
//       return z;
//     }
//     else
//     {
//       return 0;
//     }
//   }
// }

// Test 7: Non-DO
// int bar3(enc_int z, int x)
// {
//   int g[3];
//   enc_int b[z]; // Reject such declarations
//   for (int i = 0; i < sizeof(b); i++)
//   {
//     int a = x + z;
//     if (x > 0)
//     {
//       return a;
//     }
//     else
//     {
//       return z;
//     }
//   }
//   return 0;
// }

// Test 8: Non-DO
// int bar3(enc_int z, int x)
// {
//   int g[3];
//   int b[z]; // Reject such declarations
//   for (int i = 0; i < sizeof(b); i++)
//   {
//     int a = x + z;
//     if (x > 0)
//     {
//       return a;
//     }
//     else
//     {
//       return z;
//     }
//   }
//   return 0;
// }

// Test 9: DO
// int bar1(enc_int z, int x)
// {
//   int a = x + z;
//   if (x == 0)
//   {
//     return a;
//   }
//   else
//   {
//     return 0;
//   }
// }

// Test 10: Non-DO
// int bar1(enc_int z, int x)
// {
//   int a = x + z;
//   enc_bool cond = (x == 0);
//   if (cond)
//   {
//     return a;
//   }
//   else
//   {
//     return 0;
//   }
// }

// Test 11: Non-DO
int bar1(enc_int z, int x)
{
  int a = x + z;
  bool cond = (z == 0);
  if (cond)
  {
    return a;
  }
  else
  {
    return 0;
  }
}

int main()
{
  // Enc b = 5;
  // bar(b);
  // enc_int x = bar1(3);
  // int w = bar1(4);
  // if (w)
  // {
  //   /* code */
  // }

  // int s2 = bar1(2);

  // check(5);

  int s1 = bar1(2, 3);
  // int s4 = bar2(2, 3);
  // int s3 = bar3(2, 3);

  return 0;
}
