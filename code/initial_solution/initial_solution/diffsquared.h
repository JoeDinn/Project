#pragma once
#include "Similarity.h"
#include <unordered_map>

class diffsquared :
	public Similarity

{
public:
	diffsquared();
	long double cost(Fragment &leftImage, Fragment &rightImage);
	~diffsquared();
};