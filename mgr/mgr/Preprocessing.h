#ifndef _PREPROCESSING_H_
#define _PREPROCESSING_H_

#include "opencv2/opencv.hpp"
using namespace cv;

class Preprocessing{
public:

	// Windowing
	void Windowing(Mat& image);

	// Filtration
	void Filter(Mat& image);

	// Thresholding
	void Threshold(Mat& image);

	// Dilate
	void Dilate(Mat& image);

	// Erode
	void Erode(Mat& image);

	// Normalize
	void Normalize(Mat& image);

	// overloaded constructor
	Preprocessing();

	// Virtual descrutor
	~Preprocessing();

private:
	Mat fingerprint;
};

#endif