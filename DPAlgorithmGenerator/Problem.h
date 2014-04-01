#pragma once
#ifndef PROBLEM_H_
#define PROBLEM_H_
#include<vector>
#include<string>
#include<map>
enum OPT{MAX, MIN};
// operator must be associative
enum Operator{eADD, eMULT};
union Decision{
	int _int;
	struct {
		short _shortHi;
		short _shortLo;
	} _Pair;
	char _char[4];
	bool _bool;
};
class DecisionHandler{
public:
	DecisionHandler(){}
	~DecisionHandler(){}
	static Decision ConstructFromPair(short i,short j)
	{
		Decision d;
		d._Pair._shortHi = i;
		d._Pair._shortLo = j;
		return d;
	}
};
class PetriNet;
typedef int SOLUTIONTYPE;
/* the operator will default to addition for now 
*/
class Problem
{
public:
	Problem();
	~Problem(void);

	virtual int Reward(Decision state,Decision d);
	virtual std::vector<Decision> Transition(Decision state,Decision d);

	virtual std::vector<Decision> GetDecisionSpace(Decision state) = 0;

	virtual void DPFE() = 0;
	virtual std::string GetInputData() = 0;
	virtual std::string GetSolution() = 0;
	virtual void Traceback() = 0;
	
	double GetOptimal(){return _optimal;}
	void SetOpt(OPT opt);
	void SetOperator(Operator o);
	void SetNumRecursiveCalls(short n){_numRecursiveCalls = n;}
	short GetNumberOfRecursiveCallsInDPFE(){return _numRecursiveCalls;}

	int EvaluateArithmeticExpression(int a, int b){return _operatorFunction(a,b);}
	bool isOpt(int a, int b){return _optFunction(a,b);}

	void ClearNet();
	
	static int Add(int a, int b){return a + b;}
	static int Mult(int a, int b){return a*b;}
	static bool isMax(int a,int b){return a > b;}
	static bool isMin(int a,int b){return a < b;}
	
	virtual SOLUTIONTYPE Base(Decision baseState){return 0;}
	virtual bool IsBaseState(Decision state) = 0;
	void SetOptimal(SOLUTIONTYPE o){_optimal = o;}
	virtual bool IsValidState(Decision state) {return true;}

	void AddToTrackback(int key, Decision d);
protected:
	OPT _opt;
	Operator _operator;
	std::vector<Decision> _state;
	int (*_operatorFunction)(int,int);
	bool (*_optFunction)(int,int);
	
	double _optimal;
	short _numRecursiveCalls;
	PetriNet *_net;
	std::map<int,Decision> _dpTraceback;
};
#endif

