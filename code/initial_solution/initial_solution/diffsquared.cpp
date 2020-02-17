#include "stdafx.h"
#include "diffsquared.h"


diffsquared::diffsquared()
{
}

long double diffsquared::cost(Fragment &leftImage, Fragment &rightImage)
{
	assert(leftImage.img.rows == rightImage.img.rows);
	assert(leftImage.img.cols == rightImage.img.cols);
	try
	{
		return LUTable.at(leftImage.name + rightImage.name);
	}
	catch (const std::out_of_range &e)
	{
		double difference{};
		for (int i = 0; i < leftImage.img.rows; i++)
		{
			for (int j = 0; j < leftImage.img.cols; j++)
			{
				double pixel_difference = leftImage.img.at<uchar>(i, j) - rightImage.img.at<uchar>(i, rightImage.img.cols - j);
				difference += pow(pixel_difference, 2);
			}
		}
		difference = sqrt(difference);
		LUTable.emplace(leftImage.name + rightImage.name, difference);
		return  difference;
		//std::cout << img.at<uchar>(i, j) << std::endl;
	}
}


diffsquared::~diffsquared()
{
}
