#ifndef GENERAL_H
#define GENERAL_H


#include <cstdlib>
#include <tuple>
#include <list>
#include <iostream>
#include <fstream>
#include <math.h>
#include <unordered_map>
#include <memory>
#include <string>
#include <algorithm>

#define DEBUG_MODE 1

using namespace std;

typedef enum {clusterHead=0,relay,slave,coop,solo,dual} State;
typedef enum {strongEdge=0,weakEdge} edgeType;


uint32_t const connectivityTreshold=100;
uint32_t const cooperativeTreshold=1000;

uint32_t const dualVertexStartingID=1000;



#endif // GENERAL_H_INCLUDED
