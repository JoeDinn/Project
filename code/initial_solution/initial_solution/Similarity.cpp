#include "stdafx.h"
#include "Similarity.h"

long double Similarity::compare(Fragment & left_image, Fragment & right_image)
{
	try
	{
		return LU_table.at(left_image.name + right_image.name);
	}
	catch (const std::out_of_range &e)
	{
		long double distance_cost{cost(left_image,right_image)};
		LU_table.emplace(left_image.name + right_image.name, distance_cost);
		return  distance_cost;
	}
}
