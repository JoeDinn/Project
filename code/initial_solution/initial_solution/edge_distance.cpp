#include "stdafx.h"
#include "edge_distance.h"


edge_distance::edge_distance()
{
}

long double edge_distance::cost(Fragment & leftImage, Fragment & rightImage)
{
	assert(leftImage.img.rows == rightImage.img.rows);
	assert(leftImage.img.cols == rightImage.img.cols);


	double difference{};
	for (int i = 0; i < leftImage.img.rows; i++)
	{
			
		difference += abs( leftImage.img.at<uchar>(i, leftImage.last_pixel[i] - 1) - rightImage.img.at<uchar>(i, rightImage.first_pixel[i]))
					+ abs( leftImage.img.at<uchar>(i, leftImage.last_pixel[i] - 2) - rightImage.img.at<uchar>(i, rightImage.first_pixel[i] + 1))
					+ abs( leftImage.img.at<uchar>(i, leftImage.last_pixel[i] - 3) - rightImage.img.at<uchar>(i, rightImage.first_pixel[i] + 2));

	}
	return  difference/3;
}


edge_distance::~edge_distance()
{
}
