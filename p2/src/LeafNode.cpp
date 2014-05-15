//Reed Weichler, Tin Huynh

#include <iostream>
#include <climits>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()

void LeafNode::addToLeft(int value, int last)
{
  leftSibling->insert(values[0]);

  for(int i = 0; i < count - 1; i++)
    values[i] = values[i + 1];

  values[count - 1] = last;
  if(parent)
    parent->resetMinimum(this);
} // LeafNode::ToLeft()

void LeafNode::addToRight(int value, int last)
{
  rightSibling->insert(last);

  if(value == values[0] && parent)
    parent->resetMinimum(this);
} // LeafNode::addToRight()

void LeafNode::addToThis(int value)
{
  int i;

  for(i = count - 1; i >= 0 && values[i] > value; i--)
      values[i + 1] = values[i];

  values[i + 1] = value;
  count++;

  if(value == values[0] && parent)
    parent->resetMinimum(this);
} // LeafNode::addToThis()

bool LeafNode::removeFromThis(int value)
{
    bool shouldReset = value == values[0] && parent;

    int i;
    bool found = false;
    for(i = 0; i < count; i++)
    {
        if(!found)
        {
            if(values[i] == value)
                found = true;
        }
        else
        {
            values[i - 1] = values[i];
        }
    }

    if(found)
    {
        count--;

        if(shouldReset)
           parent->resetMinimum(this);
    }

    return found;
} // LeafNode:removeFromThis()

LeafNode* LeafNode::rotateFromLeft(int value)
{
    int replace = leftSibling->getMaximum();
    leftSibling->remove(replace);

    insert(replace);

    /*
    InternalNode *parentNode = (InternalNode *)parent;
    int i;
    for(i = 0; i < parentNode->getCount(); i++)
    {
        if(parentNode->keys[i] > parentReplace)
        {
            insert(parentNode->keys[i]);
            parentNode->keys[i] = parentReplace;

            break;
        }
    }*/

    return NULL;
}

LeafNode* LeafNode::rotateFromRight(int value)
{
    int replace = rightSibling->getMinimum();
    rightSibling->remove(replace);

    insert(replace);

    /*
    InternalNode *parentNode = (InternalNode *)parent;
    int i;
    for(i = parentNode->getCount() - 1; i >= 0; i--)
    {
        if(parentNode->keys[i] < parentReplace)
        {
            insert(parentNode->keys[i]);
            parentNode->keys[i] = parentReplace;

            break;
        }
    }
    */
    return NULL;
}

void LeafNode::addValue(int value, int &last)
{
  int i;

  if(value > values[count - 1])
    last = value;
  else
  {
    last = values[count - 1];

    for(i = count - 2; i >= 0 && values[i] > value; i--)
      values[i + 1] = values[i];
    // i may end up at -1
    values[i + 1] = value;
  }
} // LeafNode:: addValue()

int LeafNode::getMaximum()const
{
  if(count > 0)  // should always be the case
    return values[count - 1];
  else
    return INT_MAX;
} // getMaximum()


int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()


LeafNode* LeafNode::insert(int value)
{
  int last;

  if(count < leafSize)
  {
    addToThis(value);
    return NULL;
  } // if room for value

  addValue(value, last);

  if(leftSibling && leftSibling->getCount() < leafSize)
  {
    addToLeft(value, last);
    return NULL;
  }
  else // left sibling full or non-existent
    if(rightSibling && rightSibling->getCount() < leafSize)
    {
      addToRight(value, last);
      return NULL;
    }
    else // both siblings full or non-existent
      return split(value, last);
}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()


LeafNode* LeafNode::merge(LeafNode *other)
{
    int i;
    for(i = 0; i < count; i++)
    {
        other->insert(values[i]);
    }
    return other;
}

LeafNode* LeafNode::remove(int value)
{   // To be written by students

    removeFromThis(value);

    int minSize = (leafSize + 1)/2;

    if(count >= minSize)
    {
        return NULL;
    }
    else if(leftSibling)
    {
        if(leftSibling->getCount() > minSize)
            return rotateFromLeft(value);
        else
            return merge((LeafNode *)leftSibling);
    }
    else if(rightSibling)
    {
        if(rightSibling->getCount() > minSize)
            return rotateFromRight(value);
        else
            return merge((LeafNode *)rightSibling);
    }
    else
    {
        return NULL;
    }
    return NULL;
}  // LeafNode::remove()

LeafNode* LeafNode::split(int value, int last)
{
  LeafNode *ptr = new LeafNode(leafSize, parent, this, rightSibling);


  if(rightSibling)
    rightSibling->setLeftSibling(ptr);

  rightSibling = ptr;

  for(int i = (leafSize + 1) / 2; i < leafSize; i++)
    ptr->values[ptr->count++] = values[i];

  ptr->values[ptr->count++] = last;
  count = (leafSize + 1) / 2;

  if(value == values[0] && parent)
    parent->resetMinimum(this);
  return ptr;
} // LeafNode::split()

