#include "stdafx.h"
#include "Similarity.h"

long double Similarity::compare(Fragment & leftImage, Fragment & rightImage)
{
	try
	{
		return LUTable.at(leftImage.name + rightImage.name);
	}
	catch (const std::out_of_range &e)
	{
		long double distance_cost{cost(leftImage,rightImage)};
		LUTable.emplace(leftImage.name + rightImage.name, distance_cost);
		return  distance_cost;
	}
}
