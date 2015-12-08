#ifndef EDGE_H
#define EDGE_H

#include "general.h"


class Edge
{
    public:
        //Constructors:
        Edge();
        Edge(tuple<uint32_t,uint32_t,edgeType> _connection, double _distance=-1, uint32_t _usage=1, uint32_t _weight=0);

        //Destructors:
        ~Edge();

        //Accessors
        uint32_t getConnection(const int num);
		string   getConnectionAsString();
        uint32_t getUsage();
        uint32_t getWeight();
        double   getDistance();
        edgeType getEdgeType();
		string   getEdgeTypeAsString();
		int		 getPrice();	

        //Mutetores
		void setPrice(int _price);
		void setUsage(uint32_t _usage);

        //Methods:
        bool operator==( const Edge& other ) const;
		void calcWeight();

    private:
        tuple<uint32_t,uint32_t,edgeType> connection;
        uint32_t                          usage;
        uint32_t                          weight;
		int								  price;	
        double                            distance;
};

#endif // EDGE_H



