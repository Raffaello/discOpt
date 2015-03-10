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
    if(ret != SOLV_CALL_SUCCESS)
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
    if(ret != SOLV_CALL_SUCCESS)
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
        loaded = false;
    }
}

void CoinMP::reset()
{
    _colNames.clear();
    if(_hprob != nullptr)
        destroyProblem();
    _initValues.clear();
    _lb.clear();
    _matrix.resize(0,0);
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
    if(loaded)
        return true;
    
    if(_matrix.isCompressed() == false)
        _matrix.makeCompressed();
    
    int    *mCount  = _matrix.outerIndexPtr();
    int    *mIndex  = _matrix.innerIndexPtr();
    double *mValues = _matrix.valuePtr();
    
    vector<int> mBegin(_matrix.outerSize() + 1);
    
    mBegin[0] = 0;
    for(int i = 1; i < _matrix.outerSize(); i++)
    {
        mBegin[i+1] = mBegin[i] + mCount[i];
    }
    
    int ret = CoinLoadMatrix(_hprob, _matrix.cols(), _matrix.rows(), _matrix.nonZeros(),
            _rangeCount, _objSense, _objConst, &_objCoeff[0], 
            &_lb[0], &_ub[0], &_rowType[0], &_rhsValue[0], &_rangeValues[0], 
            &mBegin[0], mCount, mIndex, mValues);
    if(ret != SOLV_CALL_SUCCESS)
        return false;
    
    //ret = CoinLoadNames(_hprob, &_colNames[0], &_rowNames[0], _objName);
    //if(ret != 0)
    //    return false;
    
    ret = CoinCheckProblem(_hprob);
    if (ret != SOLV_CALL_SUCCESS) 
        //fprintf(stdout, "Check Problem failed (result = %d)\n", ret);
        return false;
       
    return loaded=true;
}

bool CoinMP::solveProblem(const int method) 
{
    if(loaded == false)
        return false;
    
    int result = CoinOptimizeProblem(_hprob, method);
    if(result != 0)
        return false;
}




