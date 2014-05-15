//Authors: Reed Weichler, Tin Huynh

#include "LongInt.h"

LongInt LongInt::operator+(LongInt &other)
{
    LongInt result;

    int carry = 0;

    StackLi<int> thisList(stack);
    StackLi<int> otherList(other.stack);

    while(true)
    {
        bool firstEmpty = thisList.isEmpty();
        bool secondEmpty = otherList.isEmpty();

        int num = carry;

        if(!firstEmpty)
            num += thisList.topAndPop();

        if(!secondEmpty)
            num += otherList.topAndPop();

        result.stack.push(num % 10);
        carry = num / 10;

        if(firstEmpty && secondEmpty)
            break;

    }

    while(result.stack.top() == 0)
        result.stack.pop();

    return result;
}
