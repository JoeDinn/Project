#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
struct Fragment
{
	std::string name;
	cv::Mat img;
	Fragment(std::string name_, cv::Mat img_):name(name_),img(img_) {};
	Fragment():name(),img() {};
	~Fragment();
};

