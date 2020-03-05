#pragma once
#include "Similarity.h"
#include "Hungarian.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <vector>


struct PotentialChar
{
	bool is_dummy;
	int bottom;
	int top;
	int right;
	int left;
	PotentialChar(int bottom_, int top_, int right_, int left_, bool is_dummy_ = false);
	PotentialChar(bool is_dummy_ = true);

};


class OCRCharacter :
	public Similarity
{
public:
	long double cost(Fragment & left_image, Fragment & right_image);
	int char_score(cv::Mat &image);
	void grow_region(Fragment &image, cv::Mat &visited, int row, int col, int &left_most, int &right_most);
	std::vector<PotentialChar> get_potential_characters(Fragment &fragment,int *edge);
	std::vector<cv::Mat> combine(std::vector<PotentialChar> &left_characters, std::vector<PotentialChar> &right_characters, Fragment &left_image, Fragment &right_image);
	void pad(std::vector<PotentialChar> &characters, int amount);
	OCRCharacter();
	~OCRCharacter();
};
