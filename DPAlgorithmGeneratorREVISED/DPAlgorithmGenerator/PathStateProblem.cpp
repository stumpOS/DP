#include "PathStateProblem.h"


PathStateProblem::PathStateProblem(void)
{
}


PathStateProblem::~PathStateProblem(void)
{
}
/*!
The decision in a path-state problem is the next vertex we choose in the path.
The reward function in a path-state problem is the weight of 
the edge from the current to the next vertex
*/
double PathStateProblem::Reward(Decision d)
{
	if(d._int >= _graph->GetOrder())
		return 0;

	Graph::Vertex *v = GetCurrentVertex();
	if(v==NULL)
		return 0;
	Graph::Vertex::EdgeWeight *w = v->GetEdgeWeight(d._int);
	if(w!=NULL)
		return w->weight;
	else
		return 0;
}
/*!
*The new state is simply the old state plus the new vertex
*(extend the path)
*/
Decision PathStateProblem::Transition(Decision d)
{
	_state.push_back(d);
	return d;
}
Graph::Vertex* PathStateProblem::GetCurrentVertex()
{
	if(_state.size()<1)
		return NULL;
	else
	{
		int curr = Problem::_state.size()-1;
		return _graph->GetVertex(Problem::_state[curr]._int);
	}
}
/*!
* The decision space is the neighborhood of the current vertex
* intersected with the vertices in the path thus far
*/
std::vector<Decision> PathStateProblem::GetDecisionSpace(Decision state)
{
	std::vector<Decision> space;
	Graph::Vertex *v = GetCurrentVertex();
	// TODO: intersect S with N(v) -- bitwise operations will simpify this
	return space;
}

void PathStateProblem::DPFE()
{
}