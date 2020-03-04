#pragma once
#include "Similarity.h"
class EdgeDistance :
	public Similarity
{
public:
	EdgeDistance();
	long double cost(Fragment &left_image, Fragment &right_image);
	~EdgeDistance();
};

