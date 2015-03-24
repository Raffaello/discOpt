/* 
 * File:   CoinMP.cpp
 * Author: raffa
 * 
 * Created on March 8, 2015, 6:53 PM
 */

#include "CoinMP.h"
#include <cmath>

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
        return false;
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
    if(_matrix.isCompressed())
        _matrix.uncompress();
    
    int    *mCount  = _matrix.innerNonZeroPtr();
    int    *mIndex  = _matrix.innerIndexPtr();
    double *mValues = _matrix.valuePtr();
    int    *mBegin  = _matrix.outerIndexPtr();
   
    /*
    vector<int> mBegin(_matrix.outerSize() + 1);
   
    mBegin[0] = 0;
    for(int i = 0; i < _matrix.outerSize(); i++)
        mBegin[i+1] = mBegin[i] + mCount[i];
    */
#ifdef DEBUG
    _write("\nmValues: ");
    for(int i = 0; i<_matrix.nonZeros(); i++)
        _write(to_string(mValues[i]) + " ");
    
    _write("\nmIndex: ");
    for(int i = 0; i<_matrix.innerSize(); i++)
        _write(to_string(mIndex[i]) + " ");
    
    _write("\nmCount: ");
    for(int i = 0; i<_matrix.outerSize(); i++)
        _write(to_string(mCount[i]) + " ");
    
     _write("\nmBegin: ");
    for(int i = 0; i<=_matrix.outerSize(); i++)
        _write(to_string(mBegin[i]) + " ");
#endif
    
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
    
    return true;
}

void CoinMP::writeSolution() 
{
    if(_hprob == nullptr)
        return;
    
    _write(
        string("result: ") 
        + CoinGetSolutionText(_hprob) + '\n'
        + "status:" + to_string(CoinGetSolutionStatus(_hprob)) + '\n'
        + "Object Value:" + to_string(round(CoinGetObjectValue(_hprob))) + '\n'
            
    );
    
    int colCount = CoinGetColCount(_hprob);
    double* xValues = new double[colCount];
    CoinGetSolutionValues(_hprob, xValues, nullptr, nullptr, nullptr);
    for(int i = 0; i < colCount; i++)
    {
        _write(string("X")+to_string(i)+" = "+ to_string(xValues[i]));
    }
   
    delete[] xValues;
}

void CoinMP::writeProblem() 
{
    string str = "OBJ = ";
    
    str += ((_objSense==SOLV_OBJSENS_MIN)?"Min":"Max");
    str += " ";
    for(unsigned int i = 0; i < _objCoeff.size(); i++)
    {
        auto &v = _objCoeff.at(i);
        str += to_string(v) + "X" + to_string(i) + " + ";
    }
    string::size_type l = str.length();
    str[l - 2] = 0;
    str[l - 3] = '\n';
    _write(str);
    str.clear();
    
    for(unsigned int i = 0; i < _matrix.rows(); i++)
    {
        for(unsigned int j = 0; j < _matrix.cols(); j++)
        {
            str += to_string(_matrix.coeff(i,j)) + " + ";
        }
        
        str[str.length()-2] = _rowType[i];
        str += to_string(_rhsValue[i]);
        _write(str+'\n');
        str.clear();
    }
}

void CoinMP::writeOutput() 
{
    string str;
    str = to_string(static_cast<int>(round(CoinGetObjectValue(_hprob)))) + " ";
    str += (CoinGetSolutionStatus(_hprob) == 0)?"1":"0";
    str += "\n";
    _write(str);
    str.clear();
    
    int colCount = CoinGetColCount(_hprob);
    double *xValues = new double[colCount];
    if(CoinGetSolutionValues(_hprob, xValues, NULL, NULL, NULL) != SOLV_CALL_SUCCESS) 
    {
        delete[] xValues;
        return;
    }   
	
    
    for(unsigned int i = 0; i < colCount; i++)
    {
        _write(to_string(static_cast<int>(round(xValues[i]))) + " ");
    }
    
    _write("\n");
}






