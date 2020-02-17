#include "stdafx.h"
#include "utilities.h"

void output(Solution &solution)
{
	std::cout << std::endl << "Size" << solution.size() << std::endl;
	std::vector<cv::Mat> images(solution.size());
	for (int i{ 0 }; i < solution.size(); ++i)
	{
		images[i] = solution[i].img;
	}


	cv::Mat combined;
	cv::hconcat(images, combined);
	cv::namedWindow("Display window", cv::WINDOW_NORMAL); // Create a window for display.
	cv::imshow("Display window", combined); // Show our image inside it.
	cv::waitKey(0); // Wait for a keystroke in the window
}

void log(Solution &solution)
{
	std::cout <<  solution.size() << std::endl;
	for(Fragment fragment:solution)
		{
			std::cout << "> " << fragment.name << std::endl;
		}
}

void shuffle(Solution & solution)
{
	for (int i = 0; i < solution.size()-1; ++i)
	{
		int j{ random::rand_int(i,solution.size()-1)};
		std::swap(solution[i], solution[j]);
		std::cout << i << " <--> " << j << std::endl;
	}
	std::cout << "done\n";
}

int leftMost(Solution & solution)
{
	int left_most_index{};
	long long  best_whiteness{};
	for (int index{}; index < solution.size(); ++index)
	{
		long long whiteness{};
		for (int i{}; i < solution[index].img.rows; ++i)
		{
			for (int j{}; j < solution[index].img.cols; ++j)
			{
				whiteness += (int)solution[index].img.at<uchar>(i, j) * (solution[index].img.cols - j);
				
			}
		}
		std::cout << solution[index].name << " " << whiteness << std::endl;
		if (whiteness > best_whiteness)
		{
			left_most_index = index;
			best_whiteness = whiteness;
		}
	}
	return left_most_index;
}


