#ifndef _THINNING_H_
#define _THINNING_H_

#include "opencv2/opencv.hpp"
using namespace cv;

class Thinning{
public:

	void Negative(Mat& img);
	
	// Thinning 1 - ZHANG-SUEN
	void Thinning1(const Mat& src, Mat& dst);

	// Thinning iteration method
	void ThinningIteration(Mat& img, int iter);

	// Thinninh Negative 1 - ZHANG-SUEN
	void ThinningNegative1(const Mat& src, Mat& dst);


	// overloaded constructor
	Thinning();

	// Virtual descrutor
	~Thinning();

private:
	Mat fingerprint;
};

#endif