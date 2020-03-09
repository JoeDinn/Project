#include "stdafx.h"
#include "utilities.h"
#include <limits>
//#define DEBUG

void output(Solution &solution)
{
	std::cout << std::endl << "Size" << solution.size() << std::endl;
	Fragment image(solution[0]);
	for (int i{ 1 }; i < solution.size(); ++i)
	{
		//Fragment left_thresh = image.get_thresholded();
		//Fragment right_thresh = solution[i].get_thresholded();
		int right_start = best_height_match(image, solution[i]);
		image = combine_whole(image, solution[i]);
	}

	cv::imwrite("D:/ocr/bestheight.png", image.image);
	cv::namedWindow("Display window", cv::WINDOW_NORMAL); 
	cv::imshow("Display window", image.image); 
	cv::waitKey(0); 
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
#ifdef DEBUG
		std::cout << i << " <--> " << j << std::endl;
#endif // DEBUG
	}
#ifdef DEBUG
	std::cout << "done\n";
#endif // DEBUG
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
#ifdef DEBUG
	std::cout << "Best: " << solution[left_most_index].name << " " << best_whiteness << std::endl;
#endif // DEBUG
	return left_most_index;
}


Fragment combine(Fragment &left_image, Fragment &right_image, int right_start)
{
	int max_cols{};

	int total_rows = left_image.image.rows - abs(right_start);
#ifdef DEBUG
	std::cout << "total rows: " << total_rows << std::endl;
#endif // DEBUG
	for (int row = 0; row < total_rows; row++)
	{
		int left_row = MAX(row, row + right_start);
		int right_row = MAX(row, row - right_start);
		max_cols = MAX(left_image.last_pixel[left_row] + right_image.last_pixel[right_row] - right_image.first_pixel[right_row], max_cols);
	}		
#ifdef DEBUG
	std::cout << "Max cols: " << max_cols << std::endl;
#endif // DEBUG
	int *first_pixel = new int[total_rows]{};
	int *last_pixel = new int[total_rows]{};
	cv::Mat combined_image(total_rows, max_cols, left_image.image.type());
	combined_image = 0;

	for (int row = 0; row < total_rows; row++)
	{
		int left_row = MAX(row, row + right_start);
		int right_row = MAX(row, row - right_start);

		first_pixel[row] = left_image.first_pixel[left_row];
		last_pixel[row] = (left_image.last_pixel[left_row] + right_image.last_pixel[right_row]) - right_image.first_pixel[right_row];


		for (int col = 0; col < left_image.last_pixel[left_row]; col++)
		{
			combined_image.at<uchar>(row, col) = left_image(left_row, col);
		}
			
		for (int col = right_image.first_pixel[right_row]; col < right_image.last_pixel[right_row]; col++)
		{
			combined_image.at<uchar>(row, col + (left_image.last_pixel[left_row] - right_image.first_pixel[right_row])) = right_image(right_row, col);
		}
		
	}

	return Fragment(combined_image, left_image.name + right_image.name, first_pixel, last_pixel);
}

Fragment combine_whole(Fragment &left_image, Fragment &right_image, int right_start)
{
	cv::Mat combined_image;
	cv::Mat left_padded(left_image.image.rows + abs(right_start), left_image.image.cols , left_image.image.type());
	cv::Mat right_padded(left_image.image.rows + abs(right_start), right_image.image.cols, left_image.image.type());
	cv::Mat pad = cv::Mat::zeros(abs(right_start), left_image.image.cols, left_image.image.type());    // 3 cols, 4 rows
	if (right_start > 0)//left image starts at 0
	{
		//Pad left below
		cv::vconcat(left_padded, pad, left_padded);
		//Pad right above
		cv::vconcat(pad, right_padded, right_padded);
	}
	else if (right_start < 0)//Right image starts above
	{
		//Pad left above
		cv::vconcat(pad, left_padded, left_padded);
		//Pad right below
		cv::vconcat(right_padded, pad, right_padded);
	}
	else
	{
		return combine(left_image,right_image);
	}
	cv::hconcat(left_padded, right_padded, combined_image);


	return Fragment(combined_image, left_image.name + right_image.name, left_image.first_pixel, left_image.last_pixel);//Last and first dont matter
}


int best_height_match(Fragment & left_image, Fragment & right_image,long double *cost)
{
	//Iterate through each possibility and return index of best result.
	int best_right_start{};
	double best_score{ std::numeric_limits<double>::max() };

	for (int right_start = -floor(left_image.image.cols/4); right_start < floor(left_image.image.cols / 4); right_start++)
	{
		double score{0};
		int total_rows = left_image.image.rows - abs(right_start);
		for (int row = 0; row < total_rows; row++)
		{
			int left_row = MAX(row, row + right_start);
			int right_row = MAX(row, row - right_start);
			score += abs(left_image(left_row, left_image.last_pixel[left_row]-1) - right_image(right_row,right_image.first_pixel[right_row]));
		}
		score = score / total_rows;
		if (score < best_score)
		{
			best_score = score;
			best_right_start = right_start;
		}
	}
	if(cost) *cost = best_score;
	return best_right_start;
}





long long int whiteness_score(Fragment & fragment)
{
	long long int whiteness{};
	long int dim{};
	for (int i{}; i < fragment.image.rows; ++i)
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

