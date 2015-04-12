/* 
 * File:   GraphColoring.cpp
 * Author: raffa
 * 
 * Created on April 5, 2015, 3:42 PM
 */

#include "GraphColoring.h"
#include <boost/graph/adjacency_matrix.hpp>
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
    using boost::adjacency_matrix;
    using boost::copy_graph;
    using boost::num_vertices;
    
    setObjectSense(false);
    _objCoeff.clear();
    _objCoeff.resize(N);
    _colTypes.clear();
    _colTypes.resize(N);    
   
    _rowType.clear();
    _rowType.resize(N);
    
    _rhsValue.clear();
    _rhsValue.resize(N);
    
    _lb.clear();
    _lb.resize(N);
    _ub.clear();
    _ub.resize(N);
    
    _matrix.resize(0,0);
    _matrix.resize(N,N);
    
    adjacency_matrix<undirectedS> adj_graph(num_vertices(graph));
    BGL_FORALL_EDGES(e, graph, Graph)
    {
        add_edge(e.m_source, e.m_target, adj_graph);
    }
    
    vector<Triplet<double>> tripleList;
    tripleList.reserve(N);
    
    //to finish the formulation problem
    
    for(unsigned int i = 0; i < N; i++)
    {
        //obj
        _objCoeff[i] = 1;  //Yi
        _ub[i] = N;
        _lb[i] = 1;
        _colTypes[i] = 'B';
            
        //tripleList.push_back(Triplet<double>(i,i,1));
        
        _rowType[i]  = 'E';
        _rhsValue[i] = 1;
        /*
        BGL_FORALL_EDGES(e, graph, Graph)
        {
            e.m_source + e.m_target <=1
            tripleList.push_back(Triplet<double>(e.m_source, e.m_target, ));
        }
                */
    }
    
    
    unsigned int i=0;
    BGL_FORALL_EDGES(e, graph, Graph)
    {
        
        _rowType[i] = 'L';
        _rhsValue[i] = 1
        e.m_target;
        tripleList.push_back(Triplet<double>(e.m_source,e.m_target,1));
        
        i++;
    }
    
    //if(i!=E)
    //    return false;
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



