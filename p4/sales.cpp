//Reed Weichler
#include "SaleRunner.h"
#include "sales.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
long long SKU
char name[265]
int price
int onHand
int minimum
*/

//macros for ProductInfo struct so i dont need to do any unneccesary copying
//since an int is 4 bytes, and each value is less than 100 decimal, we
//can safely store numsold in ProductInfo.onHand's 2 most significant bytes
#define ON_HAND(P) ((P)->onHand & 0xFF)
#define SET_ON_HAND(P, NUM) (P)->onHand = ((P)->onHand & 0xFF00) + (NUM)
#define NUM_SOLD(P) ((P)->onHand >> 8)
#define SET_NUM_SOLD(P, NUM) (P)->onHand = ((P)->onHand & 0xFF) + ((NUM) << 8)
#define SELL(P, NUM) (P)->onHand += ((NUM) << 8) - ((NUM) & 0xFF)

typedef ProductInfo Product;

//my only ADT is an array
ProductInfo *products;
int productCount;

int sort_products(const void *a, const void *b)
{
    long long A = ((ProductInfo *)a)->SKU;
    long long B = ((ProductInfo *)b)->SKU;
    long long diff = A - B;
    if(diff < 0) return -1;
    if(diff > 0) return 1;
    return 0;
}

ProductInfo *find_product_step(int min, long long SKU, int max)
{
    if(max < min)
        return NULL;

    int mid = (min + max)/2;
    long long comp = products[mid].SKU;
    if(comp > SKU)
        return find_product_step(min, SKU, mid - 1);
    else if(comp < SKU)
        return find_product_step(mid + 1, SKU, max);
    else
        return &products[mid];
}

ProductInfo *find_product(long long SKU)
{
    return find_product_step(0, SKU, productCount - 1);
}

Sales::Sales(const ProductInfo *productInfo, int count) 
{
    productCount = count;

    products = (ProductInfo *)malloc(count*sizeof(ProductInfo));
    memcpy(products, productInfo, count*sizeof(ProductInfo));

    qsort(products, count, sizeof(ProductInfo), sort_products);

}
#define log printf
void Sales::sale(long long SKU, int numSold, const char **name, int *price)
{
    Product *p = find_product(SKU);
    *name = p->name;
    *price = p->price;
    SELL(p, numSold);
}


void Sales::report(long long SKU, long long SKU2, int *totalSales)
{
    *totalSales = 0;
    Product *current = find_product(SKU);
    Product *last = find_product(SKU2);

    while(current != last)
    {
        *totalSales += NUM_SOLD(current)*(current->price);
        current++;
    }
    *totalSales += NUM_SOLD(last)*(last->price);
}


void Sales::belowMinimums(long long belowMin[], int *belowMinCount)
{
    *belowMinCount = 0;
    for(int i = 0; i < productCount; i++)
    {
        Product *p = &products[i];
        if(ON_HAND(p) < p->minimum)
        {
            belowMin[*belowMinCount] = p->SKU;
            *belowMinCount += 1;
        }
    }
}

