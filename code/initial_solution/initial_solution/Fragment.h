#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
struct Fragment
{
	std::string name;
	cv::Mat img;
	int *first_pixel;
	int *last_pixel;
	Fragment(std::string root_,std::string name_);
	Fragment():name(),img() {};
	~Fragment();
};

