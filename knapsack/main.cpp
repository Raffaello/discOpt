/* 
 * File:   main.cpp
 * Author: raffa
 *
 * Created on March 8, 2015, 4:57 PM
 */


#include "Knapsack.h"
#include "coin/CoinMP.h"
#include <iostream>

/*
 * 
 */
int main(int argc, char** argv) 
{
    using std::endl;
    using std::cout;
    
    Knapsack knapsack;
    
   if(knapsack.solve("data/ks_100_0") == false)
   {
       cout << "error solve : " << argv[1] << endl;
       return -1;
   }
    
    cout << "Solution: " << endl;
    
    return 0;
}

