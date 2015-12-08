#ifndef GRAPH_H
#define GRAPH_H

#include "general.h"
#include "Vertex.h"
#include "Edge.h"

#include "cVertex.h"

using namespace std;
//typedef unordered_map<Vertex*,uint32_t> Map;

typedef list<shared_ptr<Edge> >::iterator   Edge_iterator;
typedef list<shared_ptr<Vertex> >::iterator Vertex_iterator;


class Graph
{

	friend class cGraph;
    public:

        //Constructors
        Graph(string PATH, string _name = "Base Graph");
        Graph();

        //Destructors
        ~Graph();

        //Accessors
        uint32_t getV();


        //Mutators
        bool setV(uint32_t V);
        void setName(string newName);


		//Methods
		void assignWeights(vector<cVertex* >& cVertexSet);	

	public:
        //Methods
        bool addVertex(tuple<float,float>,uint32_t _ID , State _myState );
        bool addVertex(shared_ptr<Vertex> u);
        void findConnections(shared_ptr<Vertex> u);
        Vertex_iterator removeVertex(Vertex_iterator it);
        void printGraph();
        shared_ptr<Vertex> findVertex(uint32_t _ID);
		bool isCoop(shared_ptr<Vertex> ver);
		bool isSlave(shared_ptr<Vertex> ver);
		bool isRealyCoop(shared_ptr<Vertex> ver);
		void identifyVertices(); 
		void cleanGraph();
		void createDualVertices();
		void removeWeakEdges(shared_ptr<Vertex>);
		void findAndSortVertices(list<shared_ptr<Vertex> >& set,State searchedState); 
		void findAdjacentVertices(list<shared_ptr<Vertex> >& set,shared_ptr<Vertex> ver,edgeType lookedType);
		void createDualVertex(shared_ptr<Vertex> CommonNeighbour, shared_ptr<Vertex> verA,shared_ptr<Vertex> verB,shared_ptr<Vertex> coopVer );
		shared_ptr<Graph> createcGraph();
		void finalClean();
		int  getNumOfEdges();
		void assignWeights(cGraph& cG);
		void finalTouch();
		void printEdgeList();

		//void findDualAndCoopVerticesInGraph(vector<uint32_t> 


    private:
        string        name;
        list<shared_ptr<Vertex> > adjList;
};


#endif // GRAPH_H
