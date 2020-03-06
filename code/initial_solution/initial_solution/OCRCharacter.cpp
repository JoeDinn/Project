#include "stdafx.h"
#include "OCRCharacter.h"
#define DEBUG


long double OCRCharacter::cost(Fragment &left_image, Fragment &right_image)
{
	//Combine images
	int best_height{ best_height_match(left_image,right_image) };
	Fragment combined_image{ combine(left_image,right_image,best_height) };
	cv::Mat visited(combined_image.image.rows, combined_image.image.cols, combined_image.image.type(), 0.0);
	//Threshold
	combined_image.threshold();
	//Get lines
	bool *is_text_line{ new bool[combined_image.image.rows] };
	combined_image.get_text_lines(is_text_line);

	long double score{ 0 };
	int number_of_chars{ 0 };
#ifdef DEBUG
	cv::Mat visualise_image{ combined_image.image };
	cvtColor(visualise_image, visualise_image, cv::COLOR_GRAY2RGB);
#endif//DEBUG
	//For each line of text
	for (int row = 0; row < combined_image.image.rows; row++)
	{
		if (is_text_line[row])
		{
			//Line of text found
			int adjusted_row = MAX(row, row + best_height);
			int left{ left_image.last_pixel[adjusted_row] };
			int right{ left_image.last_pixel[adjusted_row] };
			int top{ row };
			int bottom{ row - 1 };
			//Iterate over each row of line of text
			while (is_text_line[row])
			{
				adjusted_row = MAX(row, row + best_height);
				//If character overlaps centre boundary and hasn't already been visited (need to allow for discontonuous shapes)
				if (combined_image(row, left_image.last_pixel[adjusted_row]) == 0 and not(visited.at<uchar>(row, left_image.last_pixel[adjusted_row])))
				{
					//Flood fill to find left and right extreme to it
					grow_region(combined_image, visited, row, left_image.last_pixel[row], left, right);
				}
				//Increment row iterator
				++row;
			}
			if(left != right)
			{
				bottom = row - 1;
				//Append to vector
				cv::Mat character_image(combined_image.image,
					cv::Range(top, bottom + 1), // rows
					cv::Range(left, right + 1));// cols
#ifdef DEBUG
				cv::rectangle(visualise_image, cv::Point(left - 1, top - 1), cv::Point(right + 1, bottom + 1), CV_RGB(255, 0, 0), 1, 8, 0);
#endif // DEBUG
				score -= char_score(character_image);
				number_of_chars++;
			}
		}
	}
#ifdef DEBUG
	cv::imwrite("d:/ocr/Visualisation.png",visualise_image);
#endif // DEBUG


	return score/number_of_chars;
}

int OCRCharacter::char_score(cv::Mat &image)
{

	// ...other image pre-processing here...

	// Pass it to Tesseract API
	tesseract::TessBaseAPI tess;
	tess.Init("D:/Program Files/Tesseract-OCR/tessdata", "eng", tesseract::OEM_DEFAULT);//adjust
	tess.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);//Detect single character
#ifdef DEBUG
	std::cout << image.data << std::endl;
#endif // DEBUG

	tess.SetImage((uchar*)image.data, image.cols, image.rows, 1, image.cols);
	tess.SetSourceResolution(70);

	// Get the text
	char* character = tess.GetUTF8Text();
	int* confidence = tess.AllWordConfidences();//Confidence score in range 0,100
#ifdef DEBUG
	std::cout << "character: " << character << " confidence: " << *confidence << std::endl;
#endif // DEBUG
	return  -*confidence;
}

void OCRCharacter::grow_region(Fragment &image, cv::Mat &visited, int row, int col, int &left_most, int &right_most)
{
	if (col < image.last_pixel[row] and col >= image.first_pixel[row])
		if (image(row, col) == 0 and visited.at<uchar>(row, col) == 0)
		{
			visited.at<uchar>(row, col) = 1;

			right_most = MAX(right_most, col);

			left_most = MIN(left_most, col);

			grow_region(image, visited, row, MIN(col + 1, image.last_pixel[row] - 1), left_most, right_most);
			grow_region(image, visited, row, MAX(col - 1, image.first_pixel[row]), left_most, right_most);
			grow_region(image, visited, MIN(row + 1, image.image.rows), col, left_most, right_most);
			grow_region(image, visited, MAX(row - 1, 0), col, left_most, right_most);
		}
}



OCRCharacter::OCRCharacter()
{
}


OCRCharacter::~OCRCharacter()
{
}



/*
//#define DEBUG

long double OCRCharacter::cost(Fragment &left_image, Fragment &right_image)
{
	

	cv::Mat visited(combined_image.image.rows, combined_image.image.cols, combined_image.image.type(), 0.0);
	//Get row sums

	bool *is_text_line{ new bool[combined_image.image.rows] };
	combined_image.get_text_lines(is_text_line);
	//Find tops and bottoms of lines in left (iterate down histogram until find black)
	for (int row = 0; row < combined_image.image.rows; row++)
	{
		//If there are black pixels in the row iterate until find black pixel on edge
		if (is_text_line[row])
		{
			int left{ left_image.last_pixel[row] };
			int right{ left_image.last_pixel[row] };
			int top{ row };
			int bottom{ row - 1 };

			bool found{ false };
			while (is_text_line[row])
			{
				//If pixel overlaps edge and hasn't already been visited (need to allow for discontonuous shapes)
				if (combined_image(row, left_image.last_pixel[row]) == 0 and not(visited.at<uchar>(row, left_image.last_pixel[row])))
				{
					//Flood fill to find left and right extreme to it
					grow_region(combined_image, visited, row, left_image.last_pixel[row], left, right);
				}
				//Increment row iterator
				++row;
			}
			bottom = row;
			//Append to vector
			cv::Mat character_image(combined_image.image,
				cv::Range(top, left_characters[left_index].bottom + 1), // rows
				cv::Range(left_characters[left_index].left, left_characters[left_index].right + 1));// cols


			characters.emplace_back(PotentialChar(bottom, top, right, left));

		}
	}
	return characters;










	//Find characters in left side
	int *last_pixel_edge{ new int[left_image_thresholded.image.rows] };
	for (int i{}; i < left_image_thresholded.image.rows; ++i) last_pixel_edge[i] = left_image_thresholded.last_pixel[i] - 1;
	std::vector<PotentialChar> left_characters{get_potential_characters(left_image_thresholded, last_pixel_edge)};
	//Find characters in right side
	std::vector<PotentialChar> right_characters{ get_potential_characters(right_image_thresholded,right_image_thresholded.first_pixel) };
#ifdef DEBUG
	//////////////////////
	cv::Mat visualise_image{};
	cv::hconcat(left_image.image, right_image.image, visualise_image);
	cvtColor(visualise_image, visualise_image, cv::COLOR_GRAY2RGB);
	for (PotentialChar pot_cahr: left_characters)
	{
		cv::rectangle(visualise_image, cv::Point(pot_cahr.left, pot_cahr.top), cv::Point(pot_cahr.right, pot_cahr.bottom), CV_RGB(255, 0, 0), 1, 8, 0);
	}
	cv::imwrite("D:/ocr/chars.png", visualise_image);
	/////
#endif // DEBUG
	//Prune

	//Pad one to size of other with dummy chars
	pad(left_characters, right_characters.size() - left_characters.size());
	pad(right_characters, left_characters.size() - right_characters.size());
	//assign indexes to each other (use distance metric of closeness)
	//assign and combine images
	
	//for each image calculate confidence
	long double total_score{};
	for (cv::Mat character : characters)
	{
#ifdef DEBUG
		std::cout << "current score " << total_score << std::endl;
#endif // DEBUG
		int score{ char_score(character) };
		total_score += score;
	}
#ifdef DEBUG
	std::cout << "Total score " << total_score << std::endl;
#endif // DEBUG
	return total_score;
}
/*



/*
//Cut out rectangles
cv::Mat left_half(left_image.image,
cv::Range(left_characters[left_index].top, left_characters[left_index].bottom + 1), // rows
cv::Range(left_characters[left_index].left, left_characters[left_index].right + 1));// cols
cv::Mat right_half(right_image.image,
cv::Range(right_characters[right_index].top, right_characters[right_index].bottom + 1), // rows
cv::Range(right_characters[right_index].left, right_characters[right_index].right + 1));// cols
//Resize to be equal
if (left_half.rows < right_half.rows)
{
cv::Mat pad( right_half.rows - left_half.rows, left_half.cols, 0);
cv::vconcat(left_half, pad, left_half);
}
else if (right_half.rows < left_half.rows)
{
cv::Mat pad( left_half.rows - right_half.rows, right_half.cols, 0 );
cv::vconcat(right_half, pad, right_half);
}
//Concatenate
cv::Mat whole;
cv::hconcat(left_half, right_half, whole);
characters.emplace_back(whole);
*/