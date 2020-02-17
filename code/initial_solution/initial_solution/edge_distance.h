#pragma once
#include "Similarity.h"
class edge_distance :
	public Similarity
{
public:
	edge_distance();
	long double cost(Fragment &leftImage, Fragment &rightImage);
	~edge_distance();
};

