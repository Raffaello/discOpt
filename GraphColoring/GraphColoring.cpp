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
        register bool inserted;
        edge_descriptor_t e;
        
        fscanf(f, "%d %d", &u, &v);
 
        
        tie(e, inserted) = add_edge(u, v, graph);
        
        
        graph[e].id = i;
        graph[e].color = i;
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
    using boost::adjacency_matrix;
    using boost::copy_graph;
    using boost::num_vertices;
    
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
    _matrix.resize(E,1);
    
    //adjacency_matrix<undirectedS> adj_graph(num_vertices(graph));
    //BGL_FORALL_EDGES(e, graph, Graph)
    //{
    //    add_edge(e.m_source, e.m_target, adj_graph);
    //}
    
    vector<Triplet<double>> tripleList;
    tripleList.reserve(E);
    
    //to finish the formulation problem
    auto ub = computeMaxDegree(); // this should replace N
    
    for(unsigned int i = 0; i < N; i++)
    {
        //obj
        _objCoeff[i] = i;  //Yi color index
        _ub[i] = 1;
        _lb[i] = 0;
        _colTypes[i] = 'B';
            
        //tripleList.push_back(Triplet<double>(i,i,1));
        
        //_rowType[i]  = 'E';
        //_rhsValue[i] = 1;
        /*
        BGL_FORALL_EDGES(e, graph, Graph)
        {
            e.m_source + e.m_target <=1
            tripleList.push_back(Triplet<double>(e.m_source, e.m_target, ));
        }
                */
    }
    
    //matrix graph
    BGL_FORALL_EDGES(e, graph, Graph_t)
    {
        //auto a = graph[e].id;
        tripleList.push_back(Triplet<double>(e.m_source,0, e.m_source));
        
        //auto id = graph[e].id;
        
        _rowType[e.m_source]  = 'N';
        _rhsValue[e.m_source] = e.m_target;
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



