#include "general.h"
#include "Vertex.h"



                                        //###########################
                                        //#     Constructors        #
                                        //###########################

Vertex::Vertex(){}


Vertex::Vertex(uint32_t _ID,tuple<float,float> _poition, State _myState){

	ID=receiveUniqueID();

    position=_poition;
    myState=_myState;

}

Vertex::Vertex(int _ID){

	this->ID	   = (uint32_t)_ID;
	this->myState  = solo;
}


                                        //###########################
                                        //#       Destructors       #
                                        //###########################

Vertex::~Vertex(){

    if(DEBUG_MODE){
        cout << "DEBUG:Vertex '" << ID << "' deleted." <<endl;
    }
}

                                        //###########################
                                        //#       Accessores        #
                                        //###########################


uint32_t Vertex::getID(){ return ID;}

string Vertex::getIdAsString(){ return to_string(this-> ID);}

tuple<float,float> Vertex::getPosition(){ return position;}

State Vertex::getState(){ return myState;}

string Vertex::getStateAsString(){

	State foo = this->myState;
	switch( foo ){
		case clusterHead: return "CH";
		case relay:		  return "relay";
		case slave:		  return "slave";
		case coop:		  return "coop";
		case solo:		  return "solo";
		case dual:		  return "Dual";
		default:		  return "Unknow State!!";	
	}

}

/*
Return a pair of iterators to beggining and end of list.
*/
pair<list<shared_ptr<Edge> >::iterator,list<shared_ptr<Edge> >::iterator> Vertex::getAdjacent(){    return std::make_pair(adjacent.begin(),adjacent.end());   }


                                        //###########################
                                        //#         Mutetors        #
                                        //###########################


void Vertex::setPosition(tuple<float,float> newPosition){   this->position = newPosition;   }

void Vertex::setID(uint32_t _ID){   this->ID = _ID;    }

void Vertex::setState(State newState){   this->myState = newState;   }



                                        //###########################
                                        //#         Methods         #
                                        //###########################


void Vertex::printEdgeList(){
	
	for(Edge_iterator it = adjacent.begin(); it!=adjacent.end(); it++)
		cout<<"(" << getIdAsString() << "," << (*it)->getConnection(1) << ")" << ": "<< (*it)->getWeight() << endl;
}



uint32_t Vertex::receiveUniqueID(){

	static uint32_t ID_Generator=0;

	return ID_Generator++;

}


bool Vertex::operator==( const Vertex& other ) const  {
    return ID == other.ID;
}

bool Vertex::operator<(const Vertex& other) const{

	return (ID < other.ID );

}



void Vertex::addConnection(shared_ptr<Vertex> toAdd,edgeType type ){

	if(isAdjacent(toAdd->getID() )==false && toAdd->getID()!=this->ID ){
		shared_ptr<Edge> newEdge ( new Edge(make_tuple(this->ID,toAdd->getID(),type)) );
		this->adjacent.push_back(newEdge);
	}
}

//Adds a new connection to calling vertex.
void Vertex::addConnection(shared_ptr<Edge> newEdge){

    uint32_t vertexV_ID = newEdge->getConnection(1);
    edgeType myType = newEdge->getEdgeType();

	if(isAdjacent(vertexV_ID)==false && vertexV_ID!=this->ID  ){   //Don't add an existing edge to list and avoid self loops.
		
        adjacent.push_back(newEdge);
        if(DEBUG_MODE){

            string str = (myType==strongEdge) ? ("strong edge") : ("weak edge");
            cout << "DEBUG: Generated "<< str << "(" << ID << "," << vertexV_ID << ")" <<endl;
        }
    }
}

void Vertex::addConnection(uint32_t to,edgeType myType){

	uint32_t from = this->ID;

	if(isAdjacent(to)==false && to!=this->ID ){ //Don't add an existing vertex to list. 

		shared_ptr<Edge> e ( new Edge(make_tuple(from,to,myType) ) );
		this->adjacent.push_back(e);
	}
}



Edge_iterator Vertex::removeEdge(list<shared_ptr<Edge> >::iterator it){
	return it=adjacent.erase(it);
}

Edge_iterator Vertex::removeEdge(uint32_t toRemove){

	for(Edge_iterator it=adjacent.begin(); it!=adjacent.end(); it++){
		if( (*it)->getConnection(1) == toRemove){
			it=adjacent.erase(it);
			return --it;
		}	
	}

	return adjacent.end();	
}


//Verifies if a given vertex is adjacent to calling vertex.
//Returns ‘true’ if correct and false otherwise.
bool Vertex::isAdjacent(uint32_t _ID){

    for(std::list<shared_ptr<Edge> >::iterator it=adjacent.begin(); it != adjacent.end(); it++)
		if( (*it)->getConnection(1)== _ID ) 
			return true;
    
    return false;
}

bool Vertex::isAdjacent(shared_ptr<Vertex> u){

	uint32_t _ID = u->getID();

	for(Edge_iterator it=adjacent.begin(); it != adjacent.end(); it++)
		if( (*it)->getConnection(1)== _ID ) 
			return true;
    
    return false;
}




//   ______          _______ _                  _______ _______________________         
//  (  __  \|\     /(  ___  ( \        |\     /(  ____ (  ____ \__   __(  ____ |\     /|
//  | (  \  | )   ( | (   ) | (        | )   ( | (    \| (    )|  ) (  | (    \( \   / )
//  | |   ) | |   | | (___) | |        | |   | | (__   | (____)|  | |  | (__    \ (_) / 
//  | |   | | |   | |  ___  | |        ( (   ) |  __)  |     __)  | |  |  __)    ) _ (  
//  | |   ) | |   | | (   ) | |         \ \_/ /| (     | (\ (     | |  | (      / ( ) \ 
//  | (__/  | (___) | )   ( | (____/\    \   / | (____/| ) \ \__  | |  | (____/( /   \ )
//  (______/(_______|/     \(_______/     \_/  (_______|/   \__/  )_(  (_______|/     \|
                                                                    
DualVertex::DualVertex(){}

DualVertex::DualVertex(shared_ptr<Vertex> u,shared_ptr<Vertex> v){

	//static uint32_t newID=dualVertexStartingID;
	//this->ID = newID++;
	this->myState = dual;
	this->dualID[0] = u->getID();
	this->dualID[1] = v->getID();

}

DualVertex::DualVertex(vector<uint32_t>& __sources, shared_ptr<Vertex> verA,shared_ptr<Vertex> verB,shared_ptr<Vertex> coopVer ){

	ID=receiveUniqueID();
	myState=dual;
	
	for(vector<uint32_t>::iterator it=__sources.begin(); it!=__sources.end(); it++)
		sources.push_back((*it) );
	
	dualID[0]=verA->getID();
	dualID[1]=verB->getID();

	destinationCoop=coopVer->getID();
}



uint32_t DualVertex::getDestinationCoop(){ return this->destinationCoop; }

uint32_t DualVertex::getID(){ return this->ID;}

//Return Dual vertex ID as: "(firstVertex,secondVertex)
string DualVertex::getIdAsString(){

	return "(" + to_string(dualID[0]) + "," + to_string(dualID[1]) + ")" ;
}


DualVertex::~DualVertex(){}

