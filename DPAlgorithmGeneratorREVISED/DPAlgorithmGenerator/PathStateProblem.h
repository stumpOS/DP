#pragma once
#include"Problem.h"
#include "Graph.h"
/*
Different classes of DP problems may be characterized by how the states S 
and next states S' are defined. In the PathProblem problems, the state is 
the sequence of states made thus far, and the transition function is the 
union operator that adds the decision to the state set.
The DPFE of a path-state problem has the form

	f(S) = OPT(d not in S){f(t(S,d)) o r(S,d)}

where r, the reward function, is the weight of the edge from
the current (the last vertex of S) to the decision d and t is
the translation function defined as t(S,d) = S union {d}
*/
class PathStateProblem: public Problem
{
public:
	PathStateProblem();
	virtual ~PathStateProblem();
	virtual double Reward(Decision d);
	virtual Decision Transition(Decision d);
	virtual std::vector<Decision> GetDecisionSpace(Decision state);
	virtual void DPFE();
	virtual std::string GetInputData(){return "todo";}
	Graph::Vertex* GetCurrentVertex();
	virtual std::string GetSolution(){return "";}
	virtual void Traceback(){}

	virtual int Reward(Decision state,Decision d){return 0;}
	virtual Decision Transition(Decision state,Decision d){return d;}

private:
	Graph *_graph;
	
};
