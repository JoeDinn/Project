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
			
		difference += abs( leftImage.img.at<uchar>(i, leftImage.img.cols - 1) - rightImage.img.at<uchar>(i, 0))
					+ abs( leftImage.img.at<uchar>(i, leftImage.img.cols - 2) - rightImage.img.at<uchar>(i, 1))
					+ abs( leftImage.img.at<uchar>(i, leftImage.img.cols - 3) - rightImage.img.at<uchar>(i, 2));

	}
	return  difference/3;
}


edge_distance::~edge_distance()
{
}
