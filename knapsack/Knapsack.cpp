/* 
 * File:   Knapsack.cpp
 * Author: raffa
 * 
 * Created on March 8, 2015, 5:11 PM
 */

#include "Knapsack.h"

#include <cstdio>
#include <vector>


Knapsack::Knapsack() : CoinMP()
{
    setObjectSense(true);
    
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
    _vItems.resize(n);
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

bool Knapsack::setUpProblem() 
{
    using Eigen::Triplet;
    
    _objCoeff.clear();
    _objCoeff.resize(n);
   
    _rowType.clear();
    _rowType.resize(1);
    
    _rhsValue.clear();
    _rhsValue.resize(1);
    
    _matrix.resize(0,0);
    _matrix.resize(1,n);
    
    vector<Triplet<double>> tripleList;
    tripleList.reserve(n);
    for(unsigned int i = 0; i < n; i++)
    {
        //obj
        _objCoeff[i] = _vItems[i].first;
        tripleList.push_back(Triplet<double>(0, i, _vItems[i].second));
    }
    
    _matrix.setFromTriplets(tripleList.begin(), tripleList.end());
    
    _rowType[0]  = 'L';
    _rhsValue[0] = c;
    
    return true;
}

bool Knapsack::solve(const string& filename) 
{
    reset();
    
    if(loadFile(filename) == false)
        return false;
    
    if(_filename == "")
        return false;
    
    if((_vItems.size() != n) || (n==0))
        return false;
    
    //set up problem with CoinMP
    //1. init coinmp V
    //2. create problem V
    //3. load matrix V
    //4. load names
    //5. coin check problem
    //6. callback set
    //7. optimize
    //8. write solution
    //9. unload problem
    
    // free coinmp
    
    int ret;
    
    if(setUpProblem() == false)
        return false;
    
    if(createProblem(_filename.c_str()) == false)
        return false;
    
    if(loadMatrix() == false)
        return false;
        
    if(solveProblem() == false)
        return false;
    
    return true;
}
