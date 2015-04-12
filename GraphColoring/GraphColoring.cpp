/* 
 * File:   GraphColoring.cpp
 * Author: raffa
 * 
 * Created on April 5, 2015, 3:42 PM
 */

#include "GraphColoring.h"
//#include <boost/graph/adjacency_matrix.hpp>
//#include <boost/graph/copy.hpp>
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

bool GraphColoring::setUpProblem() 
{
    using Eigen::Triplet;
    //using boost::adjacency_matrix;
    //using boost::copy_graph;
    
    setObjectSense(false);
    _objCoeff.clear();
    _objCoeff.resize(N);
    _colTypes.clear();
    _colTypes.resize(N);    
   
    _rowType.clear();
    _rowType.resize(E);
    
    _rhsValue.clear();
    _rhsValue.resize(E);
    
    _lb.clear();
    _lb.resize(N);
    _ub.clear();
    _ub.resize(N);
    
    _matrix.resize(0,0);
    _matrix.resize(E,N);
    
    //adjacency_matrix<vecS, vecS, undirectedS> adj_graph;
    //copy_graph(graph, adj_graph);
    
    vector<Triplet<double>> tripleList;
    tripleList.reserve(N);
    
    
    for(unsigned int i = 0; i < N; i++)
    {
        //obj
        _objCoeff[i] = 1;        
        _ub[i] = N;
        _lb[i] = 0;
        _colTypes[i] = 'I';
    }
    
    unsigned int i=0;
    BGL_FORALL_EDGES(e, graph, Graph)
    {
        
        _rowType[i] = 'N';
        _rhsValue[i] = e.m_target;
        tripleList.push_back(Triplet<double>(i,0,e.m_source));
        
        i++;
    }
    
    if(i!=E)
        return false;
    /*
    for(unsigned int i = 0; i < E; i++)
    {
        graph.
        _colTypes[i] = 'B';
        tripleList.push_back(Triplet<double>(0, i, _vItems[i].second));
        _rowType[0]  = 'L';
        _rhsValue[0] = c;
    }
    */
    
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



