#include "stdafx.h"
#include "EdgeDistance.h"


EdgeDistance::EdgeDistance()
{
}

long double EdgeDistance::cost(Fragment & left_image, Fragment & right_image)
{
	assert(left_image.img.rows == right_image.img.rows);
	assert(left_image.img.cols == right_image.img.cols);


	double difference{};
	for (int i = 0; i < left_image.img.rows; i++)
	{
			
		difference += abs(left_image(i,left_image.last_pixel[i] - 1) - right_image(i,right_image.first_pixel[i]))
					+ abs( left_image(i, left_image.last_pixel[i] - 2) - right_image(i,right_image.first_pixel[i] + 1))
					+ abs( left_image(i, left_image.last_pixel[i] - 3) - right_image(i, right_image.first_pixel[i] + 2));

	}
	return  difference;
}


EdgeDistance::~EdgeDistance()
{
}
