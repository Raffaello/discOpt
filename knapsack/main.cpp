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
    int ret = 0;
    
    ret = CoinInitSolver("knapsack");

    ret = CoinFreeSolver();
    return 0;
}

