#include <ctype.h>
#include <limits.h>
#include "LongInt.h"
#include "StackLi.h"


LongInt::LongInt()
{
  digits.makeEmpty();
}  // LongInt


istream& operator>> (istream &in, LongInt &rhs)
{  // digit are stored in reverse order!!!
  char c = '0';

  rhs.digits.makeEmpty();
  ListItr <char> itr = rhs.digits.zeroth();
  while(isdigit(c))
  {
    in >> c;
    rhs.digits.insert(c - '0', itr);
    c = in.peek();
  } // while c is a digit
  in.ignore(INT_MAX, '\n');
  return in;
}  // operator>>


ostream& operator<< (ostream &out, LongInt &rhs)
{

  if(rhs.digits.isEmpty())
    out << 0;
  else
  {
    ListItr<char> itr = rhs.digits.first();
    StackLi<char> stack;
    while(!itr.isPastEnd())
    {
      stack.push(itr.retrieve());
      itr.advance();
    }
    while(!stack.isEmpty())
      out << (int) stack.topAndPop();
  } // else not empty

  return out;
} // operator<<

LongInt& LongInt::operator+ (LongInt &rhs)
{
  char num = 0;
  static LongInt result;

  result.digits.makeEmpty();
  ListItr<char> itr = digits.first(), itr2 = rhs.digits.first(),
    itr3 = result.digits.zeroth();

  while(!itr.isPastEnd() && !itr2.isPastEnd())
  {
    num += itr.retrieve() + itr2.retrieve();
    result.digits.insert(num % 10, itr3);
    num /= 10;
    itr3.advance();
    itr2.advance();
    itr.advance();
  }

  while(!itr.isPastEnd())
  {
    num += itr.retrieve();
    result.digits.insert(num % 10, itr3);
    num /= 10;
    itr3.advance();
    itr.advance();
  }

  while(!itr2.isPastEnd())
  {
    num += itr2.retrieve();
    result.digits.insert(num % 10, itr3);
    num /= 10;
    itr3.advance();
    itr2.advance();
  }

  if(num > 0){
    result.digits.insert(num, itr3);
    itr3.advance();
  }
  return result;
}  // operator+


LongInt& LongInt::operator= (LongInt &rhs)
{
  if(this == &rhs)
    return *this;

  digits.makeEmpty();
  ListItr <char> itr = digits.zeroth(), itr2 = rhs.digits.first();
  while(!itr2.isPastEnd())
  {
    digits.insert(itr2.retrieve(), itr);
    itr.advance();
    itr2.advance();
  }
  return *this;
}  // operator=
