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
    
    auto a = knapsack.getSolverName();
    auto b = knapsack.getVersion();
    auto c = knapsack.getVersionString();
    
    knapsack.solve("data/ks_100_0");
    
    
    return 0;
}

