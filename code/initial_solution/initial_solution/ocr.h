#pragma once
#include "Similarity.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
class ocr :
	public Similarity
{
public:
	long double cost(Fragment & leftImage, Fragment & rightImage);
	int word_score(cv::Mat image);
	ocr();
	~ocr();
};

