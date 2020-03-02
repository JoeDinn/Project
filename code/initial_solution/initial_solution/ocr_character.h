#pragma once
#include "Similarity.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <vector>
class ocr_character :
	public Similarity
{
public:
	long double cost(Fragment & leftImage, Fragment & rightImage);
	int char_score(cv::Mat image);
	void grow_region(Fragment &image, cv::Mat &mask, int row, int col, int &top_most, int &bottom_most, int	&left_most, int &right_most);

	ocr_character();
	~ocr_character();
};

