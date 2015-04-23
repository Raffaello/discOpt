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
    auto ncols = ub*N+1;
    //auto nrows = 3*N;
    auto nrows= N*3 +E;
    
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

    // Y
    _objCoeff[0] = 1;
    _colTypes[0] = 'I';
    _ub[0] = ub;
    _lb[0] = 1;
    
    for(unsigned int i=1; i<ncols; i++)
    {
        _objCoeff[i] = 0;
        _ub[i] = 1;
        _lb[i] = 0;
        _colTypes[i] = 'B';
    }
     
    BGL_FORALL_VERTICES(v, graph, Graph_t)
    {
        _rowType[v]  = 'E';
        _rhsValue[v] = 1; 
        //Xik 1st constraints sum Xik = 1
        for(unsigned int j = 0; j < ub; j++)
        {
            tripleList.push_back(Triplet<double>(v, v*ub+j+1, 1));
        }

        //2nd
        _rhsValue[N+v] = ub;
        _rowType[N+v]  = 'L';
        tripleList.push_back(Triplet<double>(N+v, 0, 1));

         //3rd Xik + Xjk <= 1
        //FIX THE ROWS INDEX
        int j = 0;
            BGL_FORALL_OUTEDGES(v, e, graph, Graph_t)
            {
                unsigned int i = e.m_target;
                //unsigned int k = e.m_source;
                _rowType[2*N+v+j]  = 'L';
                _rhsValue[2*N+v+j] = 1;
                tripleList.push_back(Triplet<double>(N*2+v+j, v*ub+1, 10));
                tripleList.push_back(Triplet<double>(N*2+v+j, i*ub+1, 1));
                j++;
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



