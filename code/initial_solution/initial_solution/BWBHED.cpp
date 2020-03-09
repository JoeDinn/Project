#include "stdafx.h"
#include "BWBHED.h"


long double BWBHED::cost(Fragment & left_image, Fragment & right_image)
{
	Fragment left_BW = left_image.get_thresholded();
	Fragment right_BW = right_image.get_thresholded();
	long double cost{};
	int right_start = best_height_match(left_BW, right_BW, &cost);

	return cost;
}

BWBHED::BWBHED()
{
}


BWBHED::~BWBHED()
{
}
