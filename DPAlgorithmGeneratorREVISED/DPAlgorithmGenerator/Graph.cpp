#include "Graph.h"

Graph::Graph()
{

}
Graph::~Graph()
{
}
Graph::Vertex *Graph::GetVertex(int index)
{
	if(index < mAdjacencyLists.size())
		return &mAdjacencyLists[index];
	else
		return NULL;
}
void Graph::Initialize(string filestream)
{
	ifstream graph(filestream.c_str());
	
	if(!graph)
	{
		return;
	}
	int id = 0;
	while(!graph.eof())
	{
		string adjacencyList;
		getline(graph,adjacencyList, '\n');
		char *row = new char[adjacencyList.length()+1];
		strcpy(row, adjacencyList.c_str());
		Vertex v(id);

		char *curr = strtok(row," ");
		int currVertex = 0;
		while(curr!=NULL)
		{
			if(strcmp(curr,"*")!=0)
			{
				int neighbor = atoi(curr);
				Vertex::EdgeWeight ew(currVertex, neighbor);
				v.AddNeighbor(ew);
			}
		  curr = strtok(NULL," ");
		  currVertex++;
		}
		mAdjacencyLists.push_back(v);
		id++;
		delete [] row;
	}
}



