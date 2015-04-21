/* 
 * File:   GraphColoring.h
 * Author: raffa
 *
 * Created on April 5, 2015, 3:42 PM
 */

#ifndef GRAPHCOLORING_H
#define	GRAPHCOLORING_H

#include "../CoinMP.h"
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

using boost::adjacency_list;
using boost::vecS;
using boost::undirectedS;
using boost::add_edge;
using boost::graph_traits;
using boost::no_property;


class GraphColoring final : public CoinMP
{
public:
    GraphColoring(writeDelegate write = nullptr);
    GraphColoring(const GraphColoring& orig) = delete;
    virtual ~GraphColoring();
    
    virtual bool solve(const string& filename);
private:
    virtual bool loadFile(const string& filename);
    virtual bool setUpProblem();
    unsigned int computeMaxDegree();
    
    unsigned int N = 0;
    unsigned int E = 0;
    //unsigned int _max_graph_degree = 0;
    struct Edge_t
    {
        unsigned int id;
        unsigned int color;
    };
    
    typedef adjacency_list<vecS, vecS, undirectedS, no_property, Edge_t> Graph_t;
    Graph_t graph;
    
    typedef graph_traits<Graph_t>::edge_descriptor edge_descriptor_t;   
};

#endif	/* GRAPHCOLORING_H */

