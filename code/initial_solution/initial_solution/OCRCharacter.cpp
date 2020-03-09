#include "stdafx.h"
#include "OCRCharacter.h"
#define DEBUG


long double OCRCharacter::cost(Fragment &left_image, Fragment &right_image)
{
	
	//Combine images
	int best_height{ best_height_match(left_image,right_image) };
	Fragment thresh_left = left_image.get_thresholded();
	Fragment thresh_right = right_image.get_thresholded();
	Fragment combined_image{ combine(thresh_left,thresh_right,best_height) };
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
			if((right - left) > 2 and (bottom - top))
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
	
	


	tess.SetImage(mat_to_pix(image));
	tess.SetSourceResolution(70);

	// Get the text
	char* character = tess.GetUTF8Text();
	int* confidence = tess.AllWordConfidences();//Confidence score in range 0,100
	if ((*confidence) == -1)//Failed to identify 
	{
		return 0;
	}
#ifdef DEBUG
	std::cout << "character: " << character << " confidence: " << *confidence << std::endl;
	//cv::imwrite("d:/ocr/garbage.png", image);
	cv::namedWindow("char", cv::WINDOW_NORMAL);
	cv::imshow("char", image);
	cv::waitKey();
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

Pix *OCRCharacter::mat_to_pix(cv::Mat &image)
{

	Pix *pixd = pixCreate(image.size().width, image.size().height, 8);
	for (int y = 0; y<image.rows; y++) {
		for (int x = 0; x<image.cols; x++) {
			pixSetPixel(pixd, x, y, (l_uint32)image.at<uchar>(y, x));
		}
	}
	return pixd;
	
}



OCRCharacter::OCRCharacter()
{
	tess.Init("D:/Program Files/Tesseract-OCR/tessdata", "eng", tesseract::OEM_DEFAULT);//adjust
	tess.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);//Detect single character
}


OCRCharacter::~OCRCharacter()
{
	tess.End();
}


