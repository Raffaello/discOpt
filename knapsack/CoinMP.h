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
#include "boost/numeric/ublas/matrix_sparse.hpp"
#include "boost/numeric/ublas/io.hpp"
#include <cstdint>
#include <vector>

using std::string;
using boost::numeric::ublas::compressed_matrix;
using boost::numeric::ublas::column_major;
using std::vector;

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
    
    compressed_matrix<double, column_major> _matrix;
    bool createProblem(const string& problemName);
    void destroyProblem();
    bool loadMatrix();
       
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
    
    //matrix in CCS format
    // use sparselib++ instead of ublas
    
private:
    HPROB _hprob = nullptr;
    const char* _objName = "obj";
    int _objSense = SOLV_OBJSENS_MIN;
    int _rangeCount = 0;
    
    
};

#endif	/* COINMP_H */

