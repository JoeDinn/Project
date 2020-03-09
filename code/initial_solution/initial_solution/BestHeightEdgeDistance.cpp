#include "stdafx.h"
#include "BestHeightEdgeDistance.h"


long double BestHeightEdgeDistance::cost(Fragment & left_image, Fragment & right_image)
{
	long double cost{};
	int right_start = best_height_match(left_image, right_image, &cost);
	
	return cost;
}

BestHeightEdgeDistance::BestHeightEdgeDistance() :
	edge_distance()
{
}


BestHeightEdgeDistance::~BestHeightEdgeDistance()
{
}
