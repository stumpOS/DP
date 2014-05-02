#pragma once
#include "Problem.h"
#include <vector>
#include <map>
using namespace std;
/*! a nonserial problem is one in which the DPFE contains
* two subproblems; i.e. one of the form
*	f(S) = OPT(d in D){f(t1(S,d)) o f(t2(S,d)) o r(S,d)}
*/

class LinearProblem: public Problem
{
public:
	LinearProblem(void);
	virtual ~LinearProblem(void);
	virtual double Reward(Decision d);
	virtual Decision Transition(Decision d);
	
	virtual std::vector<Decision> GetDecisionSpace(Decision state);
	virtual void DPFE();
	virtual void Traceback(){}
	virtual std::string GetInputData(){return "";}

	void DPFE(Decision key, Decision &cost);
	bool LookUpSubproblem(Decision key,Decision &cost);
	virtual bool BaseCase(Decision state,Decision &cost);
	
	virtual Decision Transition(Decision currState,Decision d);
	virtual int Reward(Decision state,Decision d){return 0;}

	int Decide(Decision currState, Decision d);
protected:

	///! The superproblem state is the top most state 
	Decision _superProblemState;

	///! The traceback table
	map<int,int> _dpTraceback;

	///! the saved solutions to smaller subproblems 
	map<int, int> _dpMap;

	vector<vector<Decision> > _traceback;
	
};

