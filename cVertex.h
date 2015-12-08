#ifndef cVertex_H
#define cVertex_H

#include "general.h"

class cVertex
{
	private:
		int CG_ID;
		int	maxClique;
		int	src;
		int	dst;

	public:
		//Constructors
		cVertex();
		cVertex(int _ID);
		cVertex(int CG_ID,int src,int dst);

		//Destructor
		~cVertex();

		//Mutetors
		void setMaxClique(int clq);
		void setSrc(int _src);
		void setDst(int _dst);
		void set_cG_ID(int newID);

		//Accesors
		int getMaxClique();
		int getSrc();
		int getDst();
		int getCG_ID();

		//Methods
		bool operator<(const cVertex& ver) const;
		bool operator==(const cVertex& ver) const;

};

#endif