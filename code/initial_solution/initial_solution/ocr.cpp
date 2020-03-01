#include "stdafx.h"
#include "ocr.h"


long double ocr::cost(Fragment & leftImage, Fragment & rightImage)
{

	//


	return 0;
}

int ocr::word_score(cv::Mat image)
{

	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
	// ...other image pre-processing here...

	// Pass it to Tesseract API
	tesseract::TessBaseAPI tess;
	tess.Init("D:/Program Files/Tesseract-OCR/tessdata", "eng", tesseract::OEM_DEFAULT);
	tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
	tess.SetImage((uchar*)gray.data, gray.cols, gray.rows, 1, gray.cols);

	// Get the text
	char* words = tess.GetUTF8Text();
	int* confidence = tess.AllWordConfidences();
	std::cout << words << " " << *confidence << std::endl;

}


ocr::ocr()
{
}


ocr::~ocr()
{
}
