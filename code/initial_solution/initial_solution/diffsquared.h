#pragma once
#include "Similarity.h"
#include <unordered_map>

class DiffSquared :
	public Similarity

{
public:
	DiffSquared();
	long double cost(Fragment &left_image, Fragment &right_image);
	~DiffSquared();
};