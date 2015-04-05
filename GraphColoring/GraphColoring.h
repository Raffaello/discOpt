/* 
 * File:   GraphColoring.h
 * Author: raffa
 *
 * Created on April 5, 2015, 3:42 PM
 */

#ifndef GRAPHCOLORING_H
#define	GRAPHCOLORING_H

#include "../CoinMP.h"

class GraphColoring final : public CoinMP
{
public:
    GraphColoring(writeDelegate write = nullptr);
    GraphColoring(const GraphColoring& orig) = delete;
    virtual ~GraphColoring();
private:
    virtual bool loadFile(const string& filename);
    virtual bool setUpProblem();

};

#endif	/* GRAPHCOLORING_H */

