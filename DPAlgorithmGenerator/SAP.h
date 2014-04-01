#pragma once
#include<fstream>
#include<cstdio>
#include<string>
#include "Problem.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
using namespace std;

class PetriNet;

class SAP :
	public Problem
{
public:
	SAP(string f1,string f2);
	SAP(void);
	~SAP(void);

	virtual int Reward(Decision state,Decision d);
	virtual vector<Decision> Transition(Decision state,Decision d);
	virtual std::vector<Decision> GetDecisionSpace(Decision state);

	virtual void DPFE();

	virtual void Traceback();
	virtual bool IsValidState(Decision state);
	virtual bool IsBaseState(Decision d);

	char *ReadFile(string f1,size_t &num);
	virtual SOLUTIONTYPE Base(Decision baseState);
	
	virtual std::string GetInputData();
	virtual std::string GetSolution();
	int Strncmp(Decision d);

private:
	char *_X;
	char *_Y;
	size_t _lengthX;
	size_t _lengthY;
	map<int,Decision> _answer;
};

