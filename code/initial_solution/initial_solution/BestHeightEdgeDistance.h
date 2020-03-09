#pragma once
#include "Similarity.h"
#include "EdgeDistance.h"
class BestHeightEdgeDistance :
	public Similarity
{
	EdgeDistance edge_distance;
public:
	long double cost(Fragment &left_image, Fragment &right_image);
	BestHeightEdgeDistance();
	~BestHeightEdgeDistance();
};

