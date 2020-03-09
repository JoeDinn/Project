#pragma once
#include "Similarity.h"
class BWBHED :
	public Similarity
{
public:
	long double cost(Fragment &left_image, Fragment &right_image);
	BWBHED();
	~BWBHED();
};

