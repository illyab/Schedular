#ifndef VERTEX_H
#define VERTEX_H

#include "general.h"
#include "Edge.h"

typedef list<shared_ptr<Edge> >::iterator  Edge_iterator;


class Vertex
{
    public:
        //Constructors:
        Vertex();
        Vertex(uint32_t _ID,tuple<float,float> _poition=make_tuple(0,0), State _myState=slave);
        Vertex(string data);
		Vertex(int _ID);

        //Destructors:
        virtual ~Vertex();

        //Accessors:
        virtual uint32_t getID();
		virtual string getIdAsString();
        State getState();
		string getStateAsString();
        tuple<float,float> getPosition();
        pair<list<shared_ptr<Edge> >::iterator,list<shared_ptr<Edge> >::iterator> getAdjacent();
		list<shared_ptr<Edge> >& getAdjacentSet();
        //Mutetors:
        void setID(uint32_t _ID);
        void setState(State newState);
        void setPosition(tuple<float,float> newPosition);

		//Operators
		bool operator==( const Vertex& other ) const;
		bool operator<( const Vertex& other ) const;


        //Methods:
        void    addConnection(shared_ptr<Edge> newEdge);
		void	addConnection(uint32_t to , edgeType myType);
		void    addConnection(shared_ptr<Vertex> toAdd,edgeType type );
        bool    isAdjacent(uint32_t vertexID);
		bool    isAdjacent(shared_ptr<Vertex> u);
		Edge_iterator removeEdge(list<shared_ptr<Edge> >::iterator it);
		Edge_iterator removeEdge(uint32_t toRemove);
		void printEdgeList();

	protected:
		uint32_t receiveUniqueID();


	public:
		list<shared_ptr<Edge> >   adjacent;
    protected:
        uint32_t                  ID;
        State                     myState;
        tuple<float,float>        position;   // (X,Y) coordinates
        
};



class DualVertex : public Vertex
{
	friend class ConflictGraph;
	public:

		uint32_t dualID[2]; 
		vector<uint32_t> sources;
		uint32_t destinationCoop;
	
	

		//Destructors:
        ~DualVertex();

		//Constructores
		DualVertex(shared_ptr<Vertex> u,shared_ptr<Vertex> v);
		DualVertex();
		DualVertex(vector<uint32_t>& __sources, shared_ptr<Vertex> verA,shared_ptr<Vertex> verB,shared_ptr<Vertex> coopVer );

		//Accessors 
		string getIdAsString();
		uint32_t getID();
		uint32_t getDestinationCoop();
};

#endif // VERTEX_H

