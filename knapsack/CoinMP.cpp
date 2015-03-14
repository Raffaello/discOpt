/* 
 * File:   CoinMP.cpp
 * Author: raffa
 * 
 * Created on March 8, 2015, 6:53 PM
 */

#include "CoinMP.h"

using std::to_string;

CoinMP::CoinMP(writeDelegate write) 
{
    int ret;
    
    if(write == nullptr)
        _write = print;
    else
        _write = write;
   
    //_writeMsg = ;
    ret = CoinInitSolver("");
    if(ret != SOLV_CALL_SUCCESS)
    {
        //error
        
    }
    
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
    
    //auto f = std::mem_fn(&CoinMP::_printMsg);
    //COIN_MSGLOG_CB _writeMsg = &CoinMP::_printMsg; 
    //CoinRegisterMsgLogCallback(_hprob, &_printMsg, nullptr);
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
    _colTypes.clear();
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

bool CoinMP::loadProblem() 
{
    if(loaded)
        return true;
    
    if(unloadProblem() == false)
        return false;
    
    if(loadMatrix() == false)
        return false;
    
    if(loadNames() == false)
        return false;
    
    if(loadColumnType() == false)
        return false;
    int ret = CoinCheckProblem(_hprob);
    if (ret != SOLV_CALL_SUCCESS) 
    {
        _write(string("checkProblem error: ") + to_string(ret) + '\n');
    //    return false;
    }
    
    return loaded=true;
}

bool CoinMP::unloadProblem() 
{
    if(loaded == false)
        return true;
    
    if(CoinUnloadProblem(_hprob) != SOLV_CALL_SUCCESS)
        return false;
    
    loaded=false;
    return true;
}


bool CoinMP::loadMatrix() 
{   
    if(_matrix.isCompressed() == false)
        _matrix.makeCompressed();
    
    int    *mCount  = _matrix.outerIndexPtr();
    int    *mIndex  = _matrix.innerIndexPtr();
    double *mValues = _matrix.valuePtr();
    
    vector<int> mBegin(_matrix.outerSize() + 1);
    
    mBegin[0] = 0;
    for(int i = 1; i < _matrix.outerSize(); i++)
        mBegin[i+1] = mBegin[i] + mCount[i];
    
    int ret = CoinLoadMatrix(_hprob, _matrix.cols(), _matrix.rows(), _matrix.nonZeros(),
            _rangeCount, _objSense, _objConst, &_objCoeff[0], 
            &_lb[0], &_ub[0], &_rowType[0], &_rhsValue[0], &_rangeValues[0], 
            &mBegin[0], mCount, mIndex, mValues);
    if(ret != SOLV_CALL_SUCCESS)
    {
        _write(string("LoadMatrix error: ") + to_string(ret) + '\n');
        return false;
    }
    
    return true;
}

bool CoinMP::loadNames() 
{
    if(_hprob == nullptr)
        return false;
 /*   
    for(auto &v : _colNames)
        v.shrink_to_fit();
    for(auto &v : _rowNames)
        v.shrink_to_fit();
 */  
    char *cn = nullptr;
    char *rn = nullptr;
    
    if(CoinLoadNames(
            _hprob, 
            nullptr, 
            nullptr, 
            _objName) != SOLV_CALL_SUCCESS)
        return false;
    
    return true;     
}

bool CoinMP::loadColumnType() 
{
    if(_hprob == nullptr)
        return false;
    
    if((_colTypes.size() == 0)
        || (_colTypes.size() != _objCoeff.size()))
        return false;
    
    if(CoinLoadInteger(_hprob, &_colTypes[0]) != SOLV_CALL_SUCCESS)
        return false;
    
    return true;
}


bool CoinMP::solveProblem(const eSolveMethod method) 
{
    if(loaded == false)
        return false;
    
    int result = CoinOptimizeProblem(_hprob, static_cast<int> (method));
    if(result != SOLV_CALL_SUCCESS) 
    {
        _write(string("solveProblem error: ") + to_string(result) + '\n');
        return false;
    }
}

bool CoinMP::writeSolution() 
{

    _write(
        string("result: ") 
        + CoinGetSolutionText(_hprob) + '\n'
        + "status:" + to_string(CoinGetSolutionStatus(_hprob)) + '\n'
        + "Object Value:" + to_string(CoinGetObjectValue(_hprob)) + '\n'
            
    );
   
    /** example.c
     * 
	colCount = CoinGetColCount(hProb);
	xValues = (double* )malloc(colCount * sizeof(double));
	CoinGetSolutionValues(hProb, xValues, NULL, NULL, NULL);
	for (i = 0; i < colCount; i++) {
		if (xValues[i] != 0.0) {
			ColName = CoinGetColName(hProb, i);
			fprintf(stdout, "%s = %.20g\n", ColName, xValues[i]);
		}
	}
	fprintf(stdout, "---------------------------------------\n\n");
	assert(solutionStatus==0);
	assert(strcmp(solutionText,"Optimal solution found")==0);
	if (optimalValue != 0.0) {
		assert( fabs(objectValue-optimalValue) < 0.001 );
	}
            
    */
}





