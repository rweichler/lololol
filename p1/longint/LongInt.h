//Authors: Reed Weichler, Tin Huynh
#include <iostream>
#include "StackLi.h"
using namespace std;

class LongInt
{
    private:
        StackLi<int> stack;
    public:
        friend ostream &operator<<(ostream &output, LongInt &self)
        {
            StackLi<int> thisList(self.stack);

            while(!thisList.isEmpty())
            {
                output << thisList.topAndPop();
            }
            return output;
        }
        friend istream &operator>>(istream &input, LongInt &self)
        {
            self.stack.makeEmpty();
            while(input.good())
            {
                char c = input.get();
                if(c == '\n' || !input.good()) break;

                int i = c - '0';
                self.stack.push(i);

            }
            return input;
        }
        LongInt operator+(LongInt &other);
};
