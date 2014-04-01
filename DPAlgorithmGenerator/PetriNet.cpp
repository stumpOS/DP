#include "PetriNet.h"

///
/// NODES DEFINITIONS///
/// do not touch the heap in these functions
/// Leave the net to manage the memory. Thanks!

///
/// PLACENODE DEFINITIONS ///
///

///
/// STATEPLACENODE DEFINITIONS ///
///
void StatePlaceNode::Dereference()
{
	for(int i=0;i<_postset.size();i++)
		_postset[i] = NULL;
	_pred = NULL;
}
void StatePlaceNode::SetSolution(int s)
{
	_solution = s;
	_hit = true;
	for(int i=0;i<_postset.size();i++)
	{
		if(_postset[i]==NULL)
			continue;	//WARNING! THIS WILL HIDE ERRORS, AS IT SHOULDN'T EVER BE EXECUTED

		_postset[i]->UpdateKey(_name);
	}
}
///
/// IPLACENODE DEFINITIONS ///
///
void IPlaceNode::Dereference()
{
	_succ = NULL;
	_pred = NULL;
}
void IPlaceNode::SetSolution(int s)
{
	_hit = true;
	PlaceNode::_solution = s;
	if(_succ!=NULL)
	{
		_succ->UpdateKey(_name);

	}
	else
		int i=0;
	
}
///
/// TRANSITIONNODE DEFINITIONS ///
///
void TransitionNode::UpdateKey(unsigned int n)
{
	_fireCounter++;
	// how do we erase one 1? it has to omit any value present in both.
	// bitwise XOR to update key state
	_firekey ^= n;
	// if all 1's are gone
	if((_firekey&0xffffffff)==0)
	{
		_isActivated = true;
		Fire();
	}
}
void TransitionNode::Dereference()
{
	_targetPlace = NULL;
	map<unsigned int, PlaceNode*>::iterator it = _presetMap.begin();
	for(;it!=_presetMap.end();it++)
		it->second = NULL;
}
void TransitionNode::AddPlaceToPreset(PlaceNode *pn)
{
	map<unsigned int,PlaceNode*>::iterator it = _presetMap.begin();
	while(it!=_presetMap.end())
	{
		if(it->second == NULL)
		{
			it->second = pn;
			(it->second)->SetName(it->first);
			break;
		}
		it++;
	}
}
///
/// ETRANSITIONNODE DEFINITIONS ///
///
ETransitionNode::ETransitionNode(PetriNet *net, IPlaceNode *ipn, int r):TransitionNode(net),_reward(r)
{
	_targetPlace = ipn;
	// number of recursive calls in DPFE is the size of the preset
	short presetSize = _net->GetProblem()->GetNumberOfRecursiveCallsInDPFE();
	
	_firekey = 0;
	while(presetSize>0)
	{
		unsigned int v = (unsigned int)pow((double)2, (double)presetSize);
		_firekey+=v;
		_presetMap[v] = NULL;
		presetSize--;
	}
}

void ETransitionNode::AddStatePlaceNodeToPreset(StatePlaceNode *spn)
{
	TransitionNode::AddPlaceToPreset(spn);
}

/*! In fire, the expression is evaluated using the arguments 
* received from the placenodes in the preset. 
*/
void ETransitionNode::Fire()
{
	if(!_isActivated||_hasFired)
		return;
	
	map<unsigned int,PlaceNode*>::iterator it = _presetMap.begin();
	int feed1 = (it->second)->GetSolution(),feed2=0;

	it++;
	// if this is a nonserial problem, there should be exactly one node in
	// the preset
	for(;it!=_presetMap.end(); it++)
	{
		feed2 = (it->second)->GetSolution();
		feed1 = _net->GetProblem()->EvaluateArithmeticExpression(feed1,feed2);
	}

	int result = _net->GetProblem()->EvaluateArithmeticExpression(_reward,feed1);
	_targetPlace->SetSolution(result);
	_hasFired = true;
}
///
/// MTRANSITIONNODE DEFINITIONS ///
///
MTransitionNode::MTransitionNode(PetriNet *net,StatePlaceNode *spn):TransitionNode(net)
{
	_targetPlace = spn;
	// the size of the decision space is the number of places in an mtransitionNode preset
	vector<Decision> dspace = _net->GetProblem()->GetDecisionSpace(spn->GetState());
	_firekey = 0x0;
	for(int i=0;i<dspace.size();i++)
	{
		unsigned int v = (unsigned int)pow((double)2, (double)i);
		_firekey += v;
		_presetMap[v] = NULL;
	}
	
}
void MTransitionNode::Fire()
{
	//all solutions in preset must be evaluated in order to fire. Enforce this:
	if(!TransitionNode::_isActivated||_hasFired)
		return;

	map<unsigned int,PlaceNode*>::iterator it = _presetMap.begin();
	Problem *prob = _net->GetProblem();
	
	int opt = (it->second)->GetSolution();
	Decision choice = (it->second)->GetState();
	Decision state = _targetPlace->GetState();
	it++;

	for(;it!=_presetMap.end();it++)
	{
		if(it->second==NULL)
			continue;
		int next_candidate = (it->second)->GetSolution();
		if(prob->isOpt(next_candidate,opt))
		{
			opt = next_candidate;
			choice = (it->second)->GetState();
			state = _targetPlace->GetState();
		}
	}
	//record traceback information:
	// i.e. we made decision 'choice' to arrive at state 'state._int'
	_net->GetProblem()->AddToTrackback(state._int,choice);
	// send the solution to the target place

	_targetPlace->SetSolution(opt);
	_hasFired = true;
}
void MTransitionNode::AddPlaceToPreset(IPlaceNode *ipn)
{
	TransitionNode::AddPlaceToPreset(ipn);
}
///
/// PETRINET DEFINITIONS ///
///
PetriNet::PetriNet(Problem *p):_problem(p)
{
	
}
PetriNet::~PetriNet(void)
{
	EmptyTheNet();
}
void PetriNet::EmptyTheNet()
{
	// CLEAN UP EVERYTHING

	//first dereference
	for(int i=0;i<_nodes.size();i++)
		_nodes[i]->Dereference();
	map<int,StatePlaceNode*>::iterator it = _dpTable.begin();
	for(;it!=_dpTable.end();it++)
		it->second->Dereference();

	//then delete
	while(!_nodes.empty())
	{
		delete _nodes[_nodes.size()-1];
		_nodes.pop_back();
	}
	it = _dpTable.begin();
	for(;it!=_dpTable.end();it++)
		delete it->second;
	_dpTable.clear();
}
/*!
* construct the petrinet data structure recursively from the largest
* to the smallest subproblem
*/
void PetriNet::CreateNet(Decision targetState)
{
	_targetState = targetState;

	StatePlaceNode *spn = new StatePlaceNode(this, targetState);
	_dpTable[targetState._int] = spn;
	CreateSegment(spn);
}
void PetriNet::CreateSegment(StatePlaceNode *spn)
{
	// every state has at most one mtransitionNode
	// set the statenode as the target place in the constructor call
	MTransitionNode *mtn = new MTransitionNode(this,spn);
	spn->SetPred(mtn);
	_nodes.push_back(mtn);

	vector<Decision> dspace = _problem->GetDecisionSpace(spn->GetState());
	if(dspace.empty())
		return;
	// add an intermediate place for each elgible decision
	// an iplacenode has exactle one preceding etransitionnode
	// and one succeeding mtransitionnode
	for(int i=0;i<dspace.size();i++)
	{
		IPlaceNode *ipn = new IPlaceNode(this);
		ipn->SetDecision(dspace[i]);

		int reward = _problem->Reward(spn->GetState(),dspace[i]);
		ETransitionNode *etn = new ETransitionNode(this,ipn,reward);

		mtn->AddPlaceToPreset(ipn);
		ipn->SetSucc(mtn);
		ipn->SetPred(etn);
		_nodes.push_back(ipn);
		_nodes.push_back(etn);
		// TODO: state should be a set to account for nonserial problems
		vector<Decision> states = _problem->Transition(spn->GetState(),dspace[i]);
		
		for(int j=0;j<states.size();j++)
		{
			Decision state = states[j];
			if(LookUpSubproblem(state))
			{
				StatePlaceNode *subspn = _dpTable[state._int];
				subspn->AddTransitionToPostset(etn);
				etn->AddStatePlaceNodeToPreset(subspn);
			}
			else if(_problem->IsValidState(state))
			{
				StatePlaceNode *subspn = new StatePlaceNode(this, state);
				subspn->AddTransitionToPostset(etn);
				etn->AddStatePlaceNodeToPreset(subspn);
				_dpTable[state._int] = subspn;
					
				if(!_problem->IsBaseState(state))
					CreateSegment(subspn);
				else
				{
					subspn->SetSolution(_problem->Base(state));
				}
			}
		}

	}

	
}
bool PetriNet::LookUpSubproblem(Decision key, int &solution)
{
	return LookUpSubproblem(key._int,solution);
}
bool PetriNet::LookUpSubproblem(int key, int &solution)
{
	map<int,StatePlaceNode*>::iterator it = _dpTable.find(key);
	if(it!=_dpTable.end())
	{
		StatePlaceNode *p = it->second;
		if(p->GetHit()==true)
		{
			solution = p->GetSolution();
		}
		return true;
	}
	return false;
}
bool PetriNet::LookUpSubproblem(Decision state)
{
	map<int,StatePlaceNode*>::iterator it = _dpTable.find(state._int);
	if(it!=_dpTable.end())
		return true;
	return false;
}
void PetriNet::GetOptPath()
{
	if(LookUpSubproblem(_targetState))
	{
		if(_dpTable[_targetState._int]->GetHit())
			_problem->SetOptimal(_dpTable[_targetState._int]->GetSolution());
		else
		{
			KillNet();
			if(_dpTable[_targetState._int]->GetHit())
				_problem->SetOptimal(_dpTable[_targetState._int]->GetSolution());
		}
	}
}
void PetriNet::KillNet()
{
	map<int,StatePlaceNode*>::iterator it = _dpTable.begin();
	for(;it!=_dpTable.end();it++)
	{
		StatePlaceNode *curr = it->second;
		vector<ETransitionNode*> postset = curr->GetPostset();
		for(int i=0;i<postset.size();i++)
		{
			if(!postset[i]->IsActivated())
				postset[i]->UpdateKey(curr->GetName());
			else if(postset[i]->IsActivated()&&!postset[i]->HasFired())
				postset[i]->Fire();
		}

	}

}