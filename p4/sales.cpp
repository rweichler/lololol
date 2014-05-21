#include "SaleRunner.h"
#include "sales.h"


Sales::Sales(const ProductInfo *productInfo, int productCount) 
{
  for(int i = 0; i< productCount; i++)
    tree.insert(MyProduct(productInfo[i]));
 }  // Sales())


void Sales::sale(long long SKU, int numSold, const char **name, int *price)
{
  
} // sale())


 void Sales::report(long long SKU, long long SKU2, int *totalSales)
 {
   
 }  // report())
 
 
 void Sales::belowMinimums(long long belowMin[], int *belowMinCount)
 {
   
 }  // belowMinimums())

