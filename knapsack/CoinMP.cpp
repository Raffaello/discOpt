/* 
 * File:   CoinMP.cpp
 * Author: raffa
 * 
 * Created on March 8, 2015, 6:53 PM
 */

#include "CoinMP.h"

CoinMP::CoinMP() 
{
    int ret;
    
    ret = CoinInitSolver("");
    if(ret != 0)
    {
        //error
    }
}

CoinMP::CoinMP(const CoinMP& orig) {
}

CoinMP::~CoinMP() 
{
    int ret;
    ret = CoinFreeSolver();
    if(ret != 0)
    {
        //error
    }
}

bool CoinMP::createProblem(const string& problemName) 
{
    destroyProblem();
    
    _hprob = CoinCreateProblem(problemName.c_str());
    if(_hprob == nullptr)
        return false;
    
    return true;
}

void CoinMP::destroyProblem() 
{
    if(_hprob != nullptr)
    {
        CoinUnloadProblem(_hprob);
        _hprob = nullptr;
    }
}

void CoinMP::reset()
{
    _colNames.clear();
    if(_hprob != nullptr)
        destroyProblem();
    _initValues.clear();
    _lb.clear();
    _matrix.clear();
    _objCoeff.clear();
    _objConst=0.0;
    setObjectSense(false);
    _rangeCount = 0;
    _rangeValues.clear();
    _rhsValue.clear();
    _rowNames.clear();
    _rowType.clear();
    _ub.clear();
}

bool CoinMP::loadMatrix() 
{
    _matrix.complete_index1_data();

    int    *mBegin  = nullptr; //new int[_matrix.size2() + 1];
    int    *mCount  = nullptr;
    int    *mIndex  = reinterpret_cast<int*> (&_matrix.index1_data()[0]);
    double *mValues = &_matrix.value_data()[0];
    
    
    int ret = CoinLoadMatrix(_hprob, _matrix.size2(), _matrix.size1(), _matrix.nnz(),
            _rangeCount, _objSense, _objConst, &_objCoeff[0], 
            &_lb[0], &_ub[0], &_rowType[0], &_rhsValue[0], &_rangeValues[0], 
            mBegin, mCount, mIndex, mValues);
    
    return true;
}



