#include "LCS.h"


LCS::LCS(string filestr1,string filestr2)
{	
	size_t N,M;
	_X = ReadFile(filestr1,N);
	_Y = ReadFile(filestr2,M);
	_Z = NULL;	
}
char* LCS::ReadFile(string f1, size_t &num)
{
	ifstream str(f1.c_str(),ifstream::binary);
	if(!str)
		return NULL;
	filebuf* pbuf = str.rdbuf();

	size_t size = pbuf->pubseekoff(0,str.end,str.in);
	pbuf->pubseekpos(0,str.in);
	char *buffer = new char[size+1];
	pbuf->sgetn(buffer,size);
	buffer[(unsigned)size] = '\0';
	num = size;
	str.close();
	return buffer;
}

LCS::~LCS(void)
{
	if(_X!=NULL)
		delete[] _X;
	if(_Y!=NULL)
		delete[] _Y;
	if(_Z!=NULL)
		delete[] _Z;
}

Decision LCS::Transition(Decision d)
{
	return d;
}
Decision LCS::Transition(short i,short j,Decision d)
{
	Decision newState;
	if(d._Pair._shortHi==1&&d._Pair._shortLo==1)
	{
		newState._Pair._shortHi = i-1;
		newState._Pair._shortLo = j-1;
	}
	else if(d._Pair._shortHi==1&&d._Pair._shortLo==0)
	{
		newState._Pair._shortHi = i-1;
		newState._Pair._shortLo = j;
	}
	else if(d._Pair._shortHi==0&&d._Pair._shortLo==1)
	{
		newState._Pair._shortHi = i;
		newState._Pair._shortLo = j-1;
	}
	return newState;
}
/*!
 * The decision here is whether to consider a smaller x (i--),
* smaller y string (j--), or smaller both (i-- and j--). Assuming
* we have already made that decision, the reward is 1
* if X[i] == Y[j] and 0 otherwise
*/
double LCS::Reward(Decision d)
{
	/*
	if(strncmp(_X+d._Pair._shortHi,_Y+d._Pair._shortLo,1)==0)
		return 1;
	else
		return 0;
	*/
	if(d._Pair._shortHi==1&&d._Pair._shortLo==1)
		return 1;
	else
		return 0;
}
std::vector<Decision> LCS::GetDecisionSpace(Decision state)
{
	std::vector<Decision> space;
	return space;
}
void LCS::DPFE()
{
	int c=0;
	DPFE(strlen(_X)-1,strlen(_Y)-1,c);
	Problem::_optimal = (double)c;
}
std::string LCS::GetInputData()
{
	if(_X==NULL||_Y==NULL)
		return "";
	
	int M = strlen(_X);
	int N = strlen(_Y);
	char buff[110];
	sprintf(buff,"X is length %d and\n Y is length %d",M,N);
	return buff;
}

void LCS::Traceback()
{
	int len = (int)Problem::_optimal;
	if(len==0)
		return;
	if(_Z==NULL)
		_Z = new char(len+1);

	int i = strlen(_X),j = strlen(_Y),limit = 0;
	ofstream result("results.txt");
	while(i>0&&j>0 && limit <= len)
	{
		Decision key;
		key._Pair._shortHi = i;
		key._Pair._shortLo = j;
		Decision d;
		map<int,int>::iterator it= LinearProblem::_dpTraceback.find(key._int);
		if(it!=LinearProblem::_dpTraceback.end())
		{
			d._int = it->second;
		}
		else
		{
			i--;
			j--;
			continue;
		}
		if(d._Pair._shortHi==1 && d._Pair._shortLo == 1)
		{
			int index = len-limit-1;
			_Z[index] = *(_X +i);
			i--;
			j--;
			limit++;
		}
		else if(d._Pair._shortHi==0)
			j--;
		else
			i--;
	}
	_Z[len] = '\0';
	result << _Z;
	result.close();
}
std::string LCS::GetSolution()
{
	std::string s;
	if(_Z==NULL)
		s = "";
	else
		s = _Z;
	return s;
}
/*!
* Return true if solution has been previously computed. store result in
* the 'cost' variable
*/
bool LCS::LookUpSubProblem(short i,short j,int &cost,Decision &in)
{
	in._Pair._shortHi=i;
	in._Pair._shortLo=j;
	map<int,int>::iterator it = LinearProblem::_dpMap.find(in._int);
	if(it!=LinearProblem::_dpMap.end())
	{
		cost = it->second;
		return true;
	}
	return false;
}
/*!
The dpfe can be expressed as 
f(X[i],Y[j]) 
	= 0												if i==0 or j==0
	= max(d in D(X[i],Y[j])){f(t(X[i],Y[j],d)
		+ Reward(X[i],Y[j],d)}						otherwise
here the decision is either to decrement i, j, or both.

return true when a terminal state is reached
*/
void LCS::DPFE(short i, short j, int &cost)
{
	Decision currState;
	//did we already compute it?
	if(LookUpSubProblem(i,j,cost,currState))
		return;
	
	//is it a base condition?
	if(i<0||j<0)
	{
		LinearProblem::_dpMap[currState._int] = 0;
		return;
	}
	else
	{
		// Now loop the decision space based on current state
		// f(i,j) = f(t(i,j,d)) + Reward(i,j,d) = f(nextState) + Reward(currState)
		// we are using 1-indexing so decrement 1 from both indices to reflect this
		vector<Decision> space = GetDecisionSpace(i,j);
		if(space.size()==0)
		{
			return;
		}
		// if *(_X+i)==*(_Y+j), then space[0] is dec both i&j,
		//|space| = 1, and reward(space[0])=1
		Decision nextState = Transition(i,j,space[0]);
		int maxCost = 0;
		//maxCost+=Reward(space[0]);
		DPFE(nextState._Pair._shortHi,nextState._Pair._shortLo,maxCost);
		LinearProblem::_dpMap[currState._int] = maxCost + Reward(space[0]);
		LinearProblem::_dpTraceback[currState._int] = space[0]._int;
		for(int k=1;k<space.size();k++)
		{
			nextState = Transition(i,j,space[k]);
			int c = cost;
			DPFE(nextState._Pair._shortHi,nextState._Pair._shortLo,c);
			c+=Reward(space[k]);

			if(c>LinearProblem::_dpMap[currState._int])
			{
				LinearProblem::_dpMap[currState._int] = c;
				// the decision is of the form 01 or 10 and indicates whether j or i was decremented
				// 01 is analogous to a left arrow and 10 is analogous to an up arrow
				LinearProblem::_dpTraceback[currState._int] = space[k]._int;
			}
				
		}
		
	}
	cost = LinearProblem::_dpMap[currState._int];

}
//TODO: make this better - fixed array of size max D with bools?
vector<Decision> LCS::GetDecisionSpace(short i,short j)
{
	vector<Decision> space;

	// if we've reached the end of both strings, terminate
	if((*(_X+i)=='\0'&&*(_Y+j)=='\0')||(i<0||j<0))
		return space;
	// if x[i] == y[j], then D = {(1,1)} (i.e. dec both)
	else if(strncmp(_X+i,_Y+j,1)==0)
	{
		Decision d;
		d._Pair._shortHi = 1;
		d._Pair._shortLo = 1;
		space.push_back(d);
	}
	// otherwise, D = {(0,1),(1,0)} (i.e. dec either i or j)
	else
	{
		Decision d1;
		d1._Pair._shortHi = 1;
		d1._Pair._shortLo = 0;
		Decision d2;
		d2._Pair._shortHi = 0;
		d2._Pair._shortLo = 1;
		space.push_back(d1);
		space.push_back(d2);
	}
	return space;
}
int LCS::Reward(short i, short j)
{
	if(strncmp(_X+i,_Y+j,1)==0)
		return 1;
	else
		return 0;
}