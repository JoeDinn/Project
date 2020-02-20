#include "stdafx.h"
#include "Fragment.h"
#include <iostream>


Fragment::Fragment(std::string root_,std::string name_):
	name(name_), img(cv::imread(root_ + "/strips/" + name_ + ".png", cv::IMREAD_GRAYSCALE))
{
	cv::Mat mask{ cv::imread(root_ + "/masks/" + name_ + ".png", cv::IMREAD_GRAYSCALE) };
	first_pixel = new int[img.rows];
	last_pixel = new int[img.rows];

	for (int i{}; i < img.rows; ++i)
	{
		bool find_first{ true };
		bool find_last{ false };
		for (int j{}; j < img.cols; ++j)
		{
			uchar pixel_value{ mask.at<uchar>(i, j) };
			switch (pixel_value)
			{
			case 255:
				if (find_first)
				{
					first_pixel[i] = j;
					find_first = false;
					find_last = true;
				}
				break;
			case 0:
				if (find_last)
				{
					last_pixel[i] = j;
					find_last = false;
				}
				break;
			default:
				break;
			}
		}

		if (find_last)
		{
			last_pixel[i] = img.cols-1;
		}
		//std::cout << name_ << " " << i << " " << first_pixel[i] << " " << last_pixel[i] << " " << img.cols << std::endl;
	}
}

Fragment::~Fragment()
{
}
