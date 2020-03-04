#include "stdafx.h"
#include "OCRCharacter.h"

#define DEBUG

long double OCRCharacter::cost(Fragment &left_image, Fragment &right_image)
{

	std::vector<cv::Mat> characters{};
	cv::Mat left_visited(left_image.img.rows, left_image.img.cols, left_image.img.type(), 0.0);
	cv::Mat right_visited(right_image.img.rows, right_image.img.cols, right_image.img.type(), 0.0);
	
	//Threshold images
	left_image.threshold();
	right_image.threshold();

	//Get row sums
	bool *left_is_text_line{new bool[left_image.img.rows]};
	left_image.get_text_lines(left_is_text_line);

#ifdef DEBUG
	cv::Mat visualise_img{};
	cv::hconcat(left_image.img, right_image.img, visualise_img);
	cvtColor(visualise_img, visualise_img, cv::COLOR_GRAY2RGB);
	std::cout << "row 620: " << left_is_text_line[369] << std::endl;///////
	bool *right_is_text_line{ new bool[right_image.img.rows] };
	right_image.get_text_lines(right_is_text_line);
#endif // DEBUG
		
	
	//Find tops and bottoms of lines in left (iterate down histogram until find black)
	for (int row = 0; row < left_image.img.rows; row++)
	{
		
		//If there are black pixels in the row
		if (left_is_text_line[row])
		{
			
			int left_img_left{ left_image.last_pixel[row] - 1 };
			int left_img_right{ left_image.last_pixel[row] - 1 };
			int left_img_top{ row };
			int left_img_bottom;
			int left_img_height;

			int right_img_left;
			int right_img_right;
			int right_img_top;
			int	right_img_bottom;
			int right_img_height;

			bool found{ false };
			bool right_found{ false };
			while (left_is_text_line[row])
			{
				//If char overlaps edge
				if (left_image(row, left_image.last_pixel[row] - 1) == 0 and not(found))
				{
					std::cout << row << " " << left_image.last_pixel[row] - 1 << " " << (int)left_image(row, left_image.last_pixel[row] - 1) << " "  << std::endl;
					found = true;
					//Fit box to left hand char (l and r)
					grow_region(left_image, left_visited, row, left_image.last_pixel[row] - 1, left_img_left, left_img_right);
#ifdef DEBUG
					std::cout << "fin " << row << " Left: " << left_img_left << " Right: " << left_img_right << std::endl;
					
#endif // DEBUG
				}
				++row;
			}
			if (found)//If a character is found that overlaps the edge on the right
			{
				//Find right hand half of charcter
				//For now just assume same box
				for (size_t i = 0; i < length; i++)
				{

				}
				right_img_left = right_image.first_pixel[row];
				right_img_right = right_image.first_pixel[row] + abs(left_img_right - left_img_left);
				right_img_top = left_img_top;
				right_found = true;





				left_img_height = (row - 1) - left_img_top;
				left_img_bottom = left_img_top + MAX(left_img_height, right_img_height);
				right_img_bottom = right_img_top + MAX(left_img_height, right_img_height);
#ifdef DEBUG
				std::cout << "Left box: " << left_img_left << " " << left_img_right << " " << left_img_top << " " << left_img_bottom << std::endl;
				std::cout << "right box: " << right_img_left << " " << right_img_right << " " << left_img_top << " " << left_img_bottom << std::endl;
				std::cout << "                  " << cv::Point(left_img_top, left_img_left) << " " << cv::Point(left_img_bottom, left_img_right) << std::endl;
				cv::rectangle(visualise_img, cv::Point(left_img_left, left_img_top), cv::Point(left_img_right, left_img_bottom), CV_RGB(255, 0, 0), 1,  8,  0);
				cv::rectangle(visualise_img, cv::Point(right_img_left + left_image.img.cols, right_img_top), cv::Point(right_img_right + left_image.img.cols, right_img_bottom), CV_RGB(255, 0, 0), 1, 8, 0);
#endif // DEBUG
				//join boxes and add to vector
				cv::Mat left_half(
					left_image.img,
					cv::Range(left_img_top, left_img_bottom + 1), // rows
					cv::Range(left_img_left, left_img_right + 1));// cols

				cv::Mat right_half(
					right_image.img,
					cv::Range(right_img_top, right_img_bottom + 1), // rows
					cv::Range(right_img_left, right_img_right + 1));// cols

				cv::Mat whole;
				cv::hconcat(left_half, right_half, whole);

				std::cout << characters.size() << " Characters found so far" << std::endl;
				characters.emplace_back(whole);
			}
		}
	}
	//for each combined image
	//character_score(image)
	//sum wordscores
#ifdef DEBUG
	std::cout << characters.size() << " Characters found" << std::endl;
	cv::namedWindow("Firstimage");///////
	cv::imshow("Firstimage", characters[0]);////////
	cv::namedWindow("visualise", cv::WINDOW_NORMAL);///////
	cv::imshow("visualise", visualise_img);////////
	cv::imwrite("D:/ocr/foundchars.png", visualise_img);
	cv::waitKey();

#endif // DEBUG

	





	long int total_score{};
	for (cv::Mat character : characters)
	{
		int score{ char_score(character) };
		total_score += score;
	}
	std::cout << "Total score " << total_score << std::endl;
	return total_score;
}





int OCRCharacter::char_score(cv::Mat &image)
{
	std::cout << image.size << std::endl;
	
	// ...other image pre-processing here...

	// Pass it to Tesseract API
	tesseract::TessBaseAPI tess;
	tess.Init("D:/Program Files/Tesseract-OCR/tessdata", "eng", tesseract::OEM_DEFAULT);//adjust
	tess.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);//Detect single character
	tess.SetImage((uchar*)image.data, image.cols, image.rows, 1, image.cols);

	// Get the text
	char* words = tess.GetUTF8Text();
	int* confidence = tess.AllWordConfidences();//Confidence score
	std::cout << words << " " << *confidence << std::endl;
	return *confidence;
}

void OCRCharacter::grow_region(Fragment &image, cv::Mat &visited, int row, int col, int &left_most, int &right_most)
{
	if(col < image.last_pixel[row] and col >= image.first_pixel[row])
		if (image(row, col) == 0 and visited.at<uchar>(row, col) == 0)
		{
			visited.at<uchar>(row, col) = 1;
			
			if (col > right_most)
			{
				right_most = col;
			}
			else if (col < left_most)
			{
				left_most = col;
			}
		
			grow_region(image, visited, row,MIN(col + 1, image.last_pixel[row] - 1) ,  left_most,  right_most);
			grow_region(image, visited, row,MAX(col - 1, image.first_pixel[row]), left_most, right_most);
			grow_region(image, visited, MIN(row + 1, image.img.rows), col, left_most, right_most);
			grow_region(image, visited, MAX(row - 1, 0), col,  left_most, right_most);
		}
}


OCRCharacter::OCRCharacter()
{
}


OCRCharacter::~OCRCharacter()
{
}


/*
int right_row{ top };

while (right_is_text_line[right_row] and not(right_found))
{
if (right_image(right_row, right_image.first_pixel[right_row]) == 0 and not(right_found))//Find top and bottom of right char too
{
//fit box to right hand char (l and r)
right_img_left = right_image.first_pixel[right_row];
right_img_right = right_image.first_pixel[right_row];
grow_region(right_image, right_visited, right_row, right_image.first_pixel[row], right_img_left, right_img_right);
right_found = true;
}
++right_row;
}
*/