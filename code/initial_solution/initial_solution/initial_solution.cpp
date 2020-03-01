// initial_solution.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "utilities.h"
#include <iostream>
#include <fstream>
#include <string>

#include "sim_aneal.h"
#include "hungarian.h"
#include "greedy.h"
#include "diffsquared.h"
#include "edge_distance.h"
#include "ocr.h"

#include "stdlib.h"

int main(int argc, char** argv)
{

	if (argc != 2)
	{
		std::cout << " Usage: " << argv[0] << " ImageToLoadAndDisplay" << std::endl;
		return -1;
	}

	std::string root{ argv[1] };
	root = root;
	std::ifstream file (root+"/" + root + ".txt");
	std::string str;
	std::getline(file, str);
	std::cout << argv[0] << std::endl;
	std::cout << argv[1] << std::endl;
	std::cout << str << std::endl;


	int number_files = std::stoi(str);
	Solution images(number_files);
	int image_counter = 0;
	while (std::getline(file, str))
	{
		images[image_counter] = Fragment(root,str); // read the file
		if (images[image_counter].img.empty()) // check for invalid input
		{
			std::cout << "could not open or find the image" << std::endl;
			return -1;
		}

		++image_counter;
	}

	images[1].histogram();
	//cv::Mat bin;
	//cv::adaptiveThreshold(images[1].img, bin,255,cv::ADAPTIVE_THRESH_MEAN_C,cv::THRESH_BINARY,3,0);
	//bin, th2 = cv.threshold(img, 0, 255, cv.THRESH_BINARY + cv.THRESH_OTSU)
	//cv::namedWindow("Thresholded",1);
	cv::imshow("Thresholded", images[1].img);
	cv::imwrite("test_threshold.png", images[1].img);
	cv::waitKey();


	//log(images);
	shuffle(images);
	log(images);
	//output(images);

	edge_distance d{};
	//diffsquared d{};

	Similarity &s{ d };
	
	//sim_aneal o{s};
	//hungarian o{s};
	greedy o(s);
	

	Solution ordered_images{ o.optimise(images) };
	std::cout << "Final " << ordered_images.size() << std::endl;
	log(ordered_images);
	std::cout << "ss";
	output(ordered_images);
	std::cout << "dd";
	return 0;
}
/**/