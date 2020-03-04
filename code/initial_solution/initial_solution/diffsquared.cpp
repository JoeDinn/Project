#include "stdafx.h"
#include "DiffSquared.h"


DiffSquared::DiffSquared()
{
}

long double DiffSquared::cost(Fragment &left_image, Fragment &right_image)
{
	assert(left_image.image.rows == right_image.image.rows);
	assert(left_image.image.cols == right_image.image.cols);
	try
	{
		return LU_table.at(left_image.name + right_image.name);
	}
	catch (const std::out_of_range &e)
	{
		double difference{};
		for (int i = 0; i < left_image.image.rows; i++)
		{
			for (int j = 0; j < left_image.image.cols; j++)//change to min size from last/first
			{
				double pixel_difference = left_image(i,j) - right_image(i,right_image.image.cols);
				difference += pow(pixel_difference, 2);
			}
		}
		difference = sqrt(difference);
		LU_table.emplace(left_image.name + right_image.name, difference);
		return  difference;
		//std::cout << image.at<uchar>(i, j) << std::endl;
	}
}


DiffSquared::~DiffSquared()
{
}
