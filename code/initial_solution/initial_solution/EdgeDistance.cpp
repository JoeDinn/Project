#include "stdafx.h"
#include "EdgeDistance.h"


EdgeDistance::EdgeDistance()
{
}

long double EdgeDistance::cost(Fragment & left_image, Fragment & right_image)
{
	assert(left_image.image.rows == right_image.image.rows);
	assert(left_image.image.cols == right_image.image.cols);


	double difference{};
	for (int i = 0; i < left_image.image.rows; i++)
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
