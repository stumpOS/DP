#include "Document.h"


Document::Document(void)
{
	Parser::ParseDocPages("pagesDelimitedBySpecials.txt",_DocPages);
	_currentPageIndex = 0;
	_probType = eLCS;
	_problem = NULL;
	_displayText = "Go to File >> Open to select input";
}


Document::~Document(void)
{
}
void Document::GoTo(int i)
{
	if(i<0)
	{
		if(_currentPageIndex <= 0)
			_currentPageIndex = _DocPages.size()-1;
		else
			_currentPageIndex--;
	}
	else if(i==0)
		Next();
	else
	{
		if(i>=0 && i<_DocPages.size())
			_currentPageIndex = i;
	}
}
void Document::Next()
{
	if(_currentPageIndex < _DocPages.size())
		_currentPageIndex++;
	else
		_currentPageIndex = 0;
}
Page* Document::GetPage(int i)
{
	if(_DocPages.size()<1)
		return NULL;
	if(i< _DocPages.size())
		return &_DocPages[i];
	else
		return NULL;
}
void Document::LoadProblem(string filename)
{
	/*
	if(_problem != NULL)
		delete _problem;
	*/
}
void Document::LoadProblemTwoFiles(string f1, string f2)
{
	if(f1==""||f2=="")
		return;
	if(_problem != NULL)
	{
		_problem->ClearNet();
		delete _problem;
	}

	//LCS *prob = new LCS(f1,f2);
	SAP *prob = new SAP(f1,f2);
	_problem = prob;
	_displayText = _problem->GetInputData();
}
void Document::DP()
{
	if(_problem!=NULL)
	{
		_problem->DPFE();
		_problem->Traceback();
		double cost = _problem->GetOptimal();
		char buff[300];
		int n = sprintf(buff,"Optimal cost is %f\n", cost);
		_displayText = buff;
	}
	else
	{
		_displayText = "problem is NULL";
	}
	
}
