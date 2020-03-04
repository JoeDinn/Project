#include "stdafx.h"
#include "DiffSquared.h"


DiffSquared::DiffSquared()
{
}

long double DiffSquared::cost(Fragment &left_image, Fragment &right_image)
{
	assert(left_image.img.rows == right_image.img.rows);
	assert(left_image.img.cols == right_image.img.cols);
	try
	{
		return LU_table.at(left_image.name + right_image.name);
	}
	catch (const std::out_of_range &e)
	{
		double difference{};
		for (int i = 0; i < left_image.img.rows; i++)
		{
			for (int j = 0; j < left_image.img.cols; j++)//change to min size from last/first
			{
				double pixel_difference = left_image(i,j) - right_image(i,right_image.img.cols);
				difference += pow(pixel_difference, 2);
			}
		}
		difference = sqrt(difference);
		LU_table.emplace(left_image.name + right_image.name, difference);
		return  difference;
		//std::cout << img.at<uchar>(i, j) << std::endl;
	}
}


DiffSquared::~DiffSquared()
{
}
