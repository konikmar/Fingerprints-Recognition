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

	// Zhang-Suen
	void ZhangSuenThinning(const Mat& src, Mat& dst);

	// Guo-Hall
	void GuoHallThinning(const Mat& src, Mat& dst);

	// Lu and Wang
	void LuWangThinning(const Mat& src, Mat& dst);

	// PRZYDATNE
	// http://shodhganga.inflibnet.ac.in/bitstream/10603/3466/10/10_chapter%202.pdf
	// https://www.ancient-asia-journal.com/articles/10.5334/aa.06114/print/

private:
	Mat fingerprint;
};

#endif