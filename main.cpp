#define _CRT_SECURE_NO_WARNINGS

#include "general.h"
#include "cGraph.h"
#include "Graph.h"
#include "Vertex.h"
#include "Edge.h"







using namespace std;

void testGraph1(Graph* G);
void testGraph2(Graph* G);
void testGraph3(Graph* G);
void testGraph4(Graph* G);
void testGraph5(Graph* G);

//this seems always active
int main()
{
	Graph* G = new Graph();
	testGraph2(G);
	
	//Graph Voodoo
	G->identifyVertices();								
	G->cleanGraph();
	G->createDualVertices();

	G->printGraph(); 

	//cGraph Voodoo
	cGraph* cG = new cGraph(G);							
	G->assignWeights(cG->vertices);

	G->printGraph();
	G->printEdgeList();



	//####################################################
	//####################################################
	//####################################################
	//####################################################
/*
	Graph* G = new Graph("DATA.txt");
	cGraph* cG = new cGraph(G);
	G->assignWeights(cG->vertices);
*/

	return 1;
}
	
void testGraph1(Graph* G){

	shared_ptr<Vertex> A (new Vertex(1) );
	shared_ptr<Vertex> B (new Vertex(2) );
	shared_ptr<Vertex> C (new Vertex(3) );
	shared_ptr<Vertex> D (new Vertex(4) );
	shared_ptr<Vertex> E (new Vertex(5) );
	
	A->addConnection(2,strongEdge);
	A->addConnection(3,strongEdge);
	A->addConnection(4,strongEdge);
	A->addConnection(5,strongEdge);

	B->addConnection(1,strongEdge);
	C->addConnection(1,strongEdge);
	D->addConnection(1,strongEdge);
	E->addConnection(1,strongEdge);

	G->addVertex(A);
	G->addVertex(B);
	G->addVertex(C);
	G->addVertex(D);
	G->addVertex(E);

}

void testGraph2(Graph* G){

	//Create vertices
	shared_ptr<Vertex> one	 (new Vertex(1) );			
	shared_ptr<Vertex> two	 (new Vertex(2) );
	shared_ptr<Vertex> three (new Vertex(3) );
	shared_ptr<Vertex> four  (new Vertex(4) );
	shared_ptr<Vertex> five  (new Vertex(5) );
	
	//Create edges
	one->addConnection(two,strongEdge);					
	one->addConnection(three,strongEdge);
	two->addConnection(one,strongEdge);
	two->addConnection(four,weakEdge);
	two->addConnection(five,weakEdge);
	three->addConnection(one,strongEdge);
	three->addConnection(four,weakEdge);
	four->addConnection(two,weakEdge);
	four->addConnection(three,weakEdge);
	five->addConnection(two,weakEdge);

	//Add to Graph				
	G->addVertex(one);
	G->addVertex(two);
	G->addVertex(three);
	G->addVertex(four);
	G->addVertex(five);

}

void testGraph3(Graph* G){

	//Create vertices
	shared_ptr<Vertex> one	 (new Vertex(10) );			
	shared_ptr<Vertex> two	 (new Vertex(20) );
	shared_ptr<Vertex> three (new Vertex(30) );
	shared_ptr<Vertex> four  (new Vertex(40) );
	shared_ptr<Vertex> five  (new Vertex(50) );

	//Create edges
	one->addConnection(two,strongEdge);
	one->addConnection(three,strongEdge);
	one->addConnection(four,strongEdge);
	
	two->addConnection(one,strongEdge);
	two->addConnection(five,weakEdge);

	three->addConnection(one,strongEdge);
	three->addConnection(five,weakEdge);

	four->addConnection(one,strongEdge);
	four->addConnection(five,weakEdge);

	five->addConnection(two,weakEdge);
	five->addConnection(three,weakEdge);
	five->addConnection(four,weakEdge);

	//Add to Graph

	G->addVertex(one);
	G->addVertex(two);
	G->addVertex(three);
	G->addVertex(four);
	G->addVertex(five);

}

void testGraph4(Graph* G){

	//Create vertices
	shared_ptr<Vertex> one	 (new Vertex(10) );			
	shared_ptr<Vertex> two	 (new Vertex(20) );
	shared_ptr<Vertex> three (new Vertex(30) );
	shared_ptr<Vertex> four  (new Vertex(40) );
	shared_ptr<Vertex> five  (new Vertex(50) );

	//Create edges
	one->addConnection(two,strongEdge);
	one->addConnection(three,strongEdge);
	one->addConnection(four,strongEdge);
	
	two->addConnection(one,strongEdge);
	two->addConnection(five,weakEdge);
	two->addConnection(three,strongEdge);

	three->addConnection(one,strongEdge);
	three->addConnection(two,strongEdge);
	three->addConnection(four,strongEdge);
	three->addConnection(five,weakEdge);

	four->addConnection(one,strongEdge);
	four->addConnection(five,weakEdge);
	four->addConnection(three,strongEdge);

	five->addConnection(two,weakEdge);
	five->addConnection(three,weakEdge);
	five->addConnection(four,weakEdge);

	//Add to Graph

	G->addVertex(one);
	G->addVertex(two);
	G->addVertex(three);
	G->addVertex(four);
	G->addVertex(five);




}

void testGraph5(Graph* G){

	//Create vertices
	shared_ptr<Vertex> one	 (new Vertex(10) );			
	shared_ptr<Vertex> two	 (new Vertex(20) );
	shared_ptr<Vertex> three (new Vertex(30) );
	
	shared_ptr<Vertex> five  (new Vertex(50) );

	//Create edges
	one->addConnection(three,strongEdge);
	

	two->addConnection(three,strongEdge);
	two->addConnection(five,weakEdge);

	three->addConnection(one,strongEdge);
	three->addConnection(two,strongEdge);
	three->addConnection(five,weakEdge);

	

	five->addConnection(two,weakEdge);
	five->addConnection(three,weakEdge);


	//Add to Graph
	G->addVertex(one);
	G->addVertex(two);
	G->addVertex(three);

	G->addVertex(five);
}