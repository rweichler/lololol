#ifndef LongIntH
#define LongIntH

#include <iostream>
#include "LinkedList.h"
using namespace std;

class LongInt
{
  List <char> digits;
public:
  LongInt();
  friend istream& operator>> (istream &in, LongInt &rhs);
  friend ostream& operator<< (ostream &out, LongInt &rhs);
  LongInt& operator+ (LongInt &rhs);
  LongInt& operator= (LongInt &rhs);
}; // class LongInt

#endif
