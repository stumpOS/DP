#include "Problem.h"
#include "PetriNet.h"

Problem::Problem()
{
	_optimal = 0;
	_opt = MAX;
	_operator = eADD;
	_operatorFunction = Add;
	_optFunction = isMax;
	_numRecursiveCalls = 1;

	_net = new PetriNet(this);
}


Problem::~Problem(void)
{
	delete _net;
}
void Problem::ClearNet()
{
	_net->EmptyTheNet();
}
void Problem::SetOpt(OPT o)
{
	_opt = o;
	switch(o)
	{
	case MAX:
		_optFunction = isMax;
		break;
	case MIN:
		_optFunction = isMin;
		break;
	default:
		break;
	}
}
void Problem::SetOperator(Operator o)
{
	_operator = o;
	switch(o)
	{
	case eADD:
		_operatorFunction = Add;
		break;
	case eMULT:
		_operatorFunction = Mult;
	default:
		break;
	}
}
int Problem::Reward(Decision state, Decision d)
{
	// TODO: Finish me
	return 0;
}
vector<Decision> Problem::Transition(Decision state, Decision d)
{
	// TODO: Finish me
	vector<Decision> states;
	return states;
}
void Problem::AddToTrackback(int key, Decision d)
{
	_dpTraceback[key] = d;
}
