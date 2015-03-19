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
    if(argc != 2)
    {
        cout << "argument [filename] missing";
        return -1;
    }
    
   if(knapsack.solve(argv[1]) == false)
   {
       cout << "error solve : " << argv[1] << endl;
       return -1;
   }
    
    //cout << "Solution: " << endl;
    //knapsack.writeSolution();
    //knapsack.writeProblem();
    knapsack.writeOutput();
    return 0;
}

