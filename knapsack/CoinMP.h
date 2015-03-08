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
using namespace boost::numeric::ublas;
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
    
    
protected:
    
    compressed_matrix<double, column_major> _matrix;
    bool createProblem(const string& problemName);
    void destroyProblem();
    bool loadMatrix();
    
    enum class eObjSense : std::int8_t {MIN = SOLV_OBJSENS_MIN, MAX = SOLV_OBJSENS_MAX};
    
    inline void setObjectSense(eObjSense e) { _objSense = e; };
    
    double _objConst = 0.0;
    
    vector<double> _objCoeff;
    vector<double> _lb;
    vector<double> _ub;
    
    vector<double> _rhsValue;
    vector<char> _rowType;
    
    vector<string> _colNames;
    vector<string> _rowNames;
    
    vector<double> _initValues;
    
    //matrix in CCS format
    // use sparselib++ instead of ublas
    
private:
    HPROB _hprob = nullptr;
    const char* _objName = "obj";
    int _objSense = SOLV_OBJSENS_MIN;
    const int _rangeCount = 0;
    
};

#endif	/* COINMP_H */

