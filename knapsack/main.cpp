/* 
 * File:   main.cpp
 * Author: raffa
 *
 * Created on March 8, 2015, 4:57 PM
 */


#include "Knapsack.h"
#include "coin/CoinMP.h"


/*
 * 
 */
int main(int argc, char** argv) 
{
    Knapsack knapsack;
    
   if(knapsack.solve("data/ks_100_0") == false)
       return -1;
    
    return 0;
}

