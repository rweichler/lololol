
#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include "CPUTimer.h"
#include "statrunner.h"
#include "stats.h"

using namespace std;



typedef struct
{
  char type;
  int parameters[2];
  int returnedValues[10];
  
}Operation;

enum {UPDATE, QUERY};


int readFile(const char* filename, char teams[31][4], Player players[1200],
   Operation operations[500000])
{
  ifstream inf(filename);
  int count = 0;
  char line[80], *ptr;
  for(int i = 0; i < 30; i++)
    inf >> teams[i];
  
  strcpy(teams[30], "MLB");
  inf.ignore(100, '\n');
  
  for(int i = 0; i < 1200; i++)
  {
    inf.getline(line, 80);
    strcpy(players[i].name, strtok(line, ","));
    strcpy(players[i].team, strtok(NULL, ","));
  }  // for i
  
  while(inf.getline(line, 80))
  {
    ptr = strtok(line, ",");  // operation number discarded
    ptr = strtok(NULL, ",");
    
    if(*ptr == 'Q')
    {
      operations[count].type = QUERY;
      operations[count].parameters[0] = atoi(ptr + 1);
      
      for(int i = 0; i < 10; i++)
      {
        ptr = strtok(NULL, ",");
        if(strcmp(ptr, "tie") == 0)
          operations[count].returnedValues[i] = -1;
        else
          operations[count].returnedValues[i] = atoi(ptr);
      }
    } // if query
    else // update
    {
       operations[count].parameters[0] = atoi(ptr);
       operations[count].parameters[1] = atoi(strtok(NULL, ","));
    } // else
    
    count++;
  } // while more in file
  
  return count;
}  //readFile())
 

int main(int argc, char** argv) {
  int count;
  char teams[31][4];
  double startTime, endTime;
  Player players[1200], top10[10];
  Operation *operations = new Operation[5000000];
  count = readFile(argv[1], teams, players, operations);
  startTime = getCPUTime();
  Stats *stats = new Stats();
  
  for(int i = 0; i < 10; i++)
    top10[i].name[0] = top10[i].team[0] = '\0';
  
  for(int i = 0; i < count; i++)
    if(operations[i].type == UPDATE)
      stats->update((const char*) players[operations[i].parameters[0]].name, 
        (const char*) players[operations[i].parameters[0]].team, 
        operations[i].parameters[1], i);
    else // QUERY
    {
      stats->query((const char*)teams[operations[i].parameters[0]], top10, i);
      
      for(int j = 0; j < 10; j++)
        if(operations[i].returnedValues[j] != -1 
          && (strcmp(players[operations[i].returnedValues[j]].name, top10[j].name)
          || strcmp(players[operations[i].returnedValues[j]].team, top10[j].team)))
          cout << "Bad list for operation "  << i << " position " << j
            << " was " << top10[j].name << ' ' << top10[j].team << " should be " 
            << players[operations[i].returnedValues[j]].name << ' '
            << players[operations[i].returnedValues[j]].team << endl;
    } // else QUERY
  
  endTime = getCPUTime();
  cout << "CPU time: " << endTime - startTime << endl;
  return 0;
}  //main())

