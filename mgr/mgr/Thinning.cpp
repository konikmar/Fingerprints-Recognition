#include "Thinning.h"
#include "opencv2/opencv.hpp"

using namespace cv;

// overloaded constructor
Thinning::Thinning(){
}
void Thinning::Negative(Mat& img)
{
	cv::MatIterator_<uchar> it, end;
	for (it = img.begin<uchar>(), end = img.end<uchar>(); it != end; ++it)
	{
		*it = 255 - *it;
	}
}
void Thinning::Thinning1(const Mat& src, Mat& dst){
	Negative(dst);
	dst = src.clone();
	dst /= 255;         // convert to binary image

	cv::Mat prev = cv::Mat::zeros(dst.size(), CV_8UC1);
	cv::Mat diff;

	do {
		ThinningIteration(dst, 0);
		ThinningIteration(dst, 1);
		cv::absdiff(dst, prev, diff);
		dst.copyTo(prev);
	} while (cv::countNonZero(diff) > 0);

	dst *= 255;
	
}
void Thinning::ThinningIteration(Mat& img, int iter){

	CV_Assert(img.channels() == 1);
	CV_Assert(img.depth() != sizeof(uchar));
	CV_Assert(img.rows > 3 && img.cols > 3);

	cv::Mat marker = cv::Mat::zeros(img.size(), CV_8UC1);

	int nRows = img.rows;
	int nCols = img.cols;

	if (img.isContinuous()) {
		nCols *= nRows;
		nRows = 1;
	}

	int x, y;
	uchar *pAbove;
	uchar *pCurr;
	uchar *pBelow;
	uchar *nw, *no, *ne;    // north (pAbove)
	uchar *we, *me, *ea;
	uchar *sw, *so, *se;    // south (pBelow)

	uchar *pDst;

	// initialize row pointers
	pAbove = NULL;
	pCurr = img.ptr<uchar>(0);
	pBelow = img.ptr<uchar>(1);

	for (y = 1; y < img.rows - 1; ++y) {
		// shift the rows up by one
		pAbove = pCurr;
		pCurr = pBelow;
		pBelow = img.ptr<uchar>(y + 1);

		pDst = marker.ptr<uchar>(y);

		// initialize col pointers
		no = &(pAbove[0]);
		ne = &(pAbove[1]);
		me = &(pCurr[0]);
		ea = &(pCurr[1]);
		so = &(pBelow[0]);
		se = &(pBelow[1]);

		for (x = 1; x < img.cols - 1; ++x) {
			// shift col pointers left by one (scan left to right)
			nw = no;
			no = ne;
			ne = &(pAbove[x + 1]);
			we = me;
			me = ea;
			ea = &(pCurr[x + 1]);
			sw = so;
			so = se;
			se = &(pBelow[x + 1]);

			int A = (*no == 0 && *ne == 1) + (*ne == 0 && *ea == 1) +
				(*ea == 0 && *se == 1) + (*se == 0 && *so == 1) +
				(*so == 0 && *sw == 1) + (*sw == 0 && *we == 1) +
				(*we == 0 && *nw == 1) + (*nw == 0 && *no == 1);
			int B = *no + *ne + *ea + *se + *so + *sw + *we + *nw;
			int m1 = iter == 0 ? (*no * *ea * *so) : (*no * *ea * *we);
			int m2 = iter == 0 ? (*ea * *so * *we) : (*no * *so * *we);

			if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
				pDst[x] = 1;
		}
	}

	img &= ~marker;
}

void Thinning::ThinningNegative1(const Mat& src, Mat& dst){
	dst = src.clone();
	dst /= 255;         // convert to binary image

	cv::Mat prev = cv::Mat::zeros(dst.size(), CV_8UC1);
	cv::Mat diff;

	do {
		ThinningIteration(dst, 0);
		ThinningIteration(dst, 1);
		cv::absdiff(dst, prev, diff);
		dst.copyTo(prev);
	} while (cv::countNonZero(diff) > 0);

	dst *= 255;
}

Thinning::~Thinning(){
}

void Thinning::ZhangSuenThinning(const Mat& src, Mat& dst) // na podstawie: https://rosettacode.org/wiki/Zhang-Suen_thinning_algorithm
{
	int P1, P2, P3, P4, P5, P6, P7, P8, P9;
	int A=0; // number of transitions from 255 to 0 in the sequence P2,P3,P4,P5,P6,P7,P8,P9,P2
	int B=0; // number of black pixel neighbours of P1
	bool change = true;

	while (change == true)
	{

		change = false;

		// STEP 1
		for (int i = 1; i < src.rows - 1; i++)
		{
			for (int j = 1; j < src.cols - 1; j++)
			{

				P1 = src.at<uchar>(i, j);

				if (P1 == 0)
				{
					P2 = src.at<uchar>(i - 1, j);
					P3 = src.at<uchar>(i - 1, j + 1);
					P4 = src.at<uchar>(i, j + 1);
					P5 = src.at<uchar>(i + 1, j + 1);
					P6 = src.at<uchar>(i + 1, j);
					P7 = src.at<uchar>(i + 1, j - 1);
					P8 = src.at<uchar>(i, j - 1);
					P9 = src.at<uchar>(i - 1, j - 1);

					//count A
					if (P2 == 255 && P3 == 0) A++;
					if (P3 == 255 && P4 == 0) A++;
					if (P4 == 255 && P5 == 0) A++;
					if (P5 == 255 && P6 == 0) A++;
					if (P6 == 255 && P7 == 0) A++;
					if (P7 == 255 && P8 == 0) A++;
					if (P8 == 255 && P9 == 0) A++;
					if (P9 == 255 && P2 == 0) A++;

					//count B
					if (P2 == 0) B++;
					if (P3 == 0) B++;
					if (P4 == 0) B++;
					if (P5 == 0) B++;
					if (P6 == 0) B++;
					if (P7 == 0) B++;
					if (P8 == 0) B++;
					if (P9 == 0) B++;

					if (A == 1 && B >= 2 && B <= 6 && (P2 == 255 || P4 == 255 || P6 == 255) && (P4 == 255 || P6 == 255 || P8 == 255)) 
					{
						dst.at<uchar>(i, j) = 255;
						change = true;
					}

					A = 0;
					B = 0;
				}

			}
		}

		// STEP 2
		for (int i = 1; i < src.rows - 1; i++)
		{
			for (int j = 1; j < src.cols - 1; j++)
			{

				P1 = src.at<uchar>(i, j);

				if (P1 == 0)
				{
					P2 = src.at<uchar>(i - 1, j);
					P3 = src.at<uchar>(i - 1, j + 1);
					P4 = src.at<uchar>(i, j + 1);
					P5 = src.at<uchar>(i + 1, j + 1);
					P6 = src.at<uchar>(i + 1, j);
					P7 = src.at<uchar>(i + 1, j - 1);
					P8 = src.at<uchar>(i, j - 1);
					P9 = src.at<uchar>(i - 1, j - 1);

					//count A
					if (P2 == 255 && P3 == 0) A++;
					if (P3 == 255 && P4 == 0) A++;
					if (P4 == 255 && P5 == 0) A++;
					if (P5 == 255 && P6 == 0) A++;
					if (P6 == 255 && P7 == 0) A++;
					if (P7 == 255 && P8 == 0) A++;
					if (P8 == 255 && P9 == 0) A++;
					if (P9 == 255 && P2 == 0) A++;

					//count B
					if (P2 == 0) B++;
					if (P3 == 0) B++;
					if (P4 == 0) B++;
					if (P5 == 0) B++;
					if (P6 == 0) B++;
					if (P7 == 0) B++;
					if (P8 == 0) B++;
					if (P9 == 0) B++;

					if (A == 1 && B >= 2 && B <= 6 && (P2 == 255 || P4 == 255 || P8 == 255) && (P2 == 255 || P6 == 255 || P8 == 255))
					{
						dst.at<uchar>(i, j) = 255;
						change = true;
					}

					A = 0;
					B = 0;
				}

			}
		}

	}

	Negative(dst); 
	
}