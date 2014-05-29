//Reed Weichler
 
#ifndef SALES_H
#define	SALES_H

#include "SaleRunner.h"

class Sales {
public:
  Sales(const ProductInfo *productInfo, int productCount);
  void sale(long long SKU, int numSold, const char **name, int *price);
  void report(long long SKU, long long SKU2, int *totalSales);
  void belowMinimums(long long belowMin[], int *belowMinCount);
};

#endif	/* SALES_H */

