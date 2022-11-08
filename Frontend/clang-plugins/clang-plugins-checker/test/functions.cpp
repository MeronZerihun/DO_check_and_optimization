namespace enc_lib
{
  // typedef struct enc_bool{
  //   bool v;
  //   enc_bool() { v = 0; }
  //   enc_bool(int value) { v = value; }

  // }enc_bool;

  // typedef struct enc_int{
  // int v;

  // enc_int() { v = 0; }
  // enc_int(int value){ v = value; }

  // inline enc_int operator= (int value){
  //   v = value;
  //   return *this;
  // }
  // int decrypt_int(){
  //   return v;
  // }

  // public:
  //   inline friend enc_int operator+(enc_int lhs, int rhs){
  //     return enc_int(lhs.decrypt_int() + rhs);
  //   }
  //   inline friend enc_bool operator>(enc_int lhs, int rhs)
  //   {
  //     return enc_bool(lhs.decrypt_int() > rhs);
  //   }

  // }enc_int;
  typedef int enc_int;

}

using namespace enc_lib;

void foo();

enc_int bar(enc_int baz)
{
  int arr[3] = {1, 2, 3};
  // baz = baz + 2;
  // auto b = baz + 2;
  enc_int b = baz + 2;
  if (((0 == 0 || baz == 1) || (4 == 4 || baz > 2)) || baz > 3)
  {
    baz = baz + 1;
    if (2 == 2)
    {
      baz = 0;
      int a;
      arr[baz] = 5;
      int x = arr[baz + 1];
      a = b + 3;
      if (3 == 3)
      {
        baz = -1;
      }
    }
  }
  else if (baz)
  {
    baz = 4;
  }
  return baz;
}

int main()
{
  // Enc b = 5;
  // bar(b);
  return 0;
}
