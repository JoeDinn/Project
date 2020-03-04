#include "stdafx.h"
#include "OCRCharacter.h"


long double OCRCharacter::cost(Fragment &left_image, Fragment &right_image)
{
	std::vector<cv::Mat> characters{};
	cv::Mat left_visited(left_image.img.rows, left_image.img.cols, left_image.img.type(), 0.0);
	cv::Mat right_visited(right_image.img.rows, right_image.img.cols, right_image.img.type(), 0.0);


	//Threshold images
	left_image.threshold();
	right_image.threshold();
	cv::imwrite("D:/ocr/testthresh.png", left_image.img);/////
	//Get row sums

	bool *left_is_text_line{new bool[left_image.img.rows]};
	left_image.get_text_lines(left_is_text_line);
	std::cout << "row 620: " << left_is_text_line[369] << std::endl;///////
	bool *right_is_text_line{ new bool[right_image.img.rows] };
	right_image.get_text_lines(right_is_text_line);
	
	
	//Find tops and bottoms of lines in left (iterate down histogram until find black)
	for (int row = 0; row < left_image.img.rows; row++)
	{
		
		//If there are black pixels in the row
		if (left_is_text_line[row])
		{
			int top{ row };

			int left{ left_image.last_pixel[row] - 1 };
			int right{ left_image.last_pixel[row] - 1 };

			int right_img_left;
			int right_img_right;
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
					grow_region(left_image, left_visited, row, left_image.last_pixel[row] - 1, left, right);
					std::cout << "fin " << row << " Left: " << left << " Right: " << right << std::endl;
					//Find right hand half of char
					//For now just assume same box
					right_img_left = right_image.first_pixel[row];
					right_img_right = right_image.first_pixel[row] + abs(right-left);
					right_found = true;
				}
				++row;
			}
			if (found and right_found)
			{
				int bottom{ row - 1 };

				std::cout << "Left box: " << left << " " << right << " " << top << " " << bottom << std::endl;
				std::cout << "right box: " << right_img_left << " " << right_img_right << " " << top << " " << bottom << std::endl;

				//join boxes and add to vector
				cv::Mat left_half(
					left_image.img,
					cv::Range(top, bottom + 1), // rows
					cv::Range(left, right + 1));// cols

				cv::Mat right_half(
					right_image.img,
					cv::Range(top, bottom + 1), // rows
					cv::Range(right_img_left, right_img_right + 1));// cols

				cv::Mat whole;
				cv::hconcat(left_half, right_half, whole);
				/*
				cv::namedWindow("left_half");///////
				cv::imshow("left_half", left_half);////////

				cv::namedWindow("right_half");///////
				cv::imshow("right_half", right_half);////////

				cv::namedWindow("whole");///////
				cv::imshow("whole", whole);////////
				cv::waitKey();
				
				std::cout << whole.size << std::endl;
				*/
				std::cout << characters.size() << " Characters found so far" << std::endl;
				characters.emplace_back(whole);
			}
		}
	}
	//for each combined image
	//character_score(image)
	//sum wordscores
	std::cout << characters.size() << " Characters found" << std::endl;
	cv::namedWindow("Firstimage");///////
	cv::imshow("Firstimage", characters[0]);////////
	cv::waitKey();

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
	//std::cout << row << " " << col << std::endl;
	if(col < image.last_pixel[row] and col >= image.first_pixel[row])
		if (image(row, col) == 0 and visited.at<uchar>(row, col) == 0)
		{
			//std::cout << "grow: " << row << " " << col << std::endl;
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