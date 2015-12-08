#include "Edge.h"



                                        //###########################
                                        //#     Constructors        #
                                        //###########################

Edge::Edge(){}

Edge::Edge(tuple<uint32_t,uint32_t,edgeType> _connection, double _distance, uint32_t _usage, uint32_t _weight){

    connection=_connection;
    distance=_distance;
    usage=1;
    weight=0;
	price=0;
}

                                        //###########################
                                        //#     Destructores        #
                                        //###########################

Edge::~Edge(){
    if(DEBUG_MODE){
        string type;
        (getEdgeType()==strongEdge) ? (type = "strong edge") : (type = "weak edge" );

        cout<< "DEBUG:" <<type<< " (" << std::get<0>(connection) << "," << get<1>(connection) << ") deleted." << endl;
    }
}

                                        //###########################
                                        //#       Accessors         #
                                        //###########################

int Edge::getPrice(){
	return price;

}




uint32_t Edge::getConnection(const int num){

    return get<1>( this->connection);
}

string Edge::getConnectionAsString(){

	//Crude and ugly implamentation for printing edges who're connected to dual vertices. 
	if( get<0>(connection) == 0 )
		return to_string( get<1>(connection) );
	else
		return "(" + to_string(get<0>(connection)) + "-" + to_string(get<1>(connection)) + ")" ;
}



edgeType Edge::getEdgeType(){
    return get<2>( this->connection );
}

string   Edge::getEdgeTypeAsString(){

	edgeType foo = get<2> (this->connection);

	switch( foo ){
	
		case weakEdge:		return "Weak" ;
		case strongEdge:	return "Strong";
		default:			return "Unknown";
	}
}



uint32_t Edge::getUsage(){ return usage; }

uint32_t Edge::getWeight(){ return weight; }

double Edge::getDistance(){ return distance; }


void Edge::setPrice(int _price){
	price=_price;
	calcWeight();
}

void Edge::setUsage(uint32_t _usage){
	usage=_usage;
	calcWeight();
}


                                        //###########################
                                        //#         Methods         #
                                        //###########################

void Edge::calcWeight(){
	this->weight = this->price;

}






bool Edge::operator==( const Edge& other ) const  {

    return get<0>(connection) == get<1>(other.connection);
}










