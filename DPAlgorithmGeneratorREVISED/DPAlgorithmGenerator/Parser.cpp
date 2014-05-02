#include "Parser.h"


Parser::Parser(void)
{
}


Parser::~Parser(void)
{
}


void Parser::ParseDocPages(string pagesFile,vector<Page> &result)
{
	
	ifstream pages(pagesFile.c_str());

	if(!pages)
		return;
	while(!pages.eof())
	{
		string line;
		getline(pages,line,'@');
		char *page = new char[line.length()+1];
		strcpy(page,line.c_str());
		char *curr = strtok(page,"#");
		Page p;
		if(curr!=NULL)
		{
			p.sTitle = curr;
			curr = strtok(NULL,"@");
			if(curr!=NULL)
				p.sBody = curr;
		}
		if(p.sTitle!="")
			result.push_back(p);
	}
	pages.close();
}
void Parser::ParseUserDPInput(string input, Problem& problem)
{

}

