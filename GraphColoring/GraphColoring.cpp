/* 
 * File:   GraphColoring.cpp
 * Author: raffa
 * 
 * Created on April 5, 2015, 3:42 PM
 */

#include "GraphColoring.h"
#include <boost/graph/copy.hpp>
#include <boost/graph/iteration_macros.hpp>

GraphColoring::GraphColoring(writeDelegate write) : CoinMP(write)
{
    setObjectSense(false);
}

GraphColoring::~GraphColoring() 
{
}

bool GraphColoring::loadFile(const string& filename) 
{
    using std::tie;
    
    FILE *f;
    
    _filename.clear();
    graph.clear();
    f = fopen(filename.c_str(), "r");
    if(f == nullptr)
        return false;
    
    fscanf(f, "%d %d", &N, &E);
    for(register unsigned int i = 0; i < E; i++)
    {
        register unsigned int u,v;
        
        fscanf(f, "%d %d", &u, &v);        
        add_edge(u, v, graph);
    }
    
    fclose(f);
    _filename = filename;
    return true;
}

unsigned int GraphColoring::computeMaxDegree() 
{
    register unsigned int max_k = 0;
    BGL_FORALL_VERTICES(v, graph, Graph_t)
    {
        register unsigned int k = 0;
        BGL_FORALL_INEDGES(v, e, graph, Graph_t)
        {
            k++;
        }
        
        if(max_k < k)
            max_k = k;
    }
    
    return max_k;
}

bool GraphColoring::setUpProblem() 
{
    using Eigen::Triplet;
    
    auto ub = computeMaxDegree() + 1; // this should replace N
    auto ncols = ub*N;
    //auto nrows = 3*N;
    //auto nrows= N*5;
    //auto nrows = N + N*ub + N*(N-1);
    auto nrows = N + N*(N-1);
    
    setObjectSense(false);
    _objCoeff.clear();
    _objCoeff.resize(ncols);
    _colTypes.clear();
    _colTypes.resize(ncols); 
   
    _rowType.clear();
    _rowType.resize(nrows);
    
    _rhsValue.clear();
    _rhsValue.resize(nrows);
    
    _lb.clear();
    _lb.resize(ncols);
    _ub.clear();
    _ub.resize(ncols);
    
    _matrix.resize(0,0);
    _matrix.resize(nrows, ncols);
    
    vector<Triplet<double>> tripleList;
    tripleList.reserve(nrows); 

    /*
    // Y
    _objCoeff[0] = 1.0;
    _colTypes[0] = 'I';
    _ub[0] = ub;
    _lb[0] = 1.0;
    */
    for(unsigned int i=0; i<ncols; i++)
    {
        _objCoeff[i] = i%ub + 1;
        _ub[i] = 1.0;
        _lb[i] = 0.0;
        _colTypes[i] = 'B';
    }
    
    int i=0; 
    //int i2=N;
    //int i3=N + N*ub;
    int i3=N;
    BGL_FORALL_VERTICES(v, graph, Graph_t)
    {
        _rowType[i]  = 'E';
        _rhsValue[i] = 1; 
        //Xik 1st constraints sum Xik = 1
        for(unsigned int j = 0; j < ub; j++)
        {
            //Xik 1st constraints sum Xik = 1
            tripleList.push_back(Triplet<double>(i, v+ub*j, 1.0));
        }
        ++i;
        /*
        //2nd 
        for(unsigned int j = 0; j < ub; j++)
        {
            _rhsValue[i2] = ub;
             _rowType[i2]  = 'L';
            tripleList.push_back(Triplet<double>(i2, 0, 1));
            tripleList.push_back(Triplet<double>(i2, v+ub*j+1, ub));
            ++i2;
        }
        */ 
        //3rd Xik + Xjk <= 1
        BGL_FORALL_OUTEDGES(v, e, graph, Graph_t)
        {    
            unsigned int ii = e.m_target;
            
            if(v>=ii)
                continue;
            
            for(unsigned int j=0; j<ub; j++)
            {
                _rowType[i3]  = 'E';
                _rhsValue[i3] = 1;
                tripleList.push_back(Triplet<double>(i3, v+ub*j, 1));
                tripleList.push_back(Triplet<double>(i3, ii+ub*j, 1));
                i3++;
            } 
        }
    }
   
    _matrix.setFromTriplets(tripleList.begin(), tripleList.end());
    
    return true;
}

bool GraphColoring::solve(const string& filename) 
{
    if(loadFile(filename) == false)
        return false;
    
    if(_filename == "")
        return false;
    
    if(setUpProblem() == false)
        return false;
    
    if(createProblem(_filename.c_str()) == false)
        return false;
    
    if(loadProblem() == false)
        return false;
        
    if(solveProblem() == false)
        return false;
    
    return false;
}