#include "stdafx.h"
#include "Fragment.h"
#include <iostream>
#include <fstream>

Fragment::Fragment(std::string root_,std::string name_):
	name(name_), img(cv::imread(root_ + "/strips/" + name_ + ".png", cv::IMREAD_GRAYSCALE)), hist(), total_pixels()
{
	cv::Mat mask{ cv::imread(root_ + "/masks/" + name_ + ".png", cv::IMREAD_GRAYSCALE) };

	//Find first and last pixels
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
			last_pixel[i] = img.cols;
		}

	}
	row_avg  = new long double[img.rows];

	//get pixel values in each row
	for (int i{}; i < img.rows; ++i)
	{
		for (int j{ first_pixel[i] }; j < last_pixel[i]; ++j)
		{
			row_avg[i] += img.at<uchar>(i, j);
		}
		row_avg[i] = row_avg[i] / (last_pixel[i] - first_pixel[i]);
		if (name == "D00107" and (i % 20 == 0))
		{
			//std::cout << i << " " << row_avg[i] << std::endl;
		}
	}
}

void Fragment::threshold()
{
	for (int i{}; i < img.rows; ++i)
	{
		for (int j{ first_pixel[i] }; j < last_pixel[i]; ++j)
		{
			++total_pixels;
			uchar value = img.at<uchar>(i, j);
			++hist[(int)value];
		}
	}
	for (int i{}; i < 256; ++i)
	{
		//std::cout << i << " " << hist[i] << std::endl;
	}
	std::ofstream f("out.txt");

	if (f.is_open())
	{
		for (int i{}; i < 256; ++i)
		{
			f << hist[i] << std::endl;
		}
		f << "Total " << total_pixels << std::endl;

		//Get threshold using otsu

		int best_threshold{};
		int top = 256;
		int sum_lower = 0;
		unsigned long long weight_lower = 0;//Probability of lower class
		unsigned long long maximum_intra_class_var = 0;
		long double sum1{};
		for (int i = 0; i < top; i++) sum1 += (i * hist[i]);
		//f << total_pixels << std::endl;
		//f << sum1 << std::endl;

		for (int current_threshold = 0; current_threshold < top; ++current_threshold)
		{
			unsigned long long weight_upper = total_pixels - weight_lower;
			if (weight_lower > 0 and weight_upper > 0)
			{
				long double mean_upper{ (sum1 - sum_lower) / weight_upper };
				long double mean_lower{ (double)sum_lower / weight_lower };

				//f << current_threshold << " " << mean_upper << " " << mean_lower << " " << pow(mean_lower - mean_upper, 2);//

				unsigned long long intra_class_var = weight_lower * weight_upper * pow(mean_lower - mean_upper, 2);

				f << current_threshold << " " << intra_class_var << std::endl;

				if (intra_class_var >= maximum_intra_class_var)
				{
					best_threshold = current_threshold;
					maximum_intra_class_var = intra_class_var;
				}
			}
			weight_lower = weight_lower + hist[current_threshold];//Increment probability of lower class
			sum_lower = sum_lower + (current_threshold *  hist[current_threshold]);

		}
		f << "threshold " << best_threshold << std::endl;
	
	f.close();
	//threshold

	for (int i{}; i < img.rows; ++i)
	{
		for (int j{ first_pixel[i] }; j < last_pixel[i]; ++j)
		{
			img.at<uchar>(i, j) = ((int)img.at<uchar>(i, j) < best_threshold) ? 0 : 255;

		}
	}
}

}

void Fragment::grow_region(int row, int col, int & leftmost, int & topmost, int & rightmost, int & bottommost)
{
	
}


void Fragment::propogate(int row, int col, std::unordered_set<int, int>& included, int & leftmost, int & topmost, int & rightmost, int & bottommost)
{
}

Fragment::~Fragment()
{
}
