/* 
 * File:   main.cpp
 * Author: raffa
 *
 * Created on April 5, 2015, 3:36 PM
 */

#include "GraphColoring.h"
#include <iostream>

/*
 * 
 */
int main(int argc, char** argv) 
{
    using std::endl;
    using std::cout;
    
    GraphColoring graphColoring;
    if(argc != 2)
    {
        cout << "GraphColoring [filename] [file] missing" << endl;
        //return -1;
    }
   
    bool res = graphColoring.solve(/*argv[1]*/"data/gc_4_1");
#ifdef DEBUG
    cout << "Problem: " << endl;
    graphColoring.writeProblem();
#endif  
    if(res == false)
    {
        cout << "error solve : " << argv[1] << endl;
        return -1;
    }
    
#ifdef DEBUG
    cout << "Solution: " << endl;
    graphColoring.writeSolution();
#endif
    graphColoring.writeOutput();
    return 0;
}

