#pragma once
#include "utilities.h"
#include <unordered_map>
class Similarity
{
protected:
	std::unordered_map<std::string, long double> LU_table;
	virtual long double cost(Fragment &left_image, Fragment &right_image) = 0;
public:
	virtual long double compare(Fragment &left_image, Fragment &right_image);
};