#include "stdafx.h"
#include "stdlib.h"
#include "utilities.h"

#include <iostream>
#include <string>
#include <fstream>
//Optimisers
#include "SimAneal.h"
#include "Hungarian.h"
#include "Greedy.h"
//Similarity measures
#include "diffsquared.h"
#include "EdgeDistance.h"
#include "OCRCharacter.h"
#include "BestHeightEdgeDistance.h"
#include "BWBHED.h"
//Evaluation metrics
#include "neighbour_comparison.h"
#include "SimilarityTester.h"

int main(int argc, char** argv)
{

	if (argc != 2)
	{
		std::cout << " Usage: " << argv[0] << " ImageToLoadAndDisplay" << std::endl;
		return -1;
	}

	std::string root{ argv[1] };
	std::ifstream file(root + "/" + root + ".txt");
	std::string str;
	std::getline(file, str);
	std::cout << str << std::endl;
	int number_files = std::stoi(str);

	GroundTruth ground_truth(number_files);
	Solution images(number_files);
	int image_counter = 0;
	while (std::getline(file, str))
	{
		ground_truth[image_counter] = str;
		images[image_counter] = Fragment(root, str); // read the file
		if (images[image_counter].image.empty()) // check for invalid input
		{
			std::cout << "could not open or find the image" << std::endl;
	}
		++image_counter;
	}

	output(images);

	//BWBHED d{};
	//SimilarityTester s_t{};
	//s_t.test(images, d);

	OCRCharacter g{};
	Fragment left(cv::imread("D:/ocr/left.png", cv::IMREAD_GRAYSCALE), "Left", images[2].first_pixel, images[2].last_pixel);
	Fragment right(cv::imread("D:/ocr/right.png", cv::IMREAD_GRAYSCALE), "right", images[3].first_pixel, images[3].last_pixel);
	g.compare(left, right);
	//images[2].threshold();
	//images[3].threshold();


	
	//Fragment combined_best{ combine(images[25],images[17],best_height_match(images[25],images[17])) };
	//cv::imwrite("d:/ocr/combined_best.png", combined_best.image);


	//std::cout << g.compare(images[15], images[6]) << std::endl;

	//cv::imshow("Thresholded", images[1].image);
	//cv::imwrite("test_threshold.png", images[1].image);
	//cv::waitKey();


	//log(images);
	shuffle(images);
	log(images);
	//output(images);

	//EdgeDistance d{};
	//diffsquared d{};
	//OCRCharacter d{};
	BestHeightEdgeDistance d{};
	Similarity &s{ d };
	
	//sim_aneal o{s};
	//hungarian o{s};
	Greedy o(s);
	

	neighbour_comparison m{};
	
	

	Solution ordered_images{ o.optimise(images) };
	std::cout << "Final " << ordered_images.size() << std::endl;
	log(ordered_images);
	std::cout << "Evaluation score: " << m.evaluate(ordered_images, ground_truth);
	output(ordered_images);
	
	return 0;
}
/**/