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

Solution load_from_file(std::string root)
{

	std::ifstream file(root + "/" + root + ".txt");
	std::string str;
	std::getline(file, str);
	std::cout << str << std::endl;


	int number_files = std::stoi(str);
	Solution images(number_files);
	int image_counter = 0;
	while (std::getline(file, str))
	{
		images[image_counter] = Fragment(root, str); // read the file
		if (images[image_counter].img.empty()) // check for invalid input
		{
			std::cout << "could not open or find the image" << std::endl;
		}

		++image_counter;
	}
	return images;
}





int left_most(Solution & solution)
{
	int left_most_index{};
	long long  best_whiteness{};
	for (int index{}; index < solution.size(); ++index)
	{
		long long whiteness{ whiteness_score(solution[index])};

		std::cout << solution[index].name << " " << whiteness << std::endl;
		if (whiteness > best_whiteness)
		{
			left_most_index = index;
			best_whiteness = whiteness;
		}
	}
	std::cout << "Best: " << solution[left_most_index].name << " " << best_whiteness << std::endl;
	return left_most_index;
}

long long int whiteness_score(Fragment & fragment)
{
	long long int whiteness{};
	long int dim{};
	for (int i{}; i < fragment.img.rows; ++i)
	{
		dim += (fragment.last_pixel[i] - fragment.first_pixel[i]);
		for (int j{ fragment.first_pixel[i] }; j < fragment.last_pixel[i]; ++j)
		{
			//if (i == 680 and (fragment.name == "D00107" or fragment.name == "D00101"))std::cout << fragment.name << " " << i << " " << j << " " << (int)fragment(i, j) << " " << ((fragment.last_pixel[i] + 1) - j) << " " << (int)fragment(i, j) * ((fragment.last_pixel[i] + 1) - j) << std::endl;
			whiteness += (int)fragment(i, j) * ((fragment.last_pixel[i]) - j);

		}
		//std::cout << whiteness << std::endl;
	}
	return whiteness/dim;
}

