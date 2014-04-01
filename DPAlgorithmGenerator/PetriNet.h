#pragma once
#include<vector>
#include<map>
#include<cmath>
#include<bitset>
#include"Problem.h"
using namespace std;

#define NBHD_LIMIT 15
typedef short KEYTYPE;

class PetriNet;
class Node
{
public:
	Node(PetriNet *net){_net = net;}
	virtual ~Node(){}
	virtual void Dereference() = 0;
	
protected:
	
	PetriNet *_net;
private:
	// a node without a net has no meaning - disable trying to instantiate solo nodes!
	Node();
};

// forward declarations so we can define these in placenodes
class TransitionNode;
class MTransitionNode;
class ETransitionNode;
/*!
 * a PlaceNode will hold the return value of a transitionNode's
 * expression after that TransitionNode has been fired.
 * A place is either a state place or an intermediate place
*/
class PlaceNode:public Node
{
public:
	PlaceNode(PetriNet *net):Node(net){_hit = false;_solution = 0;_name=5;}
	
	virtual ~PlaceNode(){}

	void SetHit(bool h){_hit = h;}
	void SetName(int n){_name = n;}
	unsigned int GetName(){return _name;}
	bool GetHit(){return _hit;}
	int GetSolution(){return _solution;}
	// derived classes must take additional action when solition is set
	virtual void SetSolution(int s) = 0;
	virtual void Dereference()=0;
	Decision GetState(){return _state;}
protected:
	//! a variable to track whether or not its preceding 
	// transition node has sent it an return value
	bool _hit;
	int _solution;
	unsigned int _name;
	Decision _state;
};

/*!
* a stateplacenode is a place that holds the solution to
* a subproblem defined by state _state
*/
class StatePlaceNode:public PlaceNode
{
public:
	StatePlaceNode(PetriNet *net):PlaceNode(net){_pred = NULL;}
	
	StatePlaceNode(PetriNet *net,Decision s):PlaceNode(net){_pred = NULL;_state = s;}

	virtual ~StatePlaceNode(){}
	void SetPred(MTransitionNode *p){_pred = p;}
	void AddTransitionToPostset(ETransitionNode *etn){_postset.push_back(etn);}

	Decision GetState(){return _state;}
	
	virtual void SetSolution(int s);
	virtual void Dereference();
	vector<ETransitionNode*> GetPostset(){return _postset;}
private:
	MTransitionNode *_pred;
	vector<ETransitionNode*> _postset;
	//Decision _state;
};
/*!
* an iplaceNode is a place that holds the solution to an
* optimization expression evaluated by an mtransitionnode.
* Its computed value represents an elgible decision in the 
* decision space of a subproblem
* the '_state' of an iplacenode refers to its decision 
*/
class IPlaceNode:public PlaceNode
{
public:
	
	IPlaceNode(PetriNet *net):PlaceNode(net){_pred = NULL;_succ=NULL;}
	IPlaceNode(PetriNet *net,MTransitionNode *m):PlaceNode(net),_succ(m){_pred = NULL;}
	virtual ~IPlaceNode(){}
	int GetDecisionIntValue(){return _state._int;}
	void SetPred(ETransitionNode *p){_pred = p;}
	void SetSucc(MTransitionNode *p){_succ = p;}
	virtual void SetSolution(int s);
	void SetDecision(Decision d){_state = d;}
	virtual void Dereference();
private:
	ETransitionNode *_pred;
	MTransitionNode *_succ;
	//Decision _decision;
};
/*!
 * a transitionNode represents an expression to be evaluated when the
 * transition is fired. 
*/
class TransitionNode:public Node
{
public:
	
	TransitionNode(PetriNet *net):Node(net){_fireCounter = 0;_isActivated = false;_hasFired = false;_targetPlace = NULL;}
	virtual ~TransitionNode(){}
	virtual void Fire() = 0;
	void UpdateKey(unsigned int n);
	virtual void Dereference();
	bool HasFired(){return _hasFired;}
	bool IsActivated(){return _isActivated;}
protected:
	void AddPlaceToPreset(PlaceNode* pn);

	bool _isActivated;
	bool _hasFired;
	// firekey indicates the places in the transitionNode's preset
	// these are the places whose solutions must be set in order for
	// the transitionNode to be activated. Each placeholder k 
	// (0 <= k <= NBHD_LIMIT) in the bit set corresponds to a placeNode.
	// That placeNode's name is a hexidecimal integer equal to 2^k base 10 
	unsigned int _firekey;
	PlaceNode* _targetPlace;
	map<unsigned int, PlaceNode*> _presetMap;
	int _fireCounter;
};
class ETransitionNode:public TransitionNode
{
public:
	ETransitionNode(PetriNet *net):TransitionNode(net){_reward=0;}
	ETransitionNode(PetriNet *net, IPlaceNode *ipn, int r);
	virtual ~ETransitionNode(){}
	virtual void Fire();
	void SetReward(int r){_reward = r;}
	void AddStatePlaceNodeToPreset(StatePlaceNode *spn);
	
private:
	//IPlaceNode *_targetPlace;
	//map<unsigned int, StatePlaceNode *> _presetMap;
	int _reward;
};
class MTransitionNode:public TransitionNode
{
public:
	MTransitionNode(PetriNet *net):TransitionNode(net){_targetPlace = NULL;}
	MTransitionNode(PetriNet *net,StatePlaceNode *spn);
	virtual ~MTransitionNode(){}
	virtual void Fire();
	void AddPlaceToPreset(IPlaceNode *ipn);
	
//private:
	//StatePlaceNode *_targetPlace;
	//map<unsigned int, IPlaceNode *> _presetMap;
};
	
class PetriNet
{
public:
	PetriNet(Problem *p);
	~PetriNet(void);
	void CreateNet(Decision targetState);
	bool LookUpSubproblem(int key, int &solution);
	void EmptyTheNet();
	void GetOptPath();

	bool LookUpSubproblem(Decision key, int &solution);
	bool LookUpSubproblem(Decision state);
	
	void CreateSegment(StatePlaceNode *spn);
	Problem *GetProblem(){return _problem;}
	void KillNet();
	int GetSizeOfDPTable(){return _dpTable.size();}
private:
	PetriNet(){}
	vector<Node *> _nodes;
	map<int,StatePlaceNode*> _dpTable;
	Problem *_problem;
	// TODO : haltstate should be a set of base cases
	// and is problem dependent
	
	Decision _targetState;
};

