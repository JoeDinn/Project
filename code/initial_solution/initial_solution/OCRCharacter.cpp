#include "stdafx.h"
#include "OCRCharacter.h"

#define DEBUG

long double OCRCharacter::cost(Fragment &left_image, Fragment &right_image)
{
	//Threshold images
	left_image.threshold();
	right_image.threshold();
	
	//Find characters in left side
	int *last_pixel_edge{ new int[left_image.image.rows] };
	for (int i{}; i < left_image.image.rows; ++i) last_pixel_edge[i] = left_image.last_pixel[i] - 1;
	std::vector<PotentialChar> left_characters{get_potential_characters(left_image, last_pixel_edge)};
	//Find characters in right side
	std::vector<PotentialChar> right_characters{ get_potential_characters(right_image,right_image.first_pixel) };
#ifdef DEBUG
	//////////////////////
	cv::Mat visualise_image{};
	cv::hconcat(left_image.image, right_image.image, visualise_image);
	cvtColor(visualise_image, visualise_image, cv::COLOR_GRAY2RGB);
	for (PotentialChar pot_cahr: left_characters)
	{
		cv::rectangle(visualise_image, cv::Point(pot_cahr.left, pot_cahr.top), cv::Point(pot_cahr.right, pot_cahr.bottom), CV_RGB(255, 0, 0), 1, 8, 0);
	}
	for (PotentialChar pot_cahr :right_characters)
	{
		cv::rectangle(visualise_image, cv::Point(pot_cahr.left + left_image.image.cols, pot_cahr.top), cv::Point(pot_cahr.right + left_image.image.cols, pot_cahr.bottom), CV_RGB(255, 0, 0), 1, 8, 0);
	}
	cv::imwrite("D:/ocr/chars.png", visualise_image);
	/////
#endif // DEBUG
	//Pad one to size of other with dummy chars
	pad(left_characters, right_characters.size() - left_characters.size());
	pad(right_characters, left_characters.size() - right_characters.size());
	//assign indexes to each other (use distance metric of closeness)
	//assign and combine images
	std::vector<cv::Mat> characters{ combine(left_characters,right_characters,left_image,right_image) };
	//for each image calculate confidence
	long double total_score{};
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

			right_most = MAX(right_most, col);

			left_most = MIN(left_most, col);
		
			grow_region(image, visited, row,MIN(col + 1, image.last_pixel[row] - 1) ,  left_most,  right_most);
			grow_region(image, visited, row,MAX(col - 1, image.first_pixel[row]), left_most, right_most);
			grow_region(image, visited, MIN(row + 1, image.image.rows), col, left_most, right_most);
			grow_region(image, visited, MAX(row - 1, 0), col,  left_most, right_most);
		}
}

std::vector<PotentialChar> OCRCharacter::get_potential_characters(Fragment & fragment, int * edge)
{
	std::vector<PotentialChar> characters{};

	cv::Mat visited(fragment.image.rows, fragment.image.cols, fragment.image.type(), 0.0);
	//Get row sums
	
	bool *is_text_line{ new bool[fragment.image.rows] };
	fragment.get_text_lines(is_text_line);
	//Find tops and bottoms of lines in left (iterate down histogram until find black)
	for (int row = 0; row < fragment.image.rows; row++)
	{
		//If there are black pixels in the row iterate until find black pixel on edge
		if (is_text_line[row])
		{
			int left{ edge[row] };
			int right{ edge[row] };
			int top{ row };
			int bottom{ row-1 };
			
			bool found{ false };
			while (is_text_line[row])
			{
				//If pixel overlaps edge and hasn't already been visited (need to allow for discontonuous shapes)
				if (fragment(row, edge[row]) == 0 and not(visited.at<uchar>(row,edge[row])))
				{
					//Flood fill to find left and right extreme to it
					grow_region(fragment, visited, row, edge[row], left, right);
				}
				//Increment row iterator
				++row;
			}
			bottom = row;
			//Append to vector
			if (bottom-top > 1 and right-left > 1)
			{
				characters.emplace_back(PotentialChar(bottom, top, right, left));
			}
		}
	}
	return characters;
}

std::vector<cv::Mat> OCRCharacter::combine(std::vector<PotentialChar> &left_characters, std::vector<PotentialChar> &right_characters, Fragment &left_image, Fragment &right_image)
{
#ifdef DEBUG
	cv::Mat visualise_image{};
	cv::hconcat(left_image.image, right_image.image, visualise_image);
	cvtColor(visualise_image, visualise_image, cv::COLOR_GRAY2RGB);
#endif // DEBUG

	//Formulate problem
	int dim{ (int)left_characters.size() };
	col *row_in_solution{ new col[dim] };
	row *col_in_solution{ new row[dim] };
	cost_value *u{ new cost_value[dim] };
	cost_value *v{ new cost_value[dim] };

	cost_value **assign_cost{ new cost_value*[dim] };
	for (int i = 0; i < dim; i++)
		assign_cost[i] = new cost_value[dim];

	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			if (i == j)
			{
				assign_cost[i][j] = BIG;
			}
			else if (left_characters[i].is_dummy or right_characters[j].is_dummy)
			{
				assign_cost[i][j] = 0; //Dummy matches are 0 cost (doesn't matter what they are)
			}
			else
			{
				assign_cost[i][j] = abs(left_characters[i].top - right_characters[j].top);//Cost = distance between top row of two characters  i = left, j = right);
			}
		}
	}
	Hungarian H{*this};
	H.lap(dim, assign_cost, row_in_solution, col_in_solution, u, v);

	std::vector<cv::Mat> characters{};
	for (int left_index = 0; left_index < dim; left_index++)
	{
		
		int right_index = row_in_solution[left_index];
		if (not(left_characters[left_index].is_dummy)and not(right_characters[right_index].is_dummy))
		{
			std::cout << cv::Point(left_characters[left_index].left, left_characters[left_index].top) << " " << cv::Point(left_characters[left_index].right, left_characters[left_index].bottom) << std::endl;
			std::cout << cv::Point(right_characters[right_index].left, right_characters[right_index].top) << " " << cv::Point(right_characters[right_index].right, right_characters[right_index].bottom) << std::endl;


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


#ifdef DEBUG
			//std::cout << "Left char at: " << left_characters[left_index].top;// << " Matched with: " << right_characters[right_index].top << std::endl;
			std::cout << "Left char at: " << left_characters[left_index].top << " Matched with: " << right_characters[right_index].top << std::endl;
			std::cout << cv::Point(left_characters[left_index].left, left_characters[left_index].top) << " " << cv::Point(left_characters[left_index].right, left_characters[left_index].bottom) << std::endl;
			std::cout << cv::Point(right_characters[right_index].left, right_characters[right_index].top) << " " << cv::Point(right_characters[right_index].right, right_characters[right_index].bottom) << std::endl;
			//cv::imwrite("D:/ocr/Whole.png", whole);///
			cv::rectangle(visualise_image, cv::Point(left_characters[left_index].left, left_characters[left_index].top), cv::Point(left_characters[left_index].right, left_characters[left_index].bottom), CV_RGB(255, 0, 0), 1, 8, 0);
			cv::rectangle(visualise_image, cv::Point(right_characters[right_index].left + left_image.image.cols, right_characters[right_index].top), cv::Point(right_characters[right_index].right + left_image.image.cols, right_characters[right_index].bottom), CV_RGB(255, 0, 0), 1, 8, 0);
			arrowedLine(visualise_image, cv::Point(left_characters[left_index].right, left_characters[left_index].bottom), cv::Point(right_characters[right_index].left + left_image.image.cols, right_characters[right_index].top), CV_RGB(255, 0, 0), 1, 8, 0, 0.1);
#endif // DEBUG
			//characters.emplace_back(whole);
		}
		
	}

#ifdef DEBUG
	cv::imwrite("D:/ocr/boxes.png", visualise_image);
#endif // DEBUG
	return characters;
}

void OCRCharacter::pad(std::vector<PotentialChar> &characters, int amount)
{
	for (int i = 0; i < amount; ++i)
	{
		characters.emplace_back(PotentialChar());
	}
}

OCRCharacter::OCRCharacter()
{
}


OCRCharacter::~OCRCharacter()
{
}

PotentialChar::PotentialChar(int bottom_, int top_, int right_, int left_, bool is_dummy_):
	bottom(bottom_), top(top_), right(right_), left(left_), is_dummy(is_dummy_)
{
}

PotentialChar::PotentialChar(bool is_dummy_):
	is_dummy(is_dummy_)
{
}