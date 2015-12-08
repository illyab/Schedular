#include "Graph.h"



                                        //###########################
                                        //#     Constructors        #
                                        //###########################
//Empty constructor
Graph::Graph(){}

//Construct a base graph.
Graph::Graph(string PATH,string _name){

    uint32_t _ID;
    float x,y;
    

    this->name = _name;
    ifstream myFile(PATH);
	while(!myFile.eof()){
		myFile >> _ID >> x >> y ;
        addVertex(make_tuple(x,y),_ID,solo);
    }
	
	identifyVertices();
	cleanGraph();
	createDualVertices();
	//finalClean();
}

                                        //###########################
                                        //#     Destructores        #
                                        //###########################

Graph::~Graph(){

    if(DEBUG_MODE)
        cout<<"DEBUG: " << name << " deleted" << endl;
}

                                        //###########################
                                        //#        Mutetores        #
                                        //###########################

void Graph::setName(string newName){    this->name = newName;    }

                                        //###########################
                                        //#       Accessores        #
                                        //###########################



                                        //###########################
                                        //##       Methods        ###
                                        //###########################

void Graph::printEdgeList(){

	string edge,weight;
	cout<<"################"<<endl;

	for(Vertex_iterator it=adjList.begin(); it!=adjList.end(); ++it){
		for(Edge_iterator edge_it = (*it)->adjacent.begin(); edge_it!=(*it)->adjacent.end(); edge_it++){
			
			edge="(" + (*it)->getIdAsString() + "," + findVertex((*edge_it)->getConnection(1) )->getIdAsString() + ")";
			if (edge.length()==5)
				edge.append("      ");
			else
				edge.append("  ");

			weight = std::to_string((*edge_it)->getWeight() );

			cout<< edge << weight <<endl<<endl;
		}
	}

	cout<<"################"<<endl;

}




//Iterate through graph's edges and assign for each one a weight.
//Edge's is generated using a conflict graph. Higher instancesa of conflict with other edges will resault with bigger weight.
void Graph::assignWeights(vector<cVertex* >& cVertexSet){

	finalTouch();

	//Note to self: running time is currently O(|E|^2). This could and should be optimized to O(|E|log|E|).
	// Binary search will be added later, perhaps using a template.


	for(Vertex_iterator verIt=adjList.begin(); verIt!=adjList.end(); ++verIt){
		for(Edge_iterator edgeIt=(*verIt)->adjacent.begin(); edgeIt!=(*verIt)->adjacent.end(); ++edgeIt)
			for(vector<cVertex* >::iterator cVer_it=cVertexSet.begin(); cVer_it!=cVertexSet.end(); cVer_it++){
				int src = (*verIt)->getID();
				int dst = (*edgeIt)->getConnection(1);
			
				if( (*cVer_it)->getSrc()==src && (*cVer_it)->getDst()==dst)
					(*edgeIt)->setPrice( (*cVer_it)->getMaxClique() );	}	}	}			
				
			
	



//1. Remove All remaining weak edges in graph.
//2. Reconnect dual related edges in graph.
//3. Remove all remaining solo vertices in graph.
void Graph::finalTouch(){

	//1.
	for(Vertex_iterator itVer=adjList.begin(); itVer!=adjList.end(); itVer++)
		for(Edge_iterator itEdge= (*itVer)->adjacent.begin(); itEdge!=(*itVer)->adjacent.end(); itEdge++)
			if( (*itEdge)->getEdgeType() == weakEdge)
				(*itVer)->removeEdge(itEdge);
		
		
	//2. and 3.	
	for(Vertex_iterator it1=adjList.begin(); it1!=adjList.end(); it1++)
		if((*it1)->getState()==dual){
			shared_ptr<DualVertex> dualVer=static_pointer_cast<DualVertex> ((*it1) );
		
			//Add edge source to dual
			for(vector<uint32_t>::iterator it2= dualVer->sources.begin(); it2!=dualVer->sources.end(); it2++){
				shared_ptr<Vertex> src_vertex= findVertex(*it2);
				src_vertex->addConnection(dualVer->getID(),strongEdge);	}
			
		
			//Add edge dual to coop.
			dualVer->addConnection(dualVer->getDestinationCoop(),strongEdge); 
		}
		else if((*it1)->getState() ==solo){
			it1 = removeVertex(it1);
		}
}










//Return number of Edges in graph.
int Graph::getNumOfEdges(){

	int E=0;
	for(Vertex_iterator it=adjList.begin(); it!=adjList.end() ; ++it)
		E+= (*it)->adjacent.size();

	return E;	}



shared_ptr<Vertex> Graph::findVertex(uint32_t _ID){

    for(list<shared_ptr<Vertex> >::iterator it=adjList.begin();it!=adjList.end(); it++)
        if( (*it)->getID() == _ID )
            return (*it);
  
	return nullptr;	}




bool Graph::addVertex(tuple<float,float> _position,uint32_t _ID,State _myState){

	if( findVertex(_ID) == nullptr){

        //Initialize 'u' Vertex and add to graph.
        shared_ptr<Vertex> u (new Vertex(_ID,_position,_myState));

        findConnections(u);    //Find connected vertices.
        adjList.push_back(u);

        return true;	}
    

    else if(DEBUG_MODE)
        cout << "DEBUG: Vertex " << _ID << " already exist in graph" <<endl;

    return false;	}


bool Graph::addVertex(shared_ptr<Vertex> u){

	adjList.push_back(u);
	return true;
}

//Remove vertex and all its exiting edges.
//return Iterator to next object in list.
Vertex_iterator Graph::removeVertex(Vertex_iterator it){

	if(it != adjList.end() ){
		it=adjList.erase( it);
	}
	return --it;		//Voodoo code.
}



//iterate through all existing vertcies and graph and calculate if a posiible weak or strong connection exist.
void Graph::findConnections(shared_ptr<Vertex> u){

    float  X2,Y2;
    double dis;
    float  X1 = get<0>(u->getPosition());
    float  Y1 = get<1>(u->getPosition());

    for(list<shared_ptr<Vertex> >::iterator it=adjList.begin();it!=adjList.end(); it++ )
        if( (*it)->getID() != u->getID() ){     //Avoid self loops.

            X2 = get<0>( (*it)->getPosition() );
            Y2 = get<1>( (*it)->getPosition() );
            dis = sqrt( pow(X1-X2,2) + pow(Y1-Y2,2) );  //Connectivity is a function of linear distance only!

            if( dis <= cooperativeTreshold  ){     //Check if any type of connection can exist.

                edgeType myType = (dis < connectivityTreshold) ? (strongEdge) : (weakEdge);

                shared_ptr<Edge> u2v ( new Edge( make_tuple(0,(*it)->getID(),myType ), dis) );
                shared_ptr<Edge> v2u ( new Edge( make_tuple(0,u->getID(),myType ), dis) );

                u->addConnection(u2v);
                (*it)->addConnection(v2u);	}	}	}  // if edge (u,v) exist , so is edge (v,u).
            
        
    



void Graph::printGraph(){

	string weak, strong;

    cout << "Printing adjacency list of graph G " <<endl;
    cout << "##################################" <<endl<<endl;

	list<shared_ptr<Vertex> >::iterator vertex_it;

    for(vertex_it=adjList.begin();vertex_it!=adjList.end(); vertex_it++ ){
		cout << (*vertex_it)->getIdAsString() << "(" << (*vertex_it)->getStateAsString()<< "): {";

		shared_ptr<Vertex> u =(*vertex_it);
		for(Edge_iterator edge_it = u->adjacent.begin(); edge_it!=u->adjacent.end(); edge_it++){
			
			uint32_t uid = (*edge_it)->getConnection(1);
			shared_ptr<Vertex> v = findVertex(uid);
			cout<< "(" << v->getIdAsString() << "),";
		
		}
		cout <<"}"<<endl;
	}

	cout << "##################################" <<endl<<endl;
}


/* Iterate through graph G's vertices and identify each one’s state.
 CH - Unique in graph ( only one exist ).
 Relay - Has a strong connection with Cluster-head type.
 Slave - Has no strong connection with cluster-head type and 
         Has at least one strong connection.
 Coop - Has two or more weak connections.
 Solo - Default state for each vertex.

*/
void Graph::identifyVertices(){

	Vertex_iterator vertex_it;
	Edge_iterator edge_it;

	uint32_t CH_ID = adjList.front()->getID(); //Access Cluster-Head ID number
	adjList.front()->setState(clusterHead); //Set first vertex state to CH.

	//Identify Relay type vertices.
	//Decision rule: Connected to CH type vertex.
	for(edge_it=adjList.front()->adjacent.begin(); edge_it!=adjList.front()->adjacent.end(); edge_it++){ 
		if((*edge_it)->getEdgeType()==strongEdge){
			uint32_t _ID = (*edge_it)->getConnection(1);
			findVertex(_ID)->setState(relay);
		}
	}

	//Identify Slave type vertices using dedicated function. 
	for(vertex_it=adjList.begin() ;vertex_it!=adjList.end() ;vertex_it++){
		if(isSlave(*vertex_it)==true)
			(*vertex_it)->setState(slave);
	}

	//Identify Coop type vertices using dedicated function. 
	for(vertex_it=adjList.begin() ;vertex_it!=adjList.end() ;vertex_it++){
		if(isCoop(*vertex_it)==true )
			(*vertex_it)->setState(coop); 
	}
}

//Check if a given vertex is a candidate to become a Slave type vertex.
//Decision rule: Has at least one strong edge. 
bool Graph::isSlave(shared_ptr<Vertex> ver){

	bool isSlave=false;

	if(ver->getState()==solo){
		
		pair<Edge_iterator,Edge_iterator> edge_it = ver->getAdjacent(); //For each vertex get iterators to its edge list.

		for(; edge_it.first != edge_it.second ; edge_it.first++)
			if((*edge_it.first)->getEdgeType()==strongEdge ) 
				isSlave=true;
	}

	return isSlave;
}

//Check if a given vertex is a candidate to become a Coop type vertex.
//Decision rule: Has two or more weak edges. 
bool Graph::isCoop(shared_ptr<Vertex> ver){

	bool isCoop=false;

	if(ver->getState()==solo){
		uint32_t edge_counter=0;
	
		pair<Edge_iterator,Edge_iterator> edge_it = ver->getAdjacent(); //For each vertex get iterators to its edge list.		
		for(; edge_it.first != edge_it.second ; edge_it.first++)
			if( (*edge_it.first)->getEdgeType() == weakEdge )
				edge_counter++;
	
		if( edge_counter >=2 )
			isCoop=true;
	}

	return isCoop;
}


//1. Remove all weak edges in graph except those which belong to a Coop type vertex.
//2. Remove remaining solo type vertices from graph
void Graph::cleanGraph(){

	State myState;
	Edge_iterator it,end;
	
	for(Vertex_iterator vertex_it=adjList.begin() ;vertex_it!=adjList.end() ;vertex_it++){
		if((*vertex_it)->getState() == solo)
			vertex_it=removeVertex(vertex_it);
		
		else{
			myState = (*vertex_it)->getState();
			it=(*vertex_it)->adjacent.begin();
			end=(*vertex_it)->adjacent.end();

			while(it!=end){
				if( (*it)->getEdgeType()==weakEdge && myState!=coop)
					it=(*vertex_it)->adjacent.erase(it);
				else
					++it;
			}
		}
	}
}




//Remove all remaining weak edges and delete any Solo type vertices.
//Remove any connection between two Dual vertices
void Graph::finalClean(){

	State myState;
	Edge_iterator it,end;
	Vertex_iterator vertex_it=adjList.begin();

	//Remove solo vertices
	while(vertex_it!=adjList.end()){
	
		myState= (*vertex_it)->getState();

		switch (myState){

			case solo:
				vertex_it=adjList.erase( vertex_it);
				break;

			case coop:
				it=(*vertex_it)->adjacent.begin();
				end=(*vertex_it)->adjacent.end();

				while(it!=end){
					if( (*it)->getEdgeType()==weakEdge)
						it=(*vertex_it)->adjacent.erase(it);
					else
						++it;
				}
				++vertex_it;
				break;

			default:
				++vertex_it;

		}

	}

	//for(Vertex_iterator vertex_it=adjList.begin() ;vertex_it!=adjList.end() ;vertex_it++){
	//	if( (*vertex_it)->getState()==solo )	//remove solo vertices.
	//		vertex_it=removeVertex(vertex_it);
	//
	//	else if( (*vertex_it)->getState()==coop)
	//		for(pair<Edge_iterator,Edge_iterator> edge_it = (*vertex_it)->getAdjacent(); edge_it.first!=edge_it.second ; edge_it.first++)
	//			if( (*edge_it.first)->getEdgeType()==weakEdge )	//Remove weak edges.
	//				edge_it.first=(*vertex_it)->removeEdge( edge_it.first );
	//	
	//}	
}


//Find all vertices who are same state as @param searchedState and 
//sort vertices according to their ID.
void Graph::findAndSortVertices(list<shared_ptr<Vertex> >& set,State searchedState){

	for(Vertex_iterator vertex_it=adjList.begin() ;vertex_it!=adjList.end() ;vertex_it++){
		if( (*vertex_it)->getState()==searchedState)
			set.push_back( (*vertex_it) );
	}
	//sort(set.begin(),set.end() );
}


/*For each coop vertex in graph try to generate a Dual vertex if possible. If for a given Coop vertcies no Dual vertex were created, 
//Changed its status to Solo for future removal from graph.
//
//Condition rule for generating Daul vertex to a given Coop vertex: It must have at least two neigboures who both have a strong edge with a common neigbour (excluding himself).
//
//	 Good Connection             Bad connection						     index
														
		 R					   		R							/,\,|    weak edge
       // \					      // \							//,\\,=  strong edge
	  //   \				     //	  \							R        Relay
	 CH     Coop			    CH	  Coop						S		 Slave
	  \\   /				    \\     |						CH		 ClusterHead
	   \\ /			             \\    |
	     R					       R===S
*/ 
void Graph::createDualVertices(){

	Vertex_iterator graph_itr,Na_itr,Nb_itr,Nc_itr1,Nc_itr2;
	

	//Iterate though graph's vertex set V.
	for(graph_itr=adjList.begin(); graph_itr!=adjList.end(); graph_itr++){
		if((*graph_itr)->getState()==coop){
			
			list<shared_ptr<Vertex> > Nc;
			bool dualVertexCreated=false;
			int counter=0;

			findAdjacentVertices(Nc,(*graph_itr),weakEdge ); //Create Vertex set N(c) - adjacent vertex set of Cooperive vertex c.
			
			for(Nc_itr1=Nc.begin();  Nc_itr1!=Nc.end();Nc_itr1++){   //iterate through N(c) and try to find two vertices who have a common source. 
	
				Nc_itr2=Nc_itr1; //avoid Nc_itr1 and Nc_itr2 to point to same vertex.
				
				for(Nc_itr2++;Nc_itr2!=Nc.end();Nc_itr2++){

					list<shared_ptr<Vertex> > Na,Nb; 
					vector<uint32_t> sources;
					
					//Create vertex sets N(a) and N(b). a,b are in N(c).
					findAdjacentVertices(Na,(*Nc_itr1),strongEdge ); 
					findAdjacentVertices(Nb,(*Nc_itr2),strongEdge );
					

					//Iterate through N(a) and N(b) and search for all mutual (which are not Coop type vertices) members.
					//Stop iteration if more then three members were found.
					for(Na_itr=Na.begin();( Na_itr!=Na.end() && counter<3 ) ;Na_itr++){  
						for(Nb_itr=Nb.begin();( Nb_itr!=Nb.end() && counter<3 ) ;Nb_itr++){
							if((*Na_itr)->getID()==(*Nb_itr)->getID() ){
								dualVertexCreated=true;
								sources.push_back((*Nb_itr)->getID() );
								counter++;
							}	
						} 
					}
					//Create Dual vertex only if a least one source was found.
					if(sources.empty()==false){ 
						shared_ptr<Vertex> newDual(new DualVertex(sources,(*Nc_itr1),(*Nc_itr2),(*graph_itr) ) );
						adjList.push_back(newDual);
					}

					Na.clear();
					Nb.clear();
					sources.clear();
					counter=0;
				}
			}

			
			if(dualVertexCreated==false)				
				(*graph_itr)->setState(solo);

			removeWeakEdges(*graph_itr);
		}
	}
}


void Graph::removeWeakEdges(shared_ptr<Vertex> u){

	Edge_iterator it = u->adjacent.begin();

	while( it!=u->adjacent.end()){
	
		if((*it)->getEdgeType()==weakEdge){
			it=u->removeEdge(it);
		}
		else{
			it++;
		}
	}

}

//Return vertex set of all stronlgy connected adjacent vertices.
void Graph::findAdjacentVertices(list<shared_ptr<Vertex> >& set,shared_ptr<Vertex> ver,edgeType lookedType){
	
	for(pair<Edge_iterator,Edge_iterator> p = ver->getAdjacent(); p.first!=p.second;p.first++)
		if((*p.first)->getEdgeType()==lookedType)
			set.push_back(findVertex((*p.first)->getConnection(1) ) );
}

/*
// Creates a Dual vertex from verA and verB according to following steps:
//	1. Create an empty Dual vertex.
//	2. Add all VerA and VerB's connections to dual vertex and vise versa.	 21/10/15	update: code is removed.
//	3. Add a strong directed edge from commonNeighbour to Dual vertex.		 21/10/15	update: code is removed.
//	4. Add a directed strong edge from Dual vertex to coopVer.				 21/10/15	update: code is removed.
//	5. Add Dual vertex to graph's vertex set.

//	   Before             After						     Index
														
		 R		       		R							/,\,|    weak edge
       // \               // 							//,\\,=  strong edge
	  //   \			 //	  							R        Relay
	 CH     Coop		CH===>Dual===>Coop				S		 Slave
	  \\   /			 \\     						CH		 ClusterHead
	   \\ /               \\    
	     R				    R
*/
void Graph::createDualVertex(shared_ptr<Vertex> commonNeighbour, shared_ptr<Vertex> verA,shared_ptr<Vertex> verB,shared_ptr<Vertex> coopVer ){

	list<shared_ptr<Vertex> > verSet;

	//Step 1.
	shared_ptr<Vertex> dual (new DualVertex(verA,verB) );			

	//Update:	22/10/15 
	
	verA->addConnection(coopVer,weakEdge);
	verB->addConnection(coopVer,weakEdge);

	//Step5.
	this->adjList.push_back(dual);		



	//Step 2.		21/10/15 most Dual connection are redundant and not real. step 2 removed

	//for(Edge_iterator it=verA->adjacent.begin();it!=verA->adjacent.end();it++ ){   //Add verA connections to dual vertex
	//	uint32_t neighbourID= (*it)->getConnection(1);	
	//	
	//	if(neighbourID >=dualVertexStartingID){   //Avoid redundant connection with other dual vertcies
	//		dual->addConnection( neighbourID,strongEdge );
	//		findVertex(neighbourID)->addConnection(dual->getID(),strongEdge);	
	//	}
	//}

	//for(Edge_iterator it=verB->adjacent.begin();it!=verB->adjacent.end();it++ ){	//Add verB connections to dual vertex
	//	uint32_t neighbourID= (*it)->getConnection(1);	
	//	
	//	if(neighbourID >=dualVertexStartingID){   //Avoid redundant connection with other dual vertcies
	//		dual->addConnection( neighbourID,strongEdge );
	//		findVertex(neighbourID)->addConnection(dual->getID(),strongEdge);	
	//	}
	//}
	//

	//Step 3.
	//commonNeighbour->addConnection(dual->getID(),strongEdge );


	//Step 4.
	//dual->addConnection(coopVer,strongEdge);
							
}











