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
    auto nrows = N+E;
    
    setObjectSense(false);
    _objCoeff.clear();
    _objCoeff.resize(ub);
    _colTypes.clear();
    _colTypes.resize(ub);    
   
    _rowType.clear();
    _rowType.resize(nrows);
    
    _rhsValue.clear();
    _rhsValue.resize(nrows);
    
    _lb.clear();
    _lb.resize(ub);
    _ub.clear();
    _ub.resize(ub);
    
    _matrix.resize(0,0);
    _matrix.resize(nrows, ub);
    
    vector<Triplet<double>> tripleList;
    tripleList.reserve(N); 
            
    // for each color as column
    for(unsigned int i = 0; i < ub; i++)
    {
        _objCoeff[i] = 1;  //Yi color index
        _ub[i] = ub;
        _lb[i] = 0;
        _colTypes[i] = 'I';  
    }
    
    //Xik 1st constraints sum Xik = 1
    for(unsigned int i = 0; i < N; i++)
    {
        _rowType[i]  = 'E';
        _rhsValue[i] = 1; 
        for(unsigned int j = 0; j < ub; j++)
        {
            tripleList.push_back(Triplet<double>(i, j, 1));
        }
    }
    
    //2nd Xik + Xjk <= 1
    BGL_FORALL_VERTICES(v, graph, Graph_t)
    {
        // FIX the triplet matrix
        BGL_FORALL_OUTEDGES(v, e, graph, Graph_t)
        {
            //e.m_source;
            unsigned int i = N + e.m_target;
            
            _rowType[i]  = 'L';
            _rhsValue[i] = 1;
            tripleList.push_back(Triplet<double>(i, 0, 1));
            tripleList.push_back(Triplet<double>(i, 1, 1));
        }
    }
    /*
    //3rd Xik <= Yik
    unsigned int idx = N+E;
    
    for(unsigned int i = 0; i < N; i++)
    {
        unsigned int ii = idx + i*ub;
        for(unsigned int j = 0; j < ub; j++)
        {
            _rowType[ii + j] = 'L';
            _rhsValue[ii+j]
        }
    }
     * */
   
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



