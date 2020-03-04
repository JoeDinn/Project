#pragma once
#include "Similarity.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <vector>
class OCRCharacter :
	public Similarity
{
public:
	long double cost(Fragment & left_mage, Fragment & right_image);
	int char_score(cv::Mat &image);
	void grow_region(Fragment &image, cv::Mat &visited, int row, int col, int &left_most, int &right_most);

	OCRCharacter();
	~OCRCharacter();
};

