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
			
		difference += abs(left_image(i,left_image.last_pixel[i] - 1) - right_image(i,right_image.first_pixel[i]));

	}
	return  difference;
}


EdgeDistance::~EdgeDistance()
{
}
