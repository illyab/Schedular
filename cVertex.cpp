#include "cVertex.h"



									//###########################
                                    //#     Constructors        #
                                    //###########################


cVertex::cVertex(){
	src=-1;
	dst=-1;
}

cVertex::cVertex(int _ID){
	
	CG_ID=_ID;

	maxClique=0;
	dst=0;
	src=0;
}

cVertex::cVertex(int _CG_ID,int _src,int _dst){

	CG_ID=_CG_ID;
	src=_src;
	dst=_dst;

}

									     //###########################
								         //#     Destructores        #
									     //###########################

cVertex::~cVertex(){}

								        //###########################
								        //#        Mutetores        #
								        //###########################

void cVertex::setMaxClique(int clq){this->maxClique=clq; }
void cVertex::setSrc(int _src){this->src=_src; }
void cVertex::setDst(int _dst){this->dst=_dst; }
void cVertex::set_cG_ID(int newID){this->CG_ID = newID; }




                                        //###########################
                                        //#       Accessores        #
                                        //###########################


int cVertex::getCG_ID(){return CG_ID; }
int cVertex::getSrc(){return src; }
int cVertex::getDst(){return dst; }
int cVertex::getMaxClique(){return maxClique; }



                                        //###########################
                                        //##       Methods        ###
                                        //###########################




bool cVertex::operator<(const cVertex& other) const{

	if( this->src != other.src) return ( this->src < other.src);

	return this->dst < other.dst;
}

bool cVertex::operator==(const cVertex& other) const{

	return (this->src==other.src && this->dst==other.dst );

}