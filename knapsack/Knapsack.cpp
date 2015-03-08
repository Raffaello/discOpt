/* 
 * File:   Knapsack.cpp
 * Author: raffa
 * 
 * Created on March 8, 2015, 5:11 PM
 */

#include "Knapsack.h"

#include <cstdio>
#include <vector>


Knapsack::Knapsack() 
{
    
}


Knapsack::~Knapsack() 
{
    
}

bool Knapsack::loadFile(const string& filename) 
{
    FILE *f;
    
    _filename="";
    _vItems.clear();
    
    f = fopen(filename.c_str(), "r");
    if(f == nullptr)
        return false;
    
    fscanf(f, "%d %d", &n, &c);
    _vItems.reserve(n);
    for(register unsigned int i = 0; i < n; i++)
    {
        register item it;
        fscanf(f,"%d %d", &it.first, &it.second);
        _vItems[i] = it;
    }
    
    fclose(f);
    _filename = filename;
    return true;
}

bool Knapsack::solve() 
{
    if(_filename == "")
        return false;
    
    if((_vItems.size() != n) || (n==0))
            return false;
    
    //set up problem with CoinMP
    //1. init coinmp
    //2. create problem
    //3. load matrix
    //4. load names
    //5. coin check problem
    //6. callback set
    //7. optimize
    //8. write solution
    //9. unload problem
    
    // free coinmp
    
    
    
}
