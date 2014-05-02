#pragma once
#include<fstream>
#include<cstdio>
#include<string>
#include "LinearProblem.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
using namespace std;

class LCS : public LinearProblem
{
public:
	LCS(string f1, string f2);
	virtual ~LCS(void);

	virtual double Reward(Decision d);
	int Reward(short i,short j);

	bool LookUpSubProblem(short i,short j, int &cost,Decision &d);
	virtual Decision Transition(Decision d);
	Decision Transition(short i,short j,Decision d);

	
	virtual std::vector<Decision> GetDecisionSpace(Decision state);
	std::vector<Decision> GetDecisionSpace(short i,short j);

	void DPFE(short i, short j, int &cost);
	virtual void DPFE();
	virtual void Traceback();

	virtual std::string GetSolution();
	virtual std::string GetInputData();
	char* ReadFile(string file,size_t &n);

	virtual int Reward(Decision state,Decision d){return 0;}
	virtual Decision Transition(Decision state,Decision d){return d;}

private:
	char *_X;
	char *_Y;
	char *_Z;
};

