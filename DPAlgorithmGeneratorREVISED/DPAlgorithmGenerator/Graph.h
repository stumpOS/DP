#ifndef GRAPH_H_
#define GRAPH_H_

#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<stdio.h>
#include<cstdlib>
using namespace std;

class Graph
{
public:
	Graph(void);
	virtual ~Graph(void);
	void Initialize(string graph);
	int GetOrder(){return mAdjacencyLists.size();}
	
	class Vertex
	{
	public:
		Vertex(int i){mVisited = false; mName = i;}
		Vertex(){}
		virtual ~Vertex(){}
		struct EdgeWeight
		{
			EdgeWeight(int e, int w):end(e),weight(w){}
			EdgeWeight(){end = 0;weight=0;}
			~EdgeWeight(){}
			int end;
			int weight;
		};
		
		void AddNeighbor(EdgeWeight x){mOutNeighbors.push_back(x);}
		int GetName(){return mName;}
		bool GetVisited(){return mVisited;}
		void SetVisited(bool visit){mVisited = visit;}
		int GetOutDegree(){return mOutNeighbors.size();}
		EdgeWeight* GetEdgeWeight(int u)
		{
			for(int i=0;i<mOutNeighbors.size();i++)
			{
				if(mOutNeighbors[i].end==u)
					return &mOutNeighbors[i];
			}
			return NULL;
		}

	
	private:
		int mName;
		vector<EdgeWeight> mOutNeighbors;
		bool mVisited;
	};
	Vertex* GetVertex(int index);
private:
	vector<Vertex> mAdjacencyLists;
};
#endif