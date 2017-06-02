#ifndef _DETECTION_H_
#define _DETECTION_H_

#include "opencv2/opencv.hpp"
using namespace cv;

class Detection{
public:
	// Delta Detection
	void DeltaDetection(const Mat& src, Mat& dst);

	// Ending Detecion
	void EndingDetection(const Mat& src, Mat& dst);

	// Analysis of pixels from border of the 5x5 window
	int Frame5x5(const Mat& src, int, int);


	// overloaded constructor
	Detection();

	// Virtual descrutor
	~Detection();

private:
	Mat fingerprint;
};

#endif