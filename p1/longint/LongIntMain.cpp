#include <iostream>
#include "LongInt.h"

using namespace std;

int main()
{
  LongInt int1, int2, int3;

  cout << "Please enter first long integer >> ";
  cin >> int1;
  cout << "Please enter second long integer >> ";
  cin >> int2;

  int3 = int1 + int2; 
  cout << int3 << endl;

  cout << "Original 1st integer: " << int1 << endl;
  cout << "Original 2nd integer: " << int2 << endl;

  return 0;
}
 
