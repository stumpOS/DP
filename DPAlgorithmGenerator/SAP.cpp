#include "SAP.h"
#include "PetriNet.h"

SAP::SAP(string filestr1,string filestr2)
{
	size_t N,M;
	_X = ReadFile(filestr1,N);
	_Y = ReadFile(filestr2,M);
	_lengthX = N;
	_lengthY = M;

	Decision final,target;
	final._Pair._shortHi = (short)(N);
	final._Pair._shortLo = (short)(M);

	SetOpt(MIN);
	
	if(M>0&&N>0)
		_net->CreateNet(final);

}

SAP::SAP(void)
{
}


SAP::~SAP(void)
{
	delete _X;
	delete _Y;
}
/*
returns the cost of making decision d to arrive at state 'state'
note that if state is a match state, the cost of the decision is 0
*/
int SAP::Reward(Decision state, Decision d)
{
	if(Strncmp(state)==0)
		return 0;
	else
		return 1;
	
}
vector<Decision> SAP::Transition(Decision state,Decision d)
{
	vector<Decision> states;
	short i = d._Pair._shortHi;
	short j = d._Pair._shortLo;
	if(i==0 && j==1)
	{
		state._Pair._shortHi-=1;
		state._Pair._shortLo-=1;
	}
	else if(i==0 && j==0)
		state._Pair._shortLo-=1;
	else if(i==1 && j==1)
		state._Pair._shortHi-=1;
	states.push_back(state);
	return states;
}
std::vector<Decision> SAP::GetDecisionSpace(Decision state)
{
	vector<Decision> s;
	short i = state._Pair._shortHi;
	short j = state._Pair._shortLo;
	if(i<0||j<0)
		return s;

		Decision stateMoveNW = DecisionHandler::ConstructFromPair(i-1,j-1);
		Decision stateMoveL = DecisionHandler::ConstructFromPair(i,j-1);
		Decision stateMoveUp = DecisionHandler::ConstructFromPair(i-1,j);
		Decision d0,d1,d2;

		if(IsValidState(stateMoveNW))
		{
			d0 = DecisionHandler::ConstructFromPair(0,1);
			s.push_back(d0);
		}
		if(IsValidState(stateMoveL))
		{
			d1 = DecisionHandler::ConstructFromPair(0,0);
			s.push_back(d1);
		}
		if(IsValidState(stateMoveUp))
		{
			d2 = DecisionHandler::ConstructFromPair(1,1);
			s.push_back(d2);
		}
	return s;
}
void SAP::DPFE()
{
	_net->GetOptPath();
}
SOLUTIONTYPE SAP::Base(Decision baseState)
{
	
	short i = baseState._Pair._shortHi;
	short j = baseState._Pair._shortLo;
	if(i==0&&j>0)
		return j;
	else if(j==0&&i>0)
		return i;
	else
		return 0;	
}
bool SAP::IsBaseState(Decision state)
{
	short i = state._Pair._shortHi;
	short j = state._Pair._shortLo;
	if((i==0&&j>=0)||(j==0&&i>=0))
		return true;
	else
		return false;
}
std::string SAP::GetInputData()
{
	char buff[200];
	sprintf(buff,"length of file 1 is %d and length of file 2 is %d",_lengthX,_lengthY);
	return buff;
}
	
std::string SAP::GetSolution()
{
	char buff[200];
	sprintf(buff,"there are %d entries in the dp table",_net->GetSizeOfDPTable());
	return buff;
}
void SAP::Traceback()
{
	ofstream results("results.txt");
	map<int,Decision>::iterator it;
	/*
	map<int,Decision>::iterator it = _dpTraceback.begin();
	for(;it!=_dpTraceback.end();it++)
	{
		Decision d;
		d._int = it->first;
		short i = d._Pair._shortHi;
		short j = d._Pair._shortLo;

		Decision choice = it->second;
		short ci = choice._Pair._shortHi;
		short cj = choice._Pair._shortLo;
		string dec = "";
		if(cj==1&&ci==0)
			dec = "dec both x and y";
		else if(cj==0&&ci==0)
			dec = "dec y";
		else if(cj==1&&ci==1)
			dec = "dec x";
		results << i << "," << j << ":" <<  "(" << *(_X+(i-1)) << "," << *(_Y+(j-1)) << "), from op " << ci << "," << cj << "(" << dec << ")" << endl;
	}*/
	Decision curr, prev;
	int length;
	if(_lengthX>_lengthY)
		length = _lengthX + _optimal +1;
	else
		length = _lengthY + _optimal +1;
	char *buffX = new char[length];

	char *buffY = new char[length];
	int ptr = length-2;
	it = _dpTraceback.find(DecisionHandler::ConstructFromPair(_lengthX,_lengthY)._int);
	while(it!=_dpTraceback.end())
	{
		curr._int = it->first;
		short adi = curr._Pair._shortHi;
		short adj = curr._Pair._shortLo;
		if(adi<1||adj<1||adi>=_lengthX+_optimal+1||adj>=_lengthY+_optimal+1||ptr<0)
			break;
		if(!IsValidState(curr))
			break;
		else if(curr._int==0)
			break;

		Decision choice = it->second;
		short hi = choice._Pair._shortHi;
		short lo = choice._Pair._shortLo;
		prev = curr;
		if(hi==0 && lo==1)
		{
			curr = DecisionHandler::ConstructFromPair(adi-1,adj-1);
			buffX[ptr] = *(_X+adi-1);
			buffY[ptr] = *(_Y+adj-1);
			
		}
		else if(hi==1 && lo==1)
		{
			curr = DecisionHandler::ConstructFromPair(adi-1,adj);

			buffX[ptr] = *(_X+adi-1);
			buffY[ptr] = '-';
		}
		else if(hi==0 && lo==0)
		{
			curr = DecisionHandler::ConstructFromPair(adi,adj-1);
			buffY[ptr] = *(_Y+adj-1);
			buffX[ptr] = '-';
		}
		ptr--;
		_dpTraceback.erase(prev._int);
		it = _dpTraceback.find(curr._int);
	}
	buffY[length-1] = '\0';
	buffX[length-1] = '\0';
	if(ptr>0)
		results << buffX+ptr << endl << buffY+ptr;
	else
		results << buffX << endl << buffY;
	results.close();
	delete[] buffX;
	delete[] buffY;
}

char* SAP::ReadFile(string f1, size_t &num)
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
bool SAP::IsValidState(Decision state)
{
	short i = state._Pair._shortHi;
	short j = state._Pair._shortLo;
	if(i<0||i>_lengthX||j<0||j>_lengthY)
		return false;
	else
		return true;
}
int SAP::Strncmp(Decision d)
{
	return strncmp(_X+(d._Pair._shortHi-1),_Y+(d._Pair._shortLo-1),1);
}