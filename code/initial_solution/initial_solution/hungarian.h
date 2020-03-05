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
typedef long double cost_value;
typedef int col;






class Hungarian :
	public Optimiser
{
public:
	std::unordered_map<std::string, double> LU_table;
	Hungarian(Similarity &similarity_);
	Solution optimise(Solution fragments);
	int lap(int dim,cost_value **assigncost_value,col *rowsol,row *colsol,cost_value *u,cost_value *v);
	~Hungarian();
};



