/* 
 * File:   Knapsack.h
 * Author: raffa
 *
 * Created on March 8, 2015, 5:11 PM
 */

#ifndef KNAPSACK_H
#define	KNAPSACK_H

#include "CoinMP.h"

using std::pair;
    
class Knapsack : public CoinMP
{
    
public:
    Knapsack(writeDelegate write = nullptr);
    Knapsack(const Knapsack& orig) = delete;
    virtual ~Knapsack();
    /*
    typedef struct
    {
        int value;
        int weight;
    } item;
    */
    typedef pair<int , int> item;
    
    
    bool solve(const string& filename);
    //bool writeOutput();
    
private:
    string _filename = "";
    unsigned int n = 0;
    int c = 0;
    vector<item> _vItems;
    
    bool loadFile(const string& filename);
    bool setUpProblem();
};

#endif	/* KNAPSACK_H */

