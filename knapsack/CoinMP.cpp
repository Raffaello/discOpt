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
}

CoinMP::CoinMP(const CoinMP& orig) {
}

CoinMP::~CoinMP() 
{
    int ret;
    ret = CoinFreeSolver();
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

bool CoinMP::loadMatrix() 
{
  /*  int ret = CoinLoadMatrix(_hprob, _matrix.size2(), _matrix.size1(), _matrix.nnz(),
            _rangeCount, _objSense, _objConst, &_objCoeff[0], 
            &_lb[0], &_ub[0], &_rowType[0], &_rhsValue[0], nullptr, 
            _matrix., _matrix., _matrix, _matrix);
    */
    return true;
}



