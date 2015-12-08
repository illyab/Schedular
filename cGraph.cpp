#define _CRT_SECURE_NO_WARNINGS

#include "cGraph.h"



										//###########################
                                        //#     Constructors        #
                                        //###########################
cGraph::cGraph(){

	/*V=5;
	
	adjMatrix.resize(V);

	for(int i=0; i<V; ++i)
	   adjMatrix[i].resize(V,0);
	
	for(int i=0; i<V; ++i)
		vertices.push_back( new cVertex() ) ;
*/

}


cGraph::cGraph(Graph* G){

	
	EdgesToVerticesConverter(*G); //generate cVertices according to number of edges in graph.

	//Resize adjMatrix to match number of created cVertices.
	adjMatrix.resize(vertices.size() );
	for(int i=0; i<(int)vertices.size(); i++)
		adjMatrix[i].resize(vertices.size(),0);

		
	findAllConflictsInGraph(*G);  //For each edge is baseGraph G, find its conflict set.
	price_cVertices(G->adjList);  // cVertex price == edge weight.

}


                                        //###########################
                                        //#     Destructores        #
                                        //###########################

cGraph::~cGraph(void)
{
}


                                       //###########################
                                        //#        Mutetores        #
                                        //###########################



                                        //###########################
                                        //#       Accessores        #
                                        //###########################



                                        //###########################
                                        //##       Methods        ###
                                        //###########################


//Convert each Edge in base graph G to a conflict vertex in conflict graph.
//Each cVertex will have a new temp ID which only applies.

void cGraph::EdgesToVerticesConverter(Graph& G){

	int num=0;	//Cvertex UID's starts from 1...
	int Dual_num=0;
	vector<uint32_t> DualIDs;

	//G.findAndSortVertices(DualIDs,dual);

	//Add all non dual related edges to conflict graph.
	for(Vertex_iterator itV=G.adjList.begin(); itV!=G.adjList.end(); ++itV){
		if((*itV)->getState()!=dual){
			for(Edge_iterator itE=(*itV)->adjacent.begin(); itE!= (*itV)->adjacent.end(); ++itE)
				vertices.push_back(new cVertex(num++, (*itV)->getID(),(*itE)->getConnection(1) ) );
		}

		//DualVertex special case
		//Add connections from dual vertex's constructing vertices to destination vertex ( coop type).
		else{	
			shared_ptr<DualVertex> dualVer=static_pointer_cast<DualVertex> ((*itV) );
			vertices.push_back( new cVertex(num++, dualVer->dualID[0], dualVer->destinationCoop) );
			vertices.push_back( new cVertex(num++, dualVer->dualID[1], dualVer->destinationCoop) );

			for(vector<uint32_t>::iterator it=dualVer->sources.begin(); it!=dualVer->sources.end(); it++){
				if(find_cVertex(*it,dualVer->getID() )== nullptr){ 
					vertices.push_back(new cVertex(--Dual_num, *it,dualVer->getID() ) ); //source to dual edge
				}	
			}

			if(find_cVertex(dualVer->getID(),dualVer->getDestinationCoop() )==nullptr ){
				vertices.push_back(new cVertex(--Dual_num, dualVer->getID(),dualVer->getDestinationCoop() ) ); //dual to coop edge
			}
		}	
	}
}

	



/* three different scenarios which illustrate two edges in a conflict.
/
/							Scenario 1	
/
/	A ---> B ---> C				Vertex B cannot function as a transmitter and receiver at the same time.
/								AB in conflict with BC.
/
/
/							Scenario 2
/	
/	C ---> A ---> B				Vertex A cannot function as transmitter and receiver at the same time.
/								AB in conflict with CA.
/
/
/							Scenario 3	
/
/	A ---> B <--- C				Vertex B cannot receive data from two vertices simultaneously.			 
/								AB in conflict with CB.
/
/
/							Scenario 4	
/
/	A <--- B <---> C <--- D		Vertex C cannot receive data from D because he's exposed to B's transmission to A.
/								DC in conflict with BA.
*/
void cGraph::findAllConflictsInGraph(Graph& G){
	vector<cVertex*>::iterator it,it1,it2;

	//Partition cVertices to related and non-related sets (related with respect to dual vertices)
	vector<cVertex*> cVerSet,dualcVerSet;
	for(it1=vertices.begin(); it1!=vertices.end(); ++it1){
		if((*it1)->getCG_ID() >= 0 )
			cVerSet.push_back(*it1 );
		else
			dualcVerSet.push_back(*it1);
	}

	//####################################################
	//Find conflicts in non related cVertex set - cVerSet.
	//####################################################
	for(it1=cVerSet.begin(); it1!=cVerSet.end(); ++it1){
		

	// ##############							Edges whos SOURCE vertex is B 
	//   Scenario 1			A ---> B ---> C		are in conflict with edge AB.
	// ##############

		for(it2=cVerSet.begin(); it2!=cVerSet.end(); ++it2){
			if( (*it1)->getDst()   == (*it2)->getSrc()  &&
				(*it1)->getCG_ID() != (*it2)->getCG_ID() )
					adjMatrix[ (*it1)->getCG_ID()][(*it2)->getCG_ID()]=true; 	
		}
		

	// ##############							Edges whos DESTINATION vertex is A 
	//   Scenario 2			C ---> A ---> B		are in conflict with edge AB.
	// ##############
		for(it2=cVerSet.begin(); it2!=cVerSet.end(); ++it2){
			if( (*it1)->getSrc()   == (*it2)->getDst()  &&
				(*it1)->getCG_ID() !=  (*it2)->getCG_ID() )
					
				adjMatrix[ (*it1)->getCG_ID()][(*it2)->getCG_ID()]=true; 
		}


	// ##############							Edges whos DESTINATION vertex is B
	//   Scenario 3			A ---> B <--- C		are in conflict with edge AB.
	// ##############
		for(it2=cVerSet.begin(); it2!=cVerSet.end(); ++it2){
			if((*it1)->getDst()    == (*it2)->getDst() && 
				(*it1)->getCG_ID() != (*it2)->getCG_ID() )
					
				adjMatrix[ (*it1)->getCG_ID()][(*it2)->getCG_ID()]=true;	
		}
			


	// ##############									Edges whos SOURCE is B are in conflict 	
	//   Scenario 3			A <--- B <----> C <--- D	with edge DC. C is exposed to B's transmission.	
	// ##############
		for(it2=cVerSet.begin(); it2!=cVerSet.end(); ++it2){
			if((*it1)->getSrc()    == (*it2)->getSrc() && 
				(*it1)->getCG_ID() != (*it2)->getCG_ID() ) //find edges like BC.
					
				for(vector<cVertex*>::iterator it3=cVerSet.begin(); it3!=cVerSet.end(); ++it3)
						if((*it2)->getDst()    == (*it3)->getDst() &&
							(*it2)->getCG_ID() != (*it3)->getCG_ID() )    //find edges like DC
								
							adjMatrix[(*it1)->getCG_ID()][(*it3)->getCG_ID()]=true;		
			}																	
	}


	//Return dual-related cVertices' negative ID to range [0,total cVertices).
	// Methode used: modulus operator
	for(vector<cVertex* >::iterator it = dualcVerSet.begin(); it!=dualcVerSet.end(); it++){
		(*it)->set_cG_ID( ((*it)->getCG_ID() + vertices.size() ) % vertices.size() );
	}
	std::sort(dualcVerSet.begin(),dualcVerSet.end()); //Sort cVertices 



	// ##############
	// Special Case: Dual Vertices				
	// ##############
/*
    		     A 				ConflictSet{(S,D)}= ConflictSet{(S,A)}  U  ConflictSet{(S,B)}
               /   \						
			 /   	 \					
			S--->D--->T			ConflictSet{(D,T)}= ConflictSet{(A,T)}  U  ConflictSet{(S,B)}
			 \    	 /					
	           \   /			S-source; D-dual; T-target; A,B - cooperative partners.
	     	     B
	
*/
	//######################################################################
	//Find conflict between dual-realted cVertex to NON dual-realted cVertex.
	//######################################################################
	for(Vertex_iterator it=G.adjList.begin(); it!=G.adjList.end(); ++it){
		if((*it)->getState() == dual){
			
			shared_ptr<DualVertex> dualVer=static_pointer_cast<DualVertex> ((*it) );	
		
			//Find cVertrex SD's conflict set. S can be comprised of up to 3 vertices.
			for(vector<uint32_t>::iterator src_it=dualVer->sources.begin(); src_it!=dualVer->sources.end(); src_it++){
				cVertex* SA = find_cVertex(*src_it,dualVer->dualID[0]);
				cVertex* SB = find_cVertex(*src_it,dualVer->dualID[1]); 
				cVertex* SD = find_cVertex(*src_it,dualVer->getID());
				cVertex* DT = find_cVertex(dualVer->getID(),dualVer->destinationCoop);
			
				for(uint32_t i=0; i<adjMatrix.size(); i++){
					if(adjMatrix[SA->getCG_ID()][i]==true || adjMatrix[SB->getCG_ID()][i]==true) 		
						adjMatrix[SD->getCG_ID()][i]=true;
				}

				//SD is in conflict with SA and SB by definition and also with DT
				adjMatrix[SD->getCG_ID()][SA->getCG_ID()] = true;
				adjMatrix[SD->getCG_ID()][SB->getCG_ID()] = true;
				adjMatrix[SD->getCG_ID()][DT->getCG_ID()] = true;
				adjMatrix[DT->getCG_ID()][SD->getCG_ID()] = true;

			}
		
			//All source to dual cVertices are in conflict.
			for(vector<uint32_t>::iterator src_it1=dualVer->sources.begin(); src_it1!=dualVer->sources.end(); src_it1++){
				for(vector<uint32_t>::iterator src_it2=dualVer->sources.begin(); src_it2!=dualVer->sources.end(); src_it2++){
					if(src_it1!=src_it2){
						cVertex* SDi = find_cVertex(*src_it1,dualVer->getID());
						cVertex* SDj = find_cVertex(*src_it1,dualVer->getID());
						
						adjMatrix[SDi->getCG_ID()][SDj->getCG_ID()];
						adjMatrix[SDj->getCG_ID()][SDi->getCG_ID()];			
					}
				}	
			}

			//Find cVertex DT's conflict set.
			cVertex* AT = find_cVertex(dualVer->dualID[0],dualVer->destinationCoop);
			cVertex* BT = find_cVertex(dualVer->dualID[1],dualVer->destinationCoop); 
			cVertex* DT = find_cVertex(dualVer->getID(),dualVer->destinationCoop);

			for(uint32_t i=0; i<adjMatrix.size(); i++){
				if(adjMatrix[AT->getCG_ID()][i]==true || adjMatrix[BT->getCG_ID()][i]==true) 
					adjMatrix[DT->getCG_ID()][i]=true;
			}

			//DT is in conflict with AT and BT by definition.
			adjMatrix[DT->getCG_ID()][AT->getCG_ID()];
			adjMatrix[DT->getCG_ID()][BT->getCG_ID()];
		}
	}

	//#############################################
	//Find conflicts between Dual-related cVertices
	//#############################################


	list<tuple<int,int,int> > tupleSet;
	list<shared_ptr<Vertex> > set;

	G.findAndSortVertices(set,dual);

	//Create tuple set. Each tuple is comprised of: < dualcVertex_uid ,cVertexA_uid, cVertexB_uid >
	for(Vertex_iterator dual_it=set.begin(); dual_it!=set.end(); dual_it++){
		shared_ptr<DualVertex> dualVer=static_pointer_cast<DualVertex> ((*dual_it) );

		for(vector<uint32_t>::iterator src_it=dualVer->sources.begin(); src_it!=dualVer->sources.end(); src_it++){
			cVertex* SA = find_cVertex(*src_it,dualVer->dualID[0]);
			cVertex* SB = find_cVertex(*src_it,dualVer->dualID[1]);						
			cVertex* SD = find_cVertex(*src_it,dualVer->getID());
		
			tupleSet.push_back(make_tuple(SD->getCG_ID(),SA->getCG_ID(),SB->getCG_ID() ) );
		}
		cVertex* DT = find_cVertex(dualVer->getID(),dualVer->destinationCoop);
		cVertex* AT = find_cVertex(dualVer->dualID[0],dualVer->destinationCoop);
		cVertex* BT = find_cVertex(dualVer->dualID[1],dualVer->destinationCoop); 

		tupleSet.push_back(make_tuple(DT->getCG_ID(),AT->getCG_ID(),BT->getCG_ID() ) );
	}

	//A conflcit between two dual cVertices will occur if at least two componenets are in conflict.
	for(list<tuple<int,int,int> >::iterator tup_it1=tupleSet.begin(); tup_it1!=tupleSet.end(); tup_it1++){
		for(list<tuple<int,int,int> >::iterator tup_it2=tupleSet.begin(); tup_it2!=tupleSet.end(); tup_it2++){
			if(get<0>(*tup_it1)!=get<0>(*tup_it2) ){
				if( adjMatrix[get<1>(*tup_it1)][get<1>(*tup_it2)]==true || 
					adjMatrix[get<1>(*tup_it1)][get<2>(*tup_it2)]==true ||
					adjMatrix[get<2>(*tup_it1)][get<1>(*tup_it2)]==true ||
					adjMatrix[get<2>(*tup_it1)][get<2>(*tup_it2)]==true ){
						
						adjMatrix[get<0>(*tup_it1)][get<0>(*tup_it2)]=true;
						adjMatrix[get<0>(*tup_it2)][get<0>(*tup_it1)]=true;
				}
			}
		}
	}
}

//Search for a cVertex using src and dst IDs
//Return cvertex's cGraphID.
cVertex* cGraph::find_cVertex(int src,int dst){

	for(vector<cVertex*>::iterator it=vertices.begin(); it!=vertices.end(); ++it)
		if( (*it)->getDst()==dst && (*it)->getSrc()==src)
			return (*it);


	return nullptr;
}


////Find a cVertex by its source ID.
//cVertex* cGraph::findBySource(int _src) {
//
//	for(vector<cVertex*>::iterator it=vertices.begin(); it!=vertices.end(); ++it)
//		if( (*it)->getSrc() == _src)
//			return *it;
//
//}
//
////Find a cVertex by its destination ID.
//cVertex* cGraph::findByDestintation(int _dst) {
//
//	for(vector<cVertex*>::iterator it=vertices.begin(); it!=vertices.end(); ++it)
//		if( (*it)->getSrc() == _dst)
//			return *it; 
//	
//	return 
//
//}
   



/*
/	while it!=vertices.end()
/	1. Generate an induced graph G' from vertex it and its adjacent vertices.
/	2. Convert G' to a DIMACS file.
/	3. Use QUALIX algorithem to calculate price of vertex.
/	4. Repeat.
*/
void cGraph::price_cVertices(list<shared_ptr<Vertex> >& baseGraphList){

	vector<int> inducedSet;

	for(vector<cVertex*>::iterator it=vertices.begin(); it!=vertices.end(); ++it){
		vector<pair<int,int> > edgesVec;
		int _E=0,_V=0;

		inducedGraphByPrimar((*it)->getCG_ID(),edgesVec,_V,_E,inducedSet);	
		(*it)->setMaxClique(findMaxCliqueInInducedGraph(edgesVec,inducedSet,_V,_E) );

		//clear parameters.
		inducedSet.clear();
		edgesVec.clear();
		_V=0;
		_E=0;

	}

}

cVertex* cGraph::edgeTo_cVertex(int src,int dst){

	for(vector<cVertex*>::iterator it=vertices.begin(); it!=vertices.end(); ++it)
		if( (*it)->getSrc()==src && (*it)->getDst()==dst)
			return (*it);


	return 0;
}




//Generate an induced graph using a primar cVertex's adjacent neighbour and himself.
//Returned values:	
//					1. E - Number of edges in graph.
//					2. V - Number of vertices in graph. 
//					3. edgesVec - A list of all directed edges in induced graph graph.
//
//	In case of dual related edges  @primarID and @secondID will have different values.
//	Otherwise, @primarID == @secondID .
//
void cGraph::inducedGraphByPrimar(int primarID,vector<pair<int,int> >& edgesVec,int& _V,int& _E,vector<int>& inducedSet){
	
	//Find All primar's cVertex adjacent neighbours.
	// And add their connection to edgesVec.
	for(int i=0; i<(int)vertices.size(); ++i){ 
		if(adjMatrix[primarID][i]==true){
			edgesVec.push_back(make_pair(primarID,i) );
			inducedSet.push_back(i);
		}
	}
	
	inducedSet.push_back(primarID);			//Add primar to induced set.
	sort(inducedSet.begin(),inducedSet.end());

	
	//Add connections to edgeVec ONLY between two members of induced set.
	for(vector<int>::iterator it=inducedSet.begin(); it!=inducedSet.end(); ++it){
		for(int i=0; i<((int) adjMatrix[*it].size() ); ++i)
			if(adjMatrix[*it][i]==true 
				&& binary_search(inducedSet.begin(), inducedSet.end(),i) 
				&& *it!=primarID )
					edgesVec.push_back(make_pair(*it,i) );
	}

	_V=inducedSet.size() ; // _V = |inducedSet| + 1 (primar vertex)
	_E=edgesVec.size();

}


//Find maximal clique in induced graph using Qualix-ms algorithm.
// Return size of maximal clique.
int cGraph::findMaxCliqueInInducedGraph(vector<pair<int,int> >& edgeVec,vector<int>& inducedSet,int V,int E){


	removeSameEdges(edgeVec);
	adjustVerticesUIDs(edgeVec,inducedSet,V);


	//Generate a DIMACS in text format.
	FILE* pFile = fopen("inducedGraph.col","w+");
	
	fprintf(pFile,"p edge %d %d",V,edgeVec.size());
	for(vector<pair<int,int> >::iterator it=edgeVec.begin(); it!=edgeVec.end(); ++it){
		fprintf(pFile,"\n");
		fprintf(pFile,"e %d %d",(*it).first,(*it).second);
	}
	fclose(pFile);

	TextToBinary("inducedGraph.col","inducedGraphBinary.b");	
	
	int res=qualix("inducedGraphBinary.b");
	
	
	return res;
}


//Remove parallel edges. AB = BA, CA=AC ve chulu....
void cGraph::removeSameEdges(vector<pair<int,int> >& edgeVec){

	vector<pair<int,int> >::iterator it,it2;
	for(it=edgeVec.begin(); it!=edgeVec.end(); ++it){	
		it2=edgeVec.begin();
		while( it2!=edgeVec.end() ){
			if( (*it).first ==(*it2).second && (*it).second == (*it2).first)
				it2=edgeVec.erase(it2);
			else
				++it2;
		}
	}
}

bool greaterFunc (int i,int j);
bool greaterFunc (int i,int j) { return (i>j); }

//Change Vertices UIDs to be in range of [1,|V|].
void cGraph::adjustVerticesUIDs(vector<pair<int,int> >& edgeVec,vector<int>& inducedSet,int V){
	
	std::sort(edgeVec.begin(),edgeVec.end() );
	std::sort(inducedSet.begin(),inducedSet.end());
	
	//Shift edges values to be in range [1,|V|].
	for(vector<pair<int,int> >::iterator it=edgeVec.begin();it!=edgeVec.end();it++){
		(*it).first+=1;
		(*it).second+=1;
	}
	

	//Shift nodes values to be in range [1,|V|].
	for(vector<int>::iterator it=inducedSet.begin(); it!=inducedSet.end(); it++)
		(*it)+=1;

	for(int i=0;i<V;i++){
		for(vector<pair<int,int> >::iterator it=edgeVec.begin();it!=edgeVec.end();it++){
			if( (*it).first==inducedSet.at(i) )
				(*it).first=i+1;

			if( (*it).second==inducedSet.at(i) )
				(*it).second=i+1;
		}
	}




}

//Convert text file 'input' to binary format 'output'.
void cGraph::TextToBinary(string input,string output ){

	string PATH_converter = "C:/Users/Magneton-Team2/Documents/Visual-Studio-2012/Projects/Scheduler/Project2/Support/converter/asc2bin.exe ";
	string command= PATH_converter + " " + input + " " + output;
	system(command.c_str() );
}


//Run Qualix-ms program and find max clique.
int cGraph::qualix(string dimcasFilename){

	
	string command;
	string PATH_QUALEX="C:/Users/Magneton-Team2/Documents/Visual-Studio-2012/Projects/Scheduler/Project2/Support/qualex-ms/qualex-ms.exe";
	string flag ="+1 +c";
	//Run Qualix-ms program.
	command = PATH_QUALEX + " " + flag + " " + dimcasFilename;
	system(command.c_str() );


	FILE* pFile;
	char line[1024];
	int MaxClq;
	char* res;

	string Sol_Filename=dimcasFilename + ".sol";

	pFile=fopen(Sol_Filename.c_str(),"r");
	while(fgets(line,1024,pFile) !=NULL){		//Search in solution file for max clique	
		if(strcmp(strtok(line," "),"s")==0 ){
			res=strtok(NULL,"\n");
			MaxClq=atoi(res);
			break;
		}
	}
	fclose(pFile);	//close file.

	return MaxClq;

}
