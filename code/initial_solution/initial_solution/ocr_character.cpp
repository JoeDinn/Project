#include "stdafx.h"
#include "ocr_character.h"


long double ocr_character::cost(Fragment &left_image, Fragment &right_image)
{
	std::cout << "cost" << std::endl;//////


	std::vector<cv::Mat> characters{};
	cv::Mat left_copy(left_image.img.rows, left_image.img.cols, left_image.img.type(), 0.0);
	cv::Mat right_copy(right_image.img.rows, right_image.img.cols, right_image.img.type(), 0.0);
	left_copy.copySize(left_image.img);
	right_copy.copySize(right_image.img);

	left_image.threshold();
	right_image.threshold();
	cv::imwrite("d:/ocr/threshold_right.png", right_image.img);///////////
	//Threshold images
	//Bound objects
	//For each index
	std::cout << left_image.img.rows << " sdsa" << std::endl;
	for (int row = 0; row < left_image.img.rows; row++)
	{
		//std::cout << "sdas" << std::endl;
		//Starting at rightmost point
		//If pixel in a character:
		//std::cout << (int)left_image.img.at<uchar>(row, left_image.last_pixel[row]) << std::endl;
		
		if (left_image.img.at<uchar>(row, left_image.last_pixel[row] - 1) == 0)
		{
			std::cout << "Char at: " << row << std::endl;//////////
			int top_most{ row };
			int	bottom_most{ row };
			int	left_most{ left_image.last_pixel[row] - 1 };
			int	right_most{ left_image.last_pixel[row] - 1 };
			//Grow region and find bounding box
			grow_region(left_image, left_copy, row, left_image.last_pixel[row]-1, top_most, bottom_most, left_most, right_most);
			
			std::cout << "box at: " << top_most << " " << bottom_most << " " << left_most << " " << right_most << std::endl;//////////


			//Look for corresponding pixel within box height in right image
			bool found{ false };
			for (int right_row = top_most; right_row < bottom_most; right_row++)
			{
				if (right_image.img.at<uchar>(right_row, right_image.first_pixel[right_row]) == 0 and not(found))
				{
					std::cout << "Right box found at: " << right_row << std::endl;///////////
					found = true;
					int top_most_right{ right_row };
					int	bottom_most_right{ right_row };
					int	left_most_right{ right_image.first_pixel[right_row] };
					int	right_most_right{ right_image.first_pixel[right_row] };
					//grow region in horizontal dimension
					grow_region(right_image, right_copy,right_row, right_image.first_pixel[right_row], top_most_right, bottom_most_right, left_most_right, right_most_right);
					std::cout << "box at: " << top_most_right << " " << bottom_most_right << " " << left_most_right << " " << right_most_right << std::endl;//////////

					//join boxes to form image and add to characters list
					cv::Mat left_half(
						left_image.img,
						cv::Range(top_most, bottom_most), // rows
						cv::Range(left_most, right_most));// cols

					cv::Mat right_half(
						right_image.img,
						cv::Range(top_most, bottom_most), // rows
						cv::Range(left_most_right, right_most_right));// cols
					cv::Mat whole;
					cv::hconcat(left_half, right_half, whole);
					characters.emplace_back(whole);
				}
			}
			//increment i to skip pixels within
			row = bottom_most + 1;
		}
	
		
	}
	//for each combined image
	//character_score(image)
	//sum wordscores
	std::cout << characters.size() << " Characters found" << std::endl;

	long int total_score{};
	for (cv::Mat character : characters)
	{
		int score{ char_score(character) };
		total_score += score;
	}
	return total_score;
	
}

int ocr_character::char_score(cv::Mat image)
{

	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
	// ...other image pre-processing here...

	// Pass it to Tesseract API
	tesseract::TessBaseAPI tess;
	tess.Init("D:/Program Files/Tesseract-OCR/tessdata", "eng", tesseract::OEM_DEFAULT);//adjust
	tess.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);//Detect single character
	tess.SetImage((uchar*)gray.data, gray.cols, gray.rows, 1, gray.cols);

	// Get the text
	char* words = tess.GetUTF8Text();
	int* confidence = tess.AllWordConfidences();//Confidence score
	std::cout << words << " " << *confidence << std::endl;
	return *confidence;
}

void ocr_character::grow_region(Fragment &image, cv::Mat &mask, int row, int col, int &top_most, int &bottom_most, int &left_most, int &right_most)
{
	//std::cout << row << " " << col << std::endl;
	if(col < image.last_pixel[row] and col >= image.first_pixel[row])
		if (image.img.at<uchar>(row, col) == 0 and mask.at<uchar>(row, col) == 0)
		{
			mask.at<uchar>(row, col) = 1;
			if (row > bottom_most)
			{
				bottom_most = row;
			}
			else if (row < top_most)
			{
				top_most = row;
			}
			if (col > right_most)
			{
				right_most = col;
			}
			else if (col < left_most)
			{
				left_most = col;
			}
		
			grow_region(image, mask, row,MIN(col + 1, image.last_pixel[row] - 1) , top_most, bottom_most,  left_most,  right_most);
			grow_region(image, mask, row,MAX(col - 1, image.first_pixel[row]), top_most, bottom_most, left_most, right_most);
			grow_region(image, mask, MIN(row + 1, image.img.rows), col, top_most, bottom_most, left_most, right_most);
			grow_region(image, mask, MAX(row - 1, 0), col, top_most, bottom_most, left_most, right_most);
		}
}


ocr_character::ocr_character()
{
}


ocr_character::~ocr_character()
{
}
