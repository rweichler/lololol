// Author: Sean Davis 
// Created on May 6, 2014, 4:34 PM
#define SHOULD_PRINT 0


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "SaleRunner.h"
#include "sales.h"
#include "CPUTimer.h"
using namespace std;

typedef struct
{
  long long SKU;
  char name[265];
  int price;
  int onHand;
  int minimum;
} Product;

typedef struct
{
  char type;
  long long SKU;
  long long SKU2;
  int productNum1;
  int value;
} Transaction;

int readSKUFile(Product products[])
{
  char line[512];
  int count = 0;
  ifstream inf("skus2.txt");
  
  while(inf.getline(line, 512))
  {
    products[count].SKU = strtoll(strtok(line,"\t"), NULL, 10);
    strcpy(products[count++].name, strtok(NULL, "\n"));
  }
  
  return count;
}  // readFile())

void readSalesFile(int **rows2, int *rowCount, int *numTransactions, 
  Transaction **transactions2, long long **belowMins, long long **belowMins2, 
  int *belowMinCount, const char *filename, Product products[], 
  ProductInfo **productInfo)
{
  char dummy, *line = new char[1000000];
  ifstream inf(filename);
  inf >> *rowCount >> dummy >> *numTransactions;
  int *rows = new int[*rowCount]; 
  *belowMins = new long long[*rowCount];
  *belowMins2 = new long long[*rowCount];
  *productInfo = new ProductInfo[*rowCount];
  Transaction *transactions = new Transaction[*numTransactions];
  inf.ignore(100, '\n');
  
  for(int i = 0; i < *rowCount; i++)
  {
    inf.getline(line, 10000);
    (*productInfo)[i].SKU = strtoll(strtok(line, ","), NULL, 10);
    rows[i] = atoi(strtok(NULL, ","));
    strcpy((*productInfo)[i].name, products[rows[i]].name);
    (*productInfo)[i].onHand = products[rows[i]].onHand = atoi(strtok(NULL, ","));
    (*productInfo)[i].minimum = products[rows[i]].minimum = atoi(strtok(NULL, ","));
    (*productInfo)[i].price = products[rows[i]].price = atoi(strtok(NULL, ","));
  }  // for each row
  
  for(int i = 0; i < *numTransactions; i++)
  {
    inf.getline(line, 10000);
    transactions[i].type = line[0];
   
    if(line[0] == 'R')
    {
      transactions[i].SKU = strtoll(strtok(&line[1], ","), NULL, 10);
      transactions[i].SKU2 = strtoll(strtok(NULL, ","), NULL, 10);
      transactions[i].value = atoi(strtok(NULL, ","));
    }  // Query
    else  // Sale
    {
       transactions[i].SKU = strtoll(strtok(&line[1], ","), NULL, 10);
       transactions[i].productNum1 = atoi(strtok(NULL, ","));
       transactions[i].value = atoi(strtok(NULL, ","));
    }
  }  // for each transaction

  inf.getline(line, 1000000);
  *belowMinCount = 0;
  
  
  for(char *ptr = strtok(line, ","); ptr; ptr = strtok(NULL, ","))
    (*belowMins)[(*belowMinCount)++] = strtoll(ptr, NULL, 10);
  
  *transactions2 = transactions;
  *rows2 = rows;
  delete [] line;
}  // readSalesFile()


int main(int argc, char** argv) {
  int numTransactions, *rows, rowCount, belowMinCount, belowMinCount2, price,
    totalSales;
  long long *belowMins, *belowMins2;
  double startTime, endTime;
  const char *ptr;
  Product *products = new Product[594000];
  Transaction *transactions;
  ProductInfo *productInfo;
  readSKUFile(products);
  readSalesFile(&rows, &rowCount, &numTransactions, &transactions, &belowMins,
    &belowMins2, &belowMinCount, argv[1], products, &productInfo);
  startTime = getCPUTime();
  Sales *sales = new Sales(productInfo, rowCount);
  delete [] productInfo;
  
  for(int i = 0; i < numTransactions; i++)
  {
    if(transactions[i].type == 'S')
    {
      sales->sale(transactions[i].SKU, transactions[i].value, &ptr, &price);
      
      if(SHOULD_PRINT && strcmp(ptr, products[transactions[i].productNum1].name))
      cout << "Name error on transaction #" << i << " yours: " << ptr
        << " correct: "  << products[transactions[i].productNum1].name << endl;
      
      if(SHOULD_PRINT && price != products[transactions[i].productNum1].price)
        cout << "Price error on transaction #" << i << " yours: "  << price
          << " correct: "  <<  products[transactions[i].productNum1].price << endl;
    }  // if sale
    else  // Report type
    {
      sales->report(transactions[i].SKU, transactions[i].SKU2, &totalSales);
      
      if(SHOULD_PRINT && totalSales != transactions[i].value)
        cout << "Total sales error on transaction #"  << i << " yours: "
          << totalSales << " correct: "  << transactions[i].value << endl;
    }  // else a report
  } // for each transaction
  
  sales->belowMinimums(belowMins2, &belowMinCount2);
  endTime = getCPUTime();
  
  if(SHOULD_PRINT && belowMinCount != belowMinCount2)
    cout << "Number of minimums error: yours: "  << belowMinCount2
      << " correct: "  << belowMinCount << endl;
  else
    for(int i = 0; i < belowMinCount; i++)
      if(SHOULD_PRINT && belowMins[i] != belowMins2[i])
        cout << "Below minimum mismatch at #"  << i << " yours: " 
          << belowMins2[i] << " correct: "  << belowMins[i] << endl; 
  
  cout << "CPU time: " << endTime - startTime << endl;
  return 0;
} // main())

