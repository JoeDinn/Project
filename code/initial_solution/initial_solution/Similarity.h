#pragma once
#include "utilities.h"
#include <unordered_map>
class Similarity
{
protected:
	std::unordered_map<std::string, long double> LUTable;
	virtual long double cost(Fragment &leftImage, Fragment &rightImage) = 0;
public:
	virtual long double compare(Fragment &leftImage, Fragment &rightImage);
};