#ifndef _DETECTION_H_
#define _DETECTION_H_

#include "opencv2/opencv.hpp"
using namespace cv;

class Detection{
public:
	// Delta Detection
	void DeltaDetection(const Mat& src, Mat& dst, std::vector<int> &DeltaListX, std::vector<int> &DeltaListY, std::vector<std::string> &Direction);

	// Ending Detecion
	void EndingDetection(const Mat& src, Mat& dst, std::vector<int> &EndListX, std::vector<int> &EndListY, std::vector<std::string> &Direction);

	// Delta Detection
	void DeltaDetectionCN(const Mat& src, Mat& dst, std::vector<int> &DeltaListX, std::vector<int> &DeltaListY, std::vector<std::string> &Direction);

	// Ending Detecion
	void EndingDetectionCN(const Mat& src, Mat& dst, std::vector<int> &EndListX, std::vector<int> &EndListY, std::vector<std::string> &Direction);


	// Delta Detection
	void DeltaDetection2(const Mat& src, Mat& dst, std::vector<int> &DeltaListX, std::vector<int> &DeltaListY, std::vector<std::string> &Direction);
	// overloaded constructor
	Detection();

	// Virtual descrutor
	~Detection();

private:
	Mat fingerprint;
};

#endif