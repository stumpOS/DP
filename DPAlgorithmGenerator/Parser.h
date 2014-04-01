#pragma once
#ifndef PARSER_H_
#define PARSER_H_
#include<string>
#include<stdlib.h>
#include<fstream>
#include"Problem.h"
#include<vector>
using namespace std;

class Page{
public:
	Page():sTitle(""),sBody(""){}
	~Page(){}
	string sTitle;
	string sBody;
};
class Parser
{
public:
	Parser(void);
	~Parser(void);
	static void ParseDocPages(string file,vector<Page> &result);
	static void ParseUserDPInput(string input,Problem &problem);
	
};
#endif
