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





class Hungarian :
	public Optimiser
{
public:
	std::unordered_map<std::string, double> LU_table;
	Hungarian(Similarity &similarity_);
	Solution optimise(Solution fragments);
	int lap(int dim,cost **assigncost,col *rowsol,row *colsol,cost *u,cost *v);
	~Hungarian();
};



