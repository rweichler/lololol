#include <iostream>
#include <fstream>
#include <iomanip>
#include "BinaryTree.h"
#include "BinaryHeap.h"

using namespace std;

class Huffman
{
  char character;
  int count;
public:
  Huffman(char c = 0, int cnt = 0) : character(c), count(cnt){}
  bool operator< (const Huffman &rhs) const {return count < rhs.count;}
  friend ostream& operator<< (ostream& os, const Huffman &huffman);
  int getCount()const {return count;}
}; // Huffman class

ostream& operator<< (ostream &os, const Huffman &huffman)
{
  os << huffman.character << setw(5) << huffman.count;
  return os;
} // operator<<

class BinaryTreePtr
{
  BinaryTree<Huffman> *ptr;
public:
  BinaryTreePtr(BinaryTree<Huffman> *BTptr = NULL): ptr(BTptr){}
  bool operator< (const BinaryTreePtr &rhs) const {return *ptr < *(rhs.ptr);}
  BinaryTree<Huffman>* getPtr() {return ptr;}
}; // class BinaryTreePtr

int main(int argc, char* argv[])
{
  char character, encoding[256];
  int counts[256];
  BinaryTreePtr first, second;
  BinaryHeap<BinaryTreePtr > heap(256);
  for(int i = 0; i < 256; i++)
    counts[i] = 0;

  ifstream inf(argv[1]);
  while((character = inf.get()) != EOF)
    counts[(int)character]++;

  for(int i = 0; i < 256; i++)
    if(counts[i])
    {
      BinaryTree<Huffman> *tree = new BinaryTree<Huffman>
        (Huffman(char(i), counts[i]), NULL, NULL);
      heap.insert(BinaryTreePtr(tree));
    }

  while(!heap.isEmpty())
  {
    heap.deleteMin(first);
    if(heap.isEmpty())
      break;
    heap.deleteMin(second);
    BinaryTree<Huffman> *tree = new BinaryTree<Huffman>
      (Huffman(char(0), first.getPtr()->getObject().getCount()
        + second.getPtr()->getObject().getCount()), first.getPtr()
        , second.getPtr());
    heap.insert(BinaryTreePtr(tree));

  }

  first.getPtr()->printTree(encoding, 0);

  return 0;
}
//---------------------------------------------------------------------------
