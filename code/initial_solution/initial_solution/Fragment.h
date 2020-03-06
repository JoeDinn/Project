#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <unordered_set>

struct Fragment
{
	
	std::string name;
	cv::Mat image;
	int *first_pixel;
	int *last_pixel;
	Fragment(std::string root_,std::string name_);
	Fragment(const Fragment &fragment);
	Fragment(cv::Mat image_, std::string name_, int *first_pixel_, int *last_pixel_);
	Fragment();
	Fragment get_thresholded();
	void threshold();
	void colour_histogram(int hist[256]);
	void get_text_lines(bool *is_text_line);
	cv::Mat dilate(int window_size, int threshold);
	uchar &operator()(int row,int col);
	~Fragment();

	


};

