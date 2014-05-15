//Authors: Reed Weichler, Tin Huynh

#include <iostream>
#include <fstream>
#include <string>
#include "CPUTimer.h"
#include "LinkedList.h"
#include "StackAr.h"
#include "CursorList.h"
#include "QueueAr.h"
#include "StackLi.h"
#include "vector.h"
#include "SkipList.h"

#define DELETE 0
#define REMOVE DELETE
#define INSERT 1

#define log(x, ...) //printf(x, ## __VA_ARGS__)

using namespace std;
vector<CursorNode <int> > cursorSpace(250000);

int getChoice()
{
    int choice;
    
    cout << "\n    ADT Menu\n"
         << "0. Quit\n"
         << "1. LinkedList\n"
         << "2. CursorList\n"
         << "3. StackAr\n"
         << "4. StackLi\n"
         << "5. QueueAr\n"
         << "6. SkipList\n"
         << "Your choice >> ";

    cin >> choice;
    return choice;
}

string readFile(char *name)
{
    ifstream input(name);
    string out;
    getline(input, out);
    getline(input, out);
    return out;
}

bool getNext(string *contents, int *operation, int *num, unsigned int *i)
{
    if(*i >= contents->length()) return false;

    char c = contents->at(*i);
    if (c == 'i')
    {
        *operation = INSERT;
    }
    else if(c == 'd')
    {
        *operation = DELETE;
    }
    *i = *i + 1;

    *num = 0;
    log("Read: ");
    while(*i < contents->length() && contents->at(*i) != ' ')
    {
        log("%c", contents->at(*i));
        *num = (*num) * 10 + contents->at(*i) - '0';
        *i = *i + 1;
    }
    *i = *i + 1;
    log("\n");

    return *i < contents->length();
}

void RunList(char *filename)
{
    List<int> list;

    string contents = readFile(filename);
    int operation;
    int num;
    unsigned int i = 0;
    while(getNext(&contents, &operation, &num, &i))
    {
        if(operation == INSERT)
        {
            list.insert(num, list.zeroth());
            log("    [INSERT ");
        }
        else if(operation == DELETE)
        {
            list.remove(num);
            log("    [DELETE ");
        }
        log("%d]\n", num);
    }

}

void RunCursorList(char *filename)
{
    CursorList<int> cursorList(cursorSpace);

    string contents = readFile(filename);
    int operation;
    int num;
    unsigned int i = 0;
    while(getNext(&contents, &operation, &num, &i))
    {
        if(operation == INSERT)
        {
            cursorList.insert(num, cursorList.zeroth());
        }
        else if(operation == DELETE)
        {
            cursorList.remove(num);
        }
    }
}

void RunStackAr(char *filename)
{
    StackAr<int> stackAr(250000);

    string contents = readFile(filename);
    int operation;
    int num;
    unsigned int i = 0;
    while(getNext(&contents, &operation, &num, &i))
    {
        if(operation == INSERT)
        {
            stackAr.push(num);
        }
        else if(operation == DELETE)
        {
            stackAr.pop();
        }
    }
}

void RunStackLi(char *filename)
{
    StackLi<int> stackLi;

    string contents = readFile(filename);
    int operation;
    int num;
    unsigned int i = 0;
    while(getNext(&contents, &operation, &num, &i))
    {
        if(operation == INSERT)
        {
            stackLi.push(num);
        }
        else if(operation == DELETE)
        {
            stackLi.pop();
        }
    }

}

void RunQueueAr(char *filename)
{
    Queue<int> queueAr(250000);

    string contents = readFile(filename);
    int operation;
    int num;
    unsigned int i = 0;
    while(getNext(&contents, &operation, &num, &i))
    {
        if(operation == INSERT)
        {
            queueAr.enqueue(num);
        }
        else if(operation == DELETE)
        {
            queueAr.dequeue();
        }
    }
}

void RunSkipList(char* filename)
{
   SkipList<int> skipList(0, 250000);

    string contents = readFile(filename);
    int operation;
    int num;
    unsigned int i = 0;
    while(getNext(&contents, &operation, &num, &i))
    {
        if(operation == INSERT)
        {
           skipList.insert(num);
        }
        else if(operation == DELETE)
        {
            skipList.deleteNode(num);
        }
    }
}

int main(int argc, char **argv){
    cout << "Filename >> ";
    char filename[60];
    cin >> filename;
    int choice;
    CPUTimer ct;
    do{
        choice = getChoice();
        ct.reset();
        switch (choice){
                case 1: RunList(filename); break;
                case 2: RunCursorList(filename); break;
                case 3: RunStackAr(filename); break;
                case 4: RunStackLi(filename); break;
                case 5: RunQueueAr(filename); break;
                case 6: RunSkipList(filename) ; break;
        }

        cout <<"CPU time: " << ct.cur_CPUTime() << endl;
    } while(choice > 0);

    return 0;
}
