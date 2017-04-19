#ifndef _FINGERPRINTSRECOGNITION_H_
#define _FINGERPRINTSRECOGNITION_H_

#include "opencv2/opencv.hpp"
using namespace cv;

class FingerprintsRecognition{
public:

	// Preprocesing
	void Preprocesnig(Mat& image);

	// overloaded constructor
	FingerprintsRecognition(void);

	// GaborFilter
	void GaborFilter(Mat& image);

	// Thinning
	void Thinning(Mat& image);

	// Virtual descrutor
	~FingerprintsRecognition();

private:
	Mat fingerprint;
};

#endif