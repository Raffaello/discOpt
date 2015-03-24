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
        cout << "knapsack [filename] [file] missing" << endl;
        return -1;
    }
   
    bool res = knapsack.solve(argv[1]);
#ifdef DEBUG
    cout << "Problem: " << endl;
    knapsack.writeProblem();
#endif  
    if(res == false)
    {
        cout << "error solve : " << argv[1] << endl;
        return -1;
    }
    
#ifdef DEBUG
    cout << "Solution: " << endl;
    //knapsack.writeSolution();
#endif
    knapsack.writeOutput();
    return 0;
}

