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


	// overloaded constructor
	Detection();

	// Virtual descrutor
	~Detection();

private:
	Mat fingerprint;
};

#endif