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
#include <functional>
#include <iostream>

using std::string;
using std::vector;
using Eigen::SparseMatrix;
using std::function;
using std::cout;

class CoinMP
{
public:
    typedef function<void(string)> writeDelegate;
    
    CoinMP(writeDelegate write = nullptr);
    CoinMP(const CoinMP& orig) = delete;
    virtual ~CoinMP();
    
    inline string getSolverName()    const { return CoinGetSolverName(); };
    inline double getVersion()       const { return CoinGetVersion(); };
    inline string getVersionString() const { return CoinGetVersionStr(); };
    void writeSolution();
    void writeProblem();
    virtual void writeOutput();
    void reset();
protected:
    writeDelegate _write;
    
    enum class eSolveMethod : int { 
        Default = SOLV_METHOD_DEFAULT, 
        Barrier = SOLV_METHOD_BARRIER, 
        Benders = SOLV_METHOD_BENDERS,
        Deq     = SOLV_METHOD_DEQ, 
        Dual    = SOLV_METHOD_DUAL, 
        Ev      = SOLV_METHOD_EV, 
        Network = SOLV_METHOD_NETWORK, 
        Primal  = SOLV_METHOD_PRIMAL 
    };
    
    SparseMatrix<double> _matrix;
    bool createProblem(const string& problemName);
    void destroyProblem();
    
    bool loadProblem();
    bool unloadProblem();        
    bool solveProblem(const eSolveMethod method = eSolveMethod::Default);
    
    
    inline void setObjectSense(bool max) { ((max)? _objSense = SOLV_OBJSENS_MAX : _objSense = SOLV_OBJSENS_MIN); } 
    
    double _objConst = 0.0;
    
    vector<double> _objCoeff;
    vector<double> _lb;
    vector<double> _ub;
    
    vector<double> _rhsValue;
    vector<char> _rowType;
    
    vector<string> _colNames;
    vector<string> _rowNames;

    vector<char> _colTypes;
    
    vector<double> _initValues;
    
    vector<double> _rangeValues; 
    
private:
    HPROB _hprob         = nullptr;
    const char* _objName = "obj";
    int _objSense        = SOLV_OBJSENS_MIN;
    int _rangeCount      = 0;
    bool loaded          = false;
    
    bool loadMatrix();
    bool loadNames();
    bool loadColumnType();
    
    inline static void print(const string str) { _print(str.c_str(), nullptr); };
    inline static int _print(const char* str, void* p)   { cout << str; };
    //COIN_MSGLOG_CB _writeMsg;
    inline int _printMsg(const char*str, void* p) { _write(str); }
};

#endif	/* COINMP_H */

