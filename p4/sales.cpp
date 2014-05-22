//Reed Weichler
#include "SaleRunner.h"
#include "sales.h"
#include <stdio.h>
#include <stdlib.h>
#include "AvlTree.h"

ProductInfo *products;
int productCount;

Sales::Sales(ProductInfoPtr &productInfo, int count) 
{
    products = productInfo;
    productCount = count;
    productInfo = (ProductInfoPtr)malloc(0);

    printf("%lu\n", sizeof(products[0]));

 }  // Sales())


void Sales::sale(long long SKU, int numSold, const char **name, int *price)
{
}


 void Sales::report(long long SKU, long long SKU2, int *totalSales)
 {
 }


 void Sales::belowMinimums(long long belowMin[], int *belowMinCount)
 {
 }

