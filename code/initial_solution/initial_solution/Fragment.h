#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <unordered_set>

struct Fragment
{
	std::string name;
	cv::Mat img;
	int *first_pixel;
	int *last_pixel;
	int total_pixels;
	long double *row_avg;
	int hist[256];
	Fragment(std::string root_,std::string name_);
	Fragment():name(),img() {};
	void threshold();
	void grow_region(int row, int col, int &leftmost, int & topmost, int & rightmost, int & bottommost);
	
	void propogate(int row, int col, std::unordered_set<int, int>& included, int & leftmost, int & topmost, int & rightmost, int & bottommost);
	~Fragment();

};

