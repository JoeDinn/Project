#pragma once
#include "Optimiser.h"
#include <unordered_map>


/************************************************************************
*
*  lap.h
version 1.0 - 21 june 1996
author  Roy Jonker, MagicLogic Optimization Inc.

header file for LAP
*
**************************************************************************/

/*************** CONSTANTS  *******************/

#define BIG 100000

/*************** TYPES      *******************/

typedef int row;
typedef int col;
typedef long double cost;





class hungarian :
	public Optimiser
{
public:
	std::unordered_map<std::string, double> LUTable;
	hungarian(Similarity &similarity_);
	Solution optimise(Solution fragments);
	int lap(int dim,cost **assigncost,col *rowsol,row *colsol,cost *u,cost *v);
	~hungarian();
};



/*
input :sh[]shreds
output:Ordered sequence of shreds
1distance[] 1,counter10,counter20
2whilecounter1numberof strips(sh)do
	3whilecounter2numberofstrips(sh)do
		4distance[counter1][counter2]=strip-Distance(counter1,counter2) (Totaldistances between band edges)
	5end
6end
7Hungarians-Reassembling(distance[],sh[]) returns indexes that assigning the best right match for leftside related to every shred.
8sequence[]0
9indexes = Hungarians-Reassembling(distance[])
10whilei,v in indexesdo
	11val=distance[i][v]
	12sequence.append((v,val))
13end
14return sequence[]
*/


