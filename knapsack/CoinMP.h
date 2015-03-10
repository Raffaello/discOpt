/* 
 * File:   CoinMP.h
 * Author: raffa
 *
 * Created on March 8, 2015, 6:53 PM
 */

#ifndef COINMP_H
#define	COINMP_H

#include <string>
#include "coin/CoinMP.h"
#include <vector>
#include <Eigen/SparseCore>

using std::string;
using std::vector;
using Eigen::SparseMatrix;

class CoinMP
{
public:
    CoinMP();
    CoinMP(const CoinMP& orig);
    virtual ~CoinMP();
    
    inline string getSolverName()    const { return CoinGetSolverName(); };
    inline double getVersion()       const { return CoinGetVersion(); };
    inline string getVersionString() const { return CoinGetVersionStr(); };
    
    void reset();
protected:
    SparseMatrix<double> _matrix;
    bool createProblem(const string& problemName);
    void destroyProblem();
    bool loadMatrix();
    bool solveProblem(const int method = SOLV_METHOD_DEFAULT);
       
    inline void setObjectSense(bool max) { (max) ? _objSense = SOLV_OBJSENS_MAX : _objSense = SOLV_OBJSENS_MIN; } 
    
    double _objConst = 0.0;
    
    vector<double> _objCoeff;
    vector<double> _lb;
    vector<double> _ub;
    
    vector<double> _rhsValue;
    vector<char> _rowType;
    
    vector<string> _colNames;
    vector<string> _rowNames;
    
    vector<double> _initValues;
    
    vector<double> _rangeValues;
    
    enum class eSolveMethod { 
        def     = SOLV_METHOD_DEFAULT, 
        barrier = SOLV_METHOD_BARRIER, 
        benders = SOLV_METHOD_BENDERS,
        deq     = SOLV_METHOD_DEQ, 
        dual    = SOLV_METHOD_DUAL, 
        ev      = SOLV_METHOD_EV, 
        net     = SOLV_METHOD_NETWORK, 
        primal  = SOLV_METHOD_PRIMAL };
    
private:
    HPROB _hprob         = nullptr;
    const char* _objName = "obj";
    int _objSense        = SOLV_OBJSENS_MIN;
    int _rangeCount      = 0;
    bool loaded          = false;
    
};

#endif	/* COINMP_H */

