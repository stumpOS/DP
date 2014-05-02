#include "LinearProblem.h"


LinearProblem::LinearProblem(void)
{
}


LinearProblem::~LinearProblem(void)
{
}

double LinearProblem::Reward(Decision d)
{
	return 0;
}
Decision LinearProblem::Transition(Decision d)
{
	return d;
}
Decision LinearProblem::Transition(Decision currState,Decision d)
{
	return currState;
}
std::vector<Decision> LinearProblem::GetDecisionSpace(Decision state)
{
	std::vector<Decision> space;
	return space;
}
void LinearProblem::DPFE()
{
	Decision optCost;
	DPFE(_superProblemState, optCost);
	Problem::_optimal = (double)optCost._int;
}
void LinearProblem::DPFE(Decision key, Decision &cost)
{
	if(LookUpSubproblem(key, cost))
		return;

	else if(BaseCase(key,cost))
		return;

	vector<Decision> space = GetDecisionSpace(key);
	// we've reached a terminating state if there are no decisions
	if(space.size()<1)
		return;
	
	// initialize the value of the solution in the dpTable
	int initCost = Decide(key,space[0]);
	_dpMap[key._int] = Problem::_operatorFunction(initCost, (int)Reward(space[0]));

	// now loop through all elgible decisions and choose the optimal one
	for(int k=1;k<space.size();k++)
	{
		int c = Decide(key,space[k]);
		cost._int = Problem::_operatorFunction(c, (int)Reward(space[k]));
		if(Problem::isOpt(cost._int,_dpMap[key._int]))
		{
			_dpMap[key._int] = cost._int;
			_dpTraceback[key._int] = space[k]._int;
		}			
	}
}
int LinearProblem::Decide(Decision currState, Decision d)
{
	// find the decision that yields the optimal result
	Decision optCost;
	// Transition returns the resulting state from making a decision
	Decision nextState = Transition(currState,d);
	// the NextState is a subproblem of current state, also referred to as 'key'
	DPFE(nextState,optCost);
	return optCost._int;
}

bool LinearProblem::LookUpSubproblem(Decision key, Decision& value)
{
	map<int,int>::iterator it = _dpMap.find(key._int);
	if(it!=_dpMap.end())
	{
		value._int = it->second;
		return true;
	}
	return false;
}
 bool LinearProblem::BaseCase(Decision state,Decision &cost)
 {
	 return false;
 }
