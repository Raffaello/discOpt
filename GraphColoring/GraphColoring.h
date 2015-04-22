/* 
 * File:   GraphColoring.h
 * Author: raffa
 *
 * Created on April 5, 2015, 3:42 PM
 */

#ifndef GRAPHCOLORING_H
#define	GRAPHCOLORING_H

#include "../CoinMP.h"
#include <boost/graph/adjacency_list.hpp>

using boost::adjacency_list;
using boost::vecS;
using boost::undirectedS;
using boost::add_edge;

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

    typedef adjacency_list<vecS, vecS, undirectedS> Graph_t;
    Graph_t graph; 
};

#endif	/* GRAPHCOLORING_H */

