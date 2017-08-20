#ifndef _FALSEMINUTIAE_H_
#define _FALSEMINUTIAE_H_

#include "opencv2/opencv.hpp"
using namespace cv;

class FalseMinutiae{
public:
	// False Delta Minutiae cleaner
	void DeltaDetectionCleaner(const std::vector<int> SrcDeltaListX, const std::vector<int> SrcDeltaListY, Mat& dst, std::vector<int>& DeltaListX, std::vector<int>& DeltaListY);

	// False Ending Detecion cleaner
	void EndingDetectionCleaner(const std::vector<int> SrcEndListX, const std::vector<int> SrcEndListY, Mat& dst, std::vector<int>& EndListX, std::vector<int>& EndListY, std::vector<std::string> SrcDirection, std::vector<std::string> Direction);

	// Frame Mark
	void FrameMark(int x0, int y0, int x1, int y1, const std::vector<int> SrcEndListX, const std::vector<int> SrcEndListY, std::vector<int> OutEndListX, std::vector<int> OutEndListY, const std::vector<int> SrcDeltaListX, const std::vector<int> SrcDeltaListY, std::vector<int> OutDeltaListX, std::vector<int> OutDeltaListY, Mat& dst);
	///W SUMIE WARTO BY BYLO ZAZNACZAC TEN ROZWAZANY FRAGMENT WCZESNIEJ-DO UZGODNIENIA

	// overloaded constructor
	FalseMinutiae();

	// Virtual descrutor
	~FalseMinutiae();

private:
	Mat fingerprint;
};

#endif