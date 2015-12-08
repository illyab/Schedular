
#ifndef cGraph_H
#define cGraph_H


#include "general.h"
#include "Graph.h"
#include "cVertex.h"






class cGraph
{
	public:

		friend void assignWeights(cGraph* cG);

		//Constructors
		cGraph(Graph* G);
		cGraph();

		//Destructor
		~cGraph();

		void price_cVertices(list<shared_ptr<Vertex> >& baseGraphList);

		
	private:
		//Methods

		cVertex* cGraph::find_cVertex(int src,int dst);
		cVertex* edgeTo_cVertex(int src,int dst);
		void EdgesToVerticesConverter(Graph& G);
		void findAllConflictsInGraph(Graph& G);
		cVertex* findBySource(int _src);
		cVertex* findByDestintation(int _dst);
		void d2un(vector<pair<int,int> >& edgeVec);
		void inducedGraphByPrimar(int primarID,vector<pair<int,int> >& edgesVec,int& V,int& E,vector<int>& inducedSet);
		int findMaxCliqueInInducedGraph(vector<pair<int,int> >& edgeVec,vector<int>&,int V,int E);
		void adjustVerticesUIDs(vector<pair<int,int> >& edgeVec,vector<int>& inducedSet,int V);
		void TextToBinary(string input,string output);
		void removeSameEdges(vector<pair<int,int> >& edgesVec);
		int qualix(string dimcasFilename);


	private:
		int V;
		vector<vector<bool> > adjMatrix;

	public:
		vector<cVertex* > vertices;
};




#endif





