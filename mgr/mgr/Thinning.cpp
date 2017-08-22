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

void Thinning::ZhangSuenThinning(const Mat& src, Mat& dst)
{
	dst = src.clone();
	Negative(dst);
	dst /= 255;
	int P1, P2, P3, P4, P5, P6, P7, P8, P9;
	int m1, m2;
	int A = 0; // number of transitions from 255 to 0 in the sequence P2,P3,P4,P5,P6,P7,P8,P9,P2
	int B = 0; // number of black pixel neighbours of P1
	bool change = true;
	cv::Mat prev = cv::Mat::zeros(dst.size(), CV_8UC1);
	cv::Mat diff;
	cv::Mat marker = cv::Mat::zeros(dst.size(), CV_8UC1);
	uchar *pDst;
	int d = 1;
	while (cv::countNonZero(diff) > 0 || d==1)
	{
		change = false;

		// STEP 1
		for (int i = 1; i < dst.rows - 1; ++i)
		{
			pDst = marker.ptr<uchar>(i);

			for (int j = 1; j < dst.cols - 1; ++j)
			{

				P1 = dst.at<uchar>(i, j);

					P2 = dst.at<uchar>(i - 1, j);
					P3 = dst.at<uchar>(i - 1, j + 1);
					P4 = dst.at<uchar>(i, j + 1);
					P5 = dst.at<uchar>(i + 1, j + 1);
					P6 = dst.at<uchar>(i + 1, j);
					P7 = dst.at<uchar>(i + 1, j - 1);
					P8 = dst.at<uchar>(i, j - 1);
					P9 = dst.at<uchar>(i - 1, j - 1);

					A = (P2 == 0 && P3 == 1) + (P3 == 0 && P4 == 1) +
						(P4 == 0 && P5 == 1) + (P5 == 0 && P6 == 1) +
						(P6 == 0 && P7 == 1) + (P7 == 0 && P8 == 1) +
						(P8 == 0 && P9 == 1) + (P9 == 0 && P2 == 1);
					B = P2 + P3 + P4 + P5 + P6 + P7 + P8 + P9;

					m1 = (P2 * P4 * P6);
					m2 = (P4 * P6 * P8);

					if (A == 1 && (B >= 2 && B <= 6) && m1==0 && m2 == 0)
					{
						pDst[j] = 1;
						change = true;
					}

					A = 0;
					B = 0;
				///}

			}
		}
		dst &= ~marker;

		// STEP 2
		for (int i = 1; i < dst.rows - 1; ++i)
		{
			pDst = marker.ptr<uchar>(i);
			for (int j = 1; j < dst.cols - 1; ++j)
			{

				P1 = dst.at<uchar>(i, j);

					P2 = dst.at<uchar>(i - 1, j);
					P3 = dst.at<uchar>(i - 1, j + 1);
					P4 = dst.at<uchar>(i, j + 1);
					P5 = dst.at<uchar>(i + 1, j + 1);
					P6 = dst.at<uchar>(i + 1, j);
					P7 = dst.at<uchar>(i + 1, j - 1);
					P8 = dst.at<uchar>(i, j - 1);
					P9 = dst.at<uchar>(i - 1, j - 1);


					A = (P2 == 0 && P3 == 1) + (P3 == 0 && P4 == 1) +
						(P4 == 0 && P5 == 1) + (P5 == 0 && P6 == 1) +
						(P6 == 0 && P7 == 1) + (P7 == 0 && P8 == 1) +
						(P8 == 0 && P9 == 1) + (P9 == 0 && P2 == 1);
					B = P2 + P3 + P4 + P5 + P6 + P7 + P8 + P9;

					m1 = (P2 * P4 * P8);
					m2 = (P2 * P6 * P8);

					if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
					{
						pDst[j] = 1;
						change = true;
					}

					A = 0;
					B = 0;
				///}

			}
		}
		dst &= ~marker;

		cv::absdiff(dst, prev, diff);
		if (cv::countNonZero(diff) == 0)
		{
			d = 0;
		}
		dst.copyTo(prev);

	} 

	dst *= 255;
	
}

void Thinning::GuoHallThinning(const Mat& src, Mat& dst)
{
	dst = src.clone();
	Negative(dst);
	dst /= 255;
	int P1, P2, P3, P4, P5, P6, P7, P8, P9;
	int M;
	int C = 0; // number of transitions from 255 to 0 in the sequence P2,P3,P4,P5,P6,P7,P8,P9,P2
	int N = 0; // number of black pixel neighbours of P1
	bool change = true;
	cv::Mat prev = cv::Mat::zeros(dst.size(), CV_8UC1);
	cv::Mat diff;
	cv::Mat marker = cv::Mat::zeros(dst.size(), CV_8UC1);
	uchar *pDst;
	int d = 1;
	while (cv::countNonZero(diff) > 0 || d == 1)
	{
		change = false;

		// STEP 1
		for (int i = 1; i < dst.rows - 1; ++i)
		{
			pDst = marker.ptr<uchar>(i);

			for (int j = 1; j < dst.cols - 1; ++j)
			{

				P1 = dst.at<uchar>(i, j);

				P2 = dst.at<uchar>(i - 1, j);
				P3 = dst.at<uchar>(i - 1, j + 1);
				P4 = dst.at<uchar>(i, j + 1);
				P5 = dst.at<uchar>(i + 1, j + 1);
				P6 = dst.at<uchar>(i + 1, j);
				P7 = dst.at<uchar>(i + 1, j - 1);
				P8 = dst.at<uchar>(i, j - 1);
				P9 = dst.at<uchar>(i - 1, j - 1);

				int C = int(~P8 & (P7 | P6)) +
					int(~P6 & (P5 | P4)) +
					int(~P4 & (P3 | P2)) +
					int(~P2 & (P9 | P8));

				if (C == 1)
				{
					int N1 = int(P9 | P8) +
						int(P7 | P6) +
						int(P5 | P4) +
						int(P3 | P2);
					int N2 = int(P8 | P7) +
						int(P6 | P5) +
						int(P4 | P3) +
						int(P2 | P9);
					int N = min(N1, N2);
					if ((N == 2) || (N == 3))
					{
						int c3 = (P8 | P7 | ~P5) & P6;
						if (c3 == 0)
						{
							pDst[j] = 1;
							change = true;
						}
					}
				}

			}
		}
		dst &= ~marker;

		// STEP 2
		for (int i = 1; i < dst.rows - 1; ++i)
		{
			pDst = marker.ptr<uchar>(i);
			for (int j = 1; j < dst.cols - 1; ++j)
			{

				P1 = dst.at<uchar>(i, j);

				P2 = dst.at<uchar>(i - 1, j);
				P3 = dst.at<uchar>(i - 1, j + 1);
				P4 = dst.at<uchar>(i, j + 1);
				P5 = dst.at<uchar>(i + 1, j + 1);
				P6 = dst.at<uchar>(i + 1, j);
				P7 = dst.at<uchar>(i + 1, j - 1);
				P8 = dst.at<uchar>(i, j - 1);
				P9 = dst.at<uchar>(i - 1, j - 1);
			

				int C = int(~P8 & (P7 | P6)) +
					int(~P6 & (P5 | P4)) +
					int(~P4 & (P3 | P2)) +
					int(~P2 & (P9 | P8));

				if (C == 1)
				{
					int N1 = int(P9 | P8) +
						int(P7 | P6) +
						int(P5 | P4) +
						int(P3 | P2);
					int N2 = int(P8 | P7) +
						int(P6 | P5) +
						int(P4 | P3) +
						int(P2 | P9);
					int N = min(N1, N2);
					if ((N == 2) || (N == 3))
					{
						int E = (P4 | P3 | ~P9) & P2;
						if (E == 0)
						{
							pDst[j] = 1;
							change = true;
						}
					}
				}
			}
		}
		dst &= ~marker;

		cv::absdiff(dst, prev, diff);
		if (cv::countNonZero(diff) == 0)
		{
			d = 0;
		}
		dst.copyTo(prev);

	}

	dst *= 255;
}

void Thinning::LuWangThinning(const Mat& src, Mat& dst)
{
	dst = src.clone();
	Negative(dst);
	dst /= 255;
	int P1, P2, P3, P4, P5, P6, P7, P8, P9;
	int m1, m2;
	int A = 0; // number of transitions from 255 to 0 in the sequence P2,P3,P4,P5,P6,P7,P8,P9,P2
	int B = 0; // number of black pixel neighbours of P1
	bool change = true;
	cv::Mat prev = cv::Mat::zeros(dst.size(), CV_8UC1);
	cv::Mat diff;
	cv::Mat marker = cv::Mat::zeros(dst.size(), CV_8UC1);
	uchar *pDst;
	int d = 1;
	while (cv::countNonZero(diff) > 0 || d == 1)
	{
		change = false;

		// STEP 1
		for (int i = 1; i < dst.rows - 1; ++i)
		{
			pDst = marker.ptr<uchar>(i);

			for (int j = 1; j < dst.cols - 1; ++j)
			{

				P1 = dst.at<uchar>(i, j);

				P2 = dst.at<uchar>(i - 1, j);
				P3 = dst.at<uchar>(i - 1, j + 1);
				P4 = dst.at<uchar>(i, j + 1);
				P5 = dst.at<uchar>(i + 1, j + 1);
				P6 = dst.at<uchar>(i + 1, j);
				P7 = dst.at<uchar>(i + 1, j - 1);
				P8 = dst.at<uchar>(i, j - 1);
				P9 = dst.at<uchar>(i - 1, j - 1);

				A = (P2 == 0 && P3 == 1) + (P3 == 0 && P4 == 1) +
					(P4 == 0 && P5 == 1) + (P5 == 0 && P6 == 1) +
					(P6 == 0 && P7 == 1) + (P7 == 0 && P8 == 1) +
					(P8 == 0 && P9 == 1) + (P9 == 0 && P2 == 1);
				B = P2 + P3 + P4 + P5 + P6 + P7 + P8 + P9;

				m1 = (P2 * P4 * P6);
				m2 = (P4 * P6 * P8);

				if (A == 1 && (B >= 3 && B <= 6) && m1 == 0 && m2 == 0)
				{
					pDst[j] = 1;
					change = true;
				}

				A = 0;
				B = 0;
				///}

			}
		}
		dst &= ~marker;

		// STEP 2
		for (int i = 1; i < dst.rows - 1; ++i)
		{
			pDst = marker.ptr<uchar>(i);
			for (int j = 1; j < dst.cols - 1; ++j)
			{

				P1 = dst.at<uchar>(i, j);

				P2 = dst.at<uchar>(i - 1, j);
				P3 = dst.at<uchar>(i - 1, j + 1);
				P4 = dst.at<uchar>(i, j + 1);
				P5 = dst.at<uchar>(i + 1, j + 1);
				P6 = dst.at<uchar>(i + 1, j);
				P7 = dst.at<uchar>(i + 1, j - 1);
				P8 = dst.at<uchar>(i, j - 1);
				P9 = dst.at<uchar>(i - 1, j - 1);


				A = (P2 == 0 && P3 == 1) + (P3 == 0 && P4 == 1) +
					(P4 == 0 && P5 == 1) + (P5 == 0 && P6 == 1) +
					(P6 == 0 && P7 == 1) + (P7 == 0 && P8 == 1) +
					(P8 == 0 && P9 == 1) + (P9 == 0 && P2 == 1);
				B = P2 + P3 + P4 + P5 + P6 + P7 + P8 + P9;

				m1 = (P2 * P4 * P8);
				m2 = (P2 * P6 * P8);

				if (A == 1 && (B >= 3 && B <= 6) && m1 == 0 && m2 == 0)
				{
					pDst[j] = 1;
					change = true;
				}

				A = 0;
				B = 0;
				///}

			}
		}
		dst &= ~marker;

		cv::absdiff(dst, prev, diff);
		if (cv::countNonZero(diff) == 0)
		{
			d = 0;
		}
		dst.copyTo(prev);

	}

	dst *= 255;

}

void Thinning::KwonWoongKangThinning(const Mat& src, Mat& dst)
{
	dst = src.clone();
	Negative(dst);
	dst /= 255;
	int P1, P2, P3, P4, P5, P6, P7, P8, P9;
	int m1, m2;
	int A = 0; // number of transitions from 255 to 0 in the sequence P2,P3,P4,P5,P6,P7,P8,P9,P2
	int B = 0; // number of black pixel neighbours of P1
	bool change = true;
	cv::Mat prev = cv::Mat::zeros(dst.size(), CV_8UC1);
	cv::Mat diff;
	cv::Mat marker = cv::Mat::zeros(dst.size(), CV_8UC1);
	uchar *pDst;
	int d = 1;

	// PASS 1

	while (cv::countNonZero(diff) > 0 || d == 1)
	{
		change = false;

		// STEP 1
		for (int i = 1; i < dst.rows - 1; ++i)
		{
			pDst = marker.ptr<uchar>(i);

			for (int j = 1; j < dst.cols - 1; ++j)
			{

				P1 = dst.at<uchar>(i, j);

				P2 = dst.at<uchar>(i - 1, j);
				P3 = dst.at<uchar>(i - 1, j + 1);
				P4 = dst.at<uchar>(i, j + 1);
				P5 = dst.at<uchar>(i + 1, j + 1);
				P6 = dst.at<uchar>(i + 1, j);
				P7 = dst.at<uchar>(i + 1, j - 1);
				P8 = dst.at<uchar>(i, j - 1);
				P9 = dst.at<uchar>(i - 1, j - 1);

				A = (P2 == 0 && P3 == 1) + (P3 == 0 && P4 == 1) +
					(P4 == 0 && P5 == 1) + (P5 == 0 && P6 == 1) +
					(P6 == 0 && P7 == 1) + (P7 == 0 && P8 == 1) +
					(P8 == 0 && P9 == 1) + (P9 == 0 && P2 == 1);
				B = P2 + P3 + P4 + P5 + P6 + P7 + P8 + P9;

				m1 = (P2 * P4 * P6);
				m2 = (P4 * P6 * P8);

				if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
				{
					pDst[j] = 1;
					change = true;
				}

				A = 0;
				B = 0;
				///}

			}
		}
		dst &= ~marker;

		// STEP 2
		for (int i = 1; i < dst.rows - 1; ++i)
		{
			pDst = marker.ptr<uchar>(i);
			for (int j = 1; j < dst.cols - 1; ++j)
			{

				P1 = dst.at<uchar>(i, j);

				P2 = dst.at<uchar>(i - 1, j);
				P3 = dst.at<uchar>(i - 1, j + 1);
				P4 = dst.at<uchar>(i, j + 1);
				P5 = dst.at<uchar>(i + 1, j + 1);
				P6 = dst.at<uchar>(i + 1, j);
				P7 = dst.at<uchar>(i + 1, j - 1);
				P8 = dst.at<uchar>(i, j - 1);
				P9 = dst.at<uchar>(i - 1, j - 1);


				A = (P2 == 0 && P3 == 1) + (P3 == 0 && P4 == 1) +
					(P4 == 0 && P5 == 1) + (P5 == 0 && P6 == 1) +
					(P6 == 0 && P7 == 1) + (P7 == 0 && P8 == 1) +
					(P8 == 0 && P9 == 1) + (P9 == 0 && P2 == 1);
				B = P2 + P3 + P4 + P5 + P6 + P7 + P8 + P9;

				m1 = (P2 * P4 * P8);
				m2 = (P2 * P6 * P8);

				if (A == 1 && (B >= 3 && B <= 6) && m1 == 0 && m2 == 0)
				{
					pDst[j] = 1;
					change = true;
				}

				A = 0;
				B = 0;
				///}

			}
		}
		dst &= ~marker;

		cv::absdiff(dst, prev, diff);
		if (cv::countNonZero(diff) == 0)
		{
			d = 0;
		}
		dst.copyTo(prev);

	}

	// PASS 2
	dst *= 255;

	dst /= 255; 
	for (int i = 1; i < dst.rows - 1; ++i)
	{
		pDst = marker.ptr<uchar>(i);

		for (int j = 1; j < dst.cols - 1; ++j)
		{
			P1 = dst.at<uchar>(i, j);

			P2 = dst.at<uchar>(i - 1, j);
			P3 = dst.at<uchar>(i - 1, j + 1);
			P4 = dst.at<uchar>(i, j + 1);
			P5 = dst.at<uchar>(i + 1, j + 1);
			P6 = dst.at<uchar>(i + 1, j);
			P7 = dst.at<uchar>(i + 1, j - 1);
			P8 = dst.at<uchar>(i, j - 1);
			P9 = dst.at<uchar>(i - 1, j - 1);



			if (((P9 == 1) && (P8 == 1) && (P6 == 1) && (P3 == 0)) ||
				((P3 == 1) && (P4 == 1) && (P8 == 1) && (P9 == 0)) ||
				((P1 == 1) && (P6 == 1) && (P8 == 1) && (P3 == 0)) ||
				((P4 == 1) && (P8 == 1) && (P7 == 1) && (P9 == 0)))
			{
				dst.at<uchar>(i, j) = 0;
			}
		}
	}

	dst *= 255;

}

void Thinning::ZhangWangThinning(const Mat& src, Mat& dst)
{
	dst = src.clone();
	Negative(dst);
	dst /= 255;
	int P1, P2, P3, P4, P5, P6, P7, P8, P9, P11, P15;
	int m1, m2;
	int A = 0; // number of transitions from 255 to 0 in the sequence P2,P3,P4,P5,P6,P7,P8,P9,P2
	int B = 0; // number of black pixel neighbours of P1
	bool change = true;
	cv::Mat prev = cv::Mat::zeros(dst.size(), CV_8UC1);
	cv::Mat diff;
	cv::Mat marker = cv::Mat::zeros(dst.size(), CV_8UC1);
	uchar *pDst;
	int d = 1;
	while (cv::countNonZero(diff) > 0 || d == 1)
	{
		change = false;

		// STEP 1
		for (int i = 2; i < dst.rows - 2; ++i)
		{
			pDst = marker.ptr<uchar>(i);

			for (int j = 2; j < dst.cols - 2; ++j)
			{

				P1 = dst.at<uchar>(i, j);

				P2 = dst.at<uchar>(i - 1, j);
				P3 = dst.at<uchar>(i - 1, j + 1);
				P4 = dst.at<uchar>(i, j + 1);
				P5 = dst.at<uchar>(i + 1, j + 1);
				P6 = dst.at<uchar>(i + 1, j);
				P7 = dst.at<uchar>(i + 1, j - 1);
				P8 = dst.at<uchar>(i, j - 1);
				P9 = dst.at<uchar>(i - 1, j - 1);

				P11 = dst.at<uchar>(i - 2, j);
				P15 = dst.at<uchar>(i, j + 2);

				A = (P2 == 0 && P3 == 1) + (P3 == 0 && P4 == 1) +
					(P4 == 0 && P5 == 1) + (P5 == 0 && P6 == 1) +
					(P6 == 0 && P7 == 1) + (P7 == 0 && P8 == 1) +
					(P8 == 0 && P9 == 1) + (P9 == 0 && P2 == 1);
				B = P2 + P3 + P4 + P5 + P6 + P7 + P8 + P9;

				m1 = (P2 * P4 * P8);
				m2 = (P2 * P4 * P6);

				if (A == 1 && (B >= 2 && B <= 6) && ((m1 == 0) || (P11 == 1)) && ((m2 == 0) || (P15 == 1)))
				{
					pDst[j] = 1;
					change = true;
				}

				A = 0;
				B = 0;
				///}

			}
		}
		dst &= ~marker;

		cv::absdiff(dst, prev, diff);
		if (cv::countNonZero(diff) == 0)
		{
			d = 0;
		}
		dst.copyTo(prev);

	}

	dst *= 255;

}

void Thinning::HilditchThinning(const Mat& src, Mat& dst)
{
	dst = src.clone();
	Negative(dst);
	dst /= 255;
	int P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P14, P15, P16;
	int m1, m2;
	int A = 0; // number of transitions from 0 to 255 in the sequence P2,P3,P4,P5,P6,P7,P8,P9,P2
	int B = 0; // number of black pixel neighbours of P1
	int AP2 = 0; // number of transitions from 0 to 255 in the sequence P11,P12,P3,P4,P1,P8,P9,P10,P11
	int AP4 = 0; // number of transitions from 0 to 255 in the sequence P3,P14,P15,P16,P5,P6,P1,P2,P3
	bool change = true;
	cv::Mat prev = cv::Mat::zeros(dst.size(), CV_8UC1);
	cv::Mat diff;
	cv::Mat marker = cv::Mat::zeros(dst.size(), CV_8UC1);
	uchar *pDst;
	int d = 1;
	while (cv::countNonZero(diff) > 0 || d == 1)
	{
		change = false;

		// STEP 1
		for (int i = 2; i < dst.rows - 2; ++i)
		{
			pDst = marker.ptr<uchar>(i);

			for (int j = 2; j < dst.cols - 2; ++j)
			{

				P1 = dst.at<uchar>(i, j);

				P2 = dst.at<uchar>(i - 1, j);
				P3 = dst.at<uchar>(i - 1, j + 1);
				P4 = dst.at<uchar>(i, j + 1);
				P5 = dst.at<uchar>(i + 1, j + 1);
				P6 = dst.at<uchar>(i + 1, j);
				P7 = dst.at<uchar>(i + 1, j - 1);
				P8 = dst.at<uchar>(i, j - 1);
				P9 = dst.at<uchar>(i - 1, j - 1);

				P10 = dst.at<uchar>(i - 2, j - 1);
				P11 = dst.at<uchar>(i - 2, j);
				P12 = dst.at<uchar>(i - 2, j + 1);

				P14 = dst.at<uchar>(i - 1, j + 2);
				P15 = dst.at<uchar>(i, j + 2);
				P16 = dst.at<uchar>(i + 1, j + 2);

				A = (P2 == 0 && P3 == 1) + (P3 == 0 && P4 == 1) +
					(P4 == 0 && P5 == 1) + (P5 == 0 && P6 == 1) +
					(P6 == 0 && P7 == 1) + (P7 == 0 && P8 == 1) +
					(P8 == 0 && P9 == 1) + (P9 == 0 && P2 == 1);

				AP2 = (P11 == 0 && P12 == 1) + (P12 == 0 && P3 == 1) +
					(P3 == 0 && P4 == 1) + (P4 == 0 && P1 == 1) +
					(P1 == 0 && P8 == 1) + (P8 == 0 && P9 == 1) +
					(P9 == 0 && P10 == 1) + (P10 == 0 && P11 == 1);

				AP2 = (P3 == 0 && P14 == 1) + (P14 == 0 && P15 == 1) +
					(P15 == 0 && P16 == 1) + (P16 == 0 && P5 == 1) +
					(P5 == 0 && P6 == 1) + (P6 == 0 && P1 == 1) +
					(P1 == 0 && P2 == 1) + (P2 == 0 && P3 == 1);
			
				B = P2 + P3 + P4 + P5 + P6 + P7 + P8 + P9;

				m1 = (P2 * P4 * P8);
				m2 = (P2 * P4 * P6);

				if (A == 1 && (B >= 2 && B <= 6) && ((m1 == 0) || (AP2 != 1)) && ((m2 == 0) || (AP4 != 1)))
				{
					pDst[j] = 1;
					change = true;
				}

				A = 0;
				B = 0;
				///}

			}
		}
		dst &= ~marker;

		cv::absdiff(dst, prev, diff);
		if (cv::countNonZero(diff) == 0)
		{
			d = 0;
		}
		dst.copyTo(prev);

	}

	dst *= 255;

}


void Thinning::ArabicParallelThinning(const Mat& src, Mat& dst)
{
	dst = src.clone();
	Negative(dst);
	dst /= 255;
	int P1, P2, P3, P4, P5, P6, P7, P8, P9;
	int m1, m2;
	int A = 0; // number of transitions from 255 to 0 in the sequence P2,P3,P4,P5,P6,P7,P8,P9,P2
	int B = 0; // number of black pixel neighbours of P1
	bool change = true;
	cv::Mat prev = cv::Mat::zeros(dst.size(), CV_8UC1);
	cv::Mat diff;
	cv::Mat marker = cv::Mat::zeros(dst.size(), CV_8UC1);
	uchar *pDst;
	int d = 1;
	while (cv::countNonZero(diff) > 0 || d == 1)
	{
		change = false;

		// STEP 1
		for (int i = 1; i < dst.rows - 1; ++i)
		{
			pDst = marker.ptr<uchar>(i);

			for (int j = 1; j < dst.cols - 1; ++j)
			{

				P1 = dst.at<uchar>(i, j);

				P2 = dst.at<uchar>(i - 1, j);
				P3 = dst.at<uchar>(i - 1, j + 1);
				P4 = dst.at<uchar>(i, j + 1);
				P5 = dst.at<uchar>(i + 1, j + 1);
				P6 = dst.at<uchar>(i + 1, j);
				P7 = dst.at<uchar>(i + 1, j - 1);
				P8 = dst.at<uchar>(i, j - 1);
				P9 = dst.at<uchar>(i - 1, j - 1);

				A = (P2 == 0 && P3 == 1) + (P3 == 0 && P4 == 1) +
					(P4 == 0 && P5 == 1) + (P5 == 0 && P6 == 1) +
					(P6 == 0 && P7 == 1) + (P7 == 0 && P8 == 1) +
					(P8 == 0 && P9 == 1) + (P9 == 0 && P2 == 1);
				B = P2 + P3 + P4 + P5 + P6 + P7 + P8 + P9;

				m1 = (P2 * P4 * P6);
				m2 = (P4 * P6 * P8);

				if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
				{
					pDst[j] = 1;
					change = true;
				}

				A = 0;
				B = 0;
				///}

			}
		}
		dst &= ~marker;

		// STEP 2
		for (int i = 1; i < dst.rows - 1; ++i)
		{
			pDst = marker.ptr<uchar>(i);
			for (int j = 1; j < dst.cols - 1; ++j)
			{

				P1 = dst.at<uchar>(i, j);

				P2 = dst.at<uchar>(i - 1, j);
				P3 = dst.at<uchar>(i - 1, j + 1);
				P4 = dst.at<uchar>(i, j + 1);
				P5 = dst.at<uchar>(i + 1, j + 1);
				P6 = dst.at<uchar>(i + 1, j);
				P7 = dst.at<uchar>(i + 1, j - 1);
				P8 = dst.at<uchar>(i, j - 1);
				P9 = dst.at<uchar>(i - 1, j - 1);


				A = (P2 == 0 && P3 == 1) + (P3 == 0 && P4 == 1) +
					(P4 == 0 && P5 == 1) + (P5 == 0 && P6 == 1) +
					(P6 == 0 && P7 == 1) + (P7 == 0 && P8 == 1) +
					(P8 == 0 && P9 == 1) + (P9 == 0 && P2 == 1);
				B = P2 + P3 + P4 + P5 + P6 + P7 + P8 + P9;

				m1 = (P2 * P6 * P8);
				m2 = (P4 * P6 * P8);

				if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
				{
					pDst[j] = 1;
					change = true;
				}

				A = 0;
				B = 0;
				///}

			}
		}
		dst &= ~marker;

		// STEP 3
		for (int i = 1; i < dst.rows - 1; ++i)
		{
			pDst = marker.ptr<uchar>(i);

			for (int j = 1; j < dst.cols - 1; ++j)
			{

				P1 = dst.at<uchar>(i, j);

				P2 = dst.at<uchar>(i - 1, j);
				P3 = dst.at<uchar>(i - 1, j + 1);
				P4 = dst.at<uchar>(i, j + 1);
				P5 = dst.at<uchar>(i + 1, j + 1);
				P6 = dst.at<uchar>(i + 1, j);
				P7 = dst.at<uchar>(i + 1, j - 1);
				P8 = dst.at<uchar>(i, j - 1);
				P9 = dst.at<uchar>(i - 1, j - 1);

				A = (P2 == 0 && P3 == 1) + (P3 == 0 && P4 == 1) +
					(P4 == 0 && P5 == 1) + (P5 == 0 && P6 == 1) +
					(P6 == 0 && P7 == 1) + (P7 == 0 && P8 == 1) +
					(P8 == 0 && P9 == 1) + (P9 == 0 && P2 == 1);
				B = P2 + P3 + P4 + P5 + P6 + P7 + P8 + P9;

				m1 = (P2 * P4 * P8);
				m2 = (P2 * P6 * P8);

				if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
				{
					pDst[j] = 1;
					change = true;
				}

				A = 0;
				B = 0;
				///}

			}
		}
		dst &= ~marker;

		// STEP 4
		for (int i = 1; i < dst.rows - 1; ++i)
		{
			pDst = marker.ptr<uchar>(i);
			for (int j = 1; j < dst.cols - 1; ++j)
			{

				P1 = dst.at<uchar>(i, j);

				P2 = dst.at<uchar>(i - 1, j);
				P3 = dst.at<uchar>(i - 1, j + 1);
				P4 = dst.at<uchar>(i, j + 1);
				P5 = dst.at<uchar>(i + 1, j + 1);
				P6 = dst.at<uchar>(i + 1, j);
				P7 = dst.at<uchar>(i + 1, j - 1);
				P8 = dst.at<uchar>(i, j - 1);
				P9 = dst.at<uchar>(i - 1, j - 1);


				A = (P2 == 0 && P3 == 1) + (P3 == 0 && P4 == 1) +
					(P4 == 0 && P5 == 1) + (P5 == 0 && P6 == 1) +
					(P6 == 0 && P7 == 1) + (P7 == 0 && P8 == 1) +
					(P8 == 0 && P9 == 1) + (P9 == 0 && P2 == 1);
				B = P2 + P3 + P4 + P5 + P6 + P7 + P8 + P9;

				m1 = (P2 * P4 * P6);
				m2 = (P2 * P4 * P8);

				if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
				{
					pDst[j] = 1;
					change = true;
				}

				A = 0;
				B = 0;
				///}

			}
		}
		dst &= ~marker;

		cv::absdiff(dst, prev, diff);
		if (cv::countNonZero(diff) == 0)
		{
			d = 0;
		}
		dst.copyTo(prev);

	}

	dst *= 255;

}


void Thinning::EfficientParallelThinning(const Mat& src, Mat& dst)
{
	dst = src.clone();
	Negative(dst);
	dst /= 255;
	int P1, P2, P3, P4, P5, P6, P7, P8, P9;
	bool change = true;
	cv::Mat prev = cv::Mat::zeros(dst.size(), CV_8UC1);
	cv::Mat diff;
	cv::Mat marker = cv::Mat::zeros(dst.size(), CV_8UC1);
	uchar *pDst;
	int d = 1;
	while (cv::countNonZero(diff) > 0 || d == 1)
	{
		change = false;

		// STEP 1
		for (int i = 1; i < dst.rows - 1; ++i)
		{
			pDst = marker.ptr<uchar>(i);

			for (int j = 1; j < dst.cols - 1; ++j)
			{

				P1 = dst.at<uchar>(i, j);

				P2 = dst.at<uchar>(i - 1, j);
				P3 = dst.at<uchar>(i - 1, j + 1);
				P4 = dst.at<uchar>(i, j + 1);
				P5 = dst.at<uchar>(i + 1, j + 1);
				P6 = dst.at<uchar>(i + 1, j);
				P7 = dst.at<uchar>(i + 1, j - 1);
				P8 = dst.at<uchar>(i, j - 1);
				P9 = dst.at<uchar>(i - 1, j - 1);

				if ((!P4 && P8 && ((P2 && !P5 && !P6) || (!P2 && !P3 && P6))) == 1 ||
					(P6 && P7 && P8 && ((P9 && P2 && !P4) || (!P2 && P4 && P5))) == 1||
					(!P2 && !P3 && !P4 && (((!P5 && !P6 && P8) && (P9 || P7)) || ((!P9 && P6 && !P8) && (P7 || P5)))) == 1 )
				{
					pDst[j] = 1;
					change = true;
				}

			}
		}
		dst &= ~marker;

		// STEP 2
		for (int i = 1; i < dst.rows - 1; ++i)
		{
			pDst = marker.ptr<uchar>(i);
			for (int j = 1; j < dst.cols - 1; ++j)
			{

				P1 = dst.at<uchar>(i, j);

				P2 = dst.at<uchar>(i - 1, j);
				P3 = dst.at<uchar>(i - 1, j + 1);
				P4 = dst.at<uchar>(i, j + 1);
				P5 = dst.at<uchar>(i + 1, j + 1);
				P6 = dst.at<uchar>(i + 1, j);
				P7 = dst.at<uchar>(i + 1, j - 1);
				P8 = dst.at<uchar>(i, j - 1);
				P9 = dst.at<uchar>(i - 1, j - 1);

				if ((P4 && !P8 && ((!P9 && !P2 && P6) || (P2 && !P6 && !P7))) == 1 ||
					(P2 && P3 && P4 && ((P5 && P6 && !P8) || (P9 && !P6 && P8))) == 1 ||
					(!P6 && !P7 && !P8 && (((!P9 && !P2 && P4) && (P5 || P3)) || ((P2 && !P4 && !P5) && (P3 || P9)))) == 1)
				{
					pDst[j] = 1;
					change = true;
				}

			}
		}
		dst &= ~marker;

		cv::absdiff(dst, prev, diff);
		if (cv::countNonZero(diff) == 0)
		{
			d = 0;
		}
		dst.copyTo(prev);

	}

	dst *= 255;

}

void Thinning::ImprovedArabicParallelThinning(const Mat& src, Mat& dst)
{
	//PASS 1
	dst = src.clone();
	Negative(dst);
	dst /= 255;
	int P1, P2, P3, P4, P5, P6, P7, P8, P9;
	int m1, m2;
	int A = 0; // number of transitions from 255 to 0 in the sequence P2,P3,P4,P5,P6,P7,P8,P9,P2
	int B = 0; // number of black pixel neighbours of P1
	bool change = true;
	cv::Mat prev = cv::Mat::zeros(dst.size(), CV_8UC1);
	cv::Mat diff;
	cv::Mat marker = cv::Mat::zeros(dst.size(), CV_8UC1);
	uchar *pDst;
	int d = 1;
	while (cv::countNonZero(diff) > 0 || d == 1)
	{
		change = false;

		// STEP 1
		for (int i = 1; i < dst.rows - 1; ++i)
		{
			pDst = marker.ptr<uchar>(i);

			for (int j = 1; j < dst.cols - 1; ++j)
			{

				P1 = dst.at<uchar>(i, j);

				P2 = dst.at<uchar>(i - 1, j);
				P3 = dst.at<uchar>(i - 1, j + 1);
				P4 = dst.at<uchar>(i, j + 1);
				P5 = dst.at<uchar>(i + 1, j + 1);
				P6 = dst.at<uchar>(i + 1, j);
				P7 = dst.at<uchar>(i + 1, j - 1);
				P8 = dst.at<uchar>(i, j - 1);
				P9 = dst.at<uchar>(i - 1, j - 1);

				A = (P2 == 0 && P3 == 1) + (P3 == 0 && P4 == 1) +
					(P4 == 0 && P5 == 1) + (P5 == 0 && P6 == 1) +
					(P6 == 0 && P7 == 1) + (P7 == 0 && P8 == 1) +
					(P8 == 0 && P9 == 1) + (P9 == 0 && P2 == 1);
				B = P2 + P3 + P4 + P5 + P6 + P7 + P8 + P9;

				m1 = (P2 * P4 * P6);
				m2 = (P4 * P6 * P8);

				if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
				{
					pDst[j] = 1;
					change = true;
				}

				A = 0;
				B = 0;
				///}

			}
		}
		dst &= ~marker;

		// STEP 2
		for (int i = 1; i < dst.rows - 1; ++i)
		{
			pDst = marker.ptr<uchar>(i);
			for (int j = 1; j < dst.cols - 1; ++j)
			{

				P1 = dst.at<uchar>(i, j);

				P2 = dst.at<uchar>(i - 1, j);
				P3 = dst.at<uchar>(i - 1, j + 1);
				P4 = dst.at<uchar>(i, j + 1);
				P5 = dst.at<uchar>(i + 1, j + 1);
				P6 = dst.at<uchar>(i + 1, j);
				P7 = dst.at<uchar>(i + 1, j - 1);
				P8 = dst.at<uchar>(i, j - 1);
				P9 = dst.at<uchar>(i - 1, j - 1);


				A = (P2 == 0 && P3 == 1) + (P3 == 0 && P4 == 1) +
					(P4 == 0 && P5 == 1) + (P5 == 0 && P6 == 1) +
					(P6 == 0 && P7 == 1) + (P7 == 0 && P8 == 1) +
					(P8 == 0 && P9 == 1) + (P9 == 0 && P2 == 1);
				B = P2 + P3 + P4 + P5 + P6 + P7 + P8 + P9;

				m1 = (P2 * P6 * P8);
				m2 = (P4 * P6 * P8);

				if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
				{
					pDst[j] = 1;
					change = true;
				}

				A = 0;
				B = 0;
				///}

			}
		}
		dst &= ~marker;

		// STEP 3
		for (int i = 1; i < dst.rows - 1; ++i)
		{
			pDst = marker.ptr<uchar>(i);

			for (int j = 1; j < dst.cols - 1; ++j)
			{

				P1 = dst.at<uchar>(i, j);

				P2 = dst.at<uchar>(i - 1, j);
				P3 = dst.at<uchar>(i - 1, j + 1);
				P4 = dst.at<uchar>(i, j + 1);
				P5 = dst.at<uchar>(i + 1, j + 1);
				P6 = dst.at<uchar>(i + 1, j);
				P7 = dst.at<uchar>(i + 1, j - 1);
				P8 = dst.at<uchar>(i, j - 1);
				P9 = dst.at<uchar>(i - 1, j - 1);

				A = (P2 == 0 && P3 == 1) + (P3 == 0 && P4 == 1) +
					(P4 == 0 && P5 == 1) + (P5 == 0 && P6 == 1) +
					(P6 == 0 && P7 == 1) + (P7 == 0 && P8 == 1) +
					(P8 == 0 && P9 == 1) + (P9 == 0 && P2 == 1);
				B = P2 + P3 + P4 + P5 + P6 + P7 + P8 + P9;

				m1 = (P2 * P4 * P8);
				m2 = (P2 * P6 * P8);

				if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
				{
					pDst[j] = 1;
					change = true;
				}

				A = 0;
				B = 0;
				///}

			}
		}
		dst &= ~marker;

		// STEP 4
		for (int i = 1; i < dst.rows - 1; ++i)
		{
			pDst = marker.ptr<uchar>(i);
			for (int j = 1; j < dst.cols - 1; ++j)
			{

				P1 = dst.at<uchar>(i, j);

				P2 = dst.at<uchar>(i - 1, j);
				P3 = dst.at<uchar>(i - 1, j + 1);
				P4 = dst.at<uchar>(i, j + 1);
				P5 = dst.at<uchar>(i + 1, j + 1);
				P6 = dst.at<uchar>(i + 1, j);
				P7 = dst.at<uchar>(i + 1, j - 1);
				P8 = dst.at<uchar>(i, j - 1);
				P9 = dst.at<uchar>(i - 1, j - 1);


				A = (P2 == 0 && P3 == 1) + (P3 == 0 && P4 == 1) +
					(P4 == 0 && P5 == 1) + (P5 == 0 && P6 == 1) +
					(P6 == 0 && P7 == 1) + (P7 == 0 && P8 == 1) +
					(P8 == 0 && P9 == 1) + (P9 == 0 && P2 == 1);
				B = P2 + P3 + P4 + P5 + P6 + P7 + P8 + P9;

				m1 = (P2 * P4 * P6);
				m2 = (P2 * P4 * P8);

				if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
				{
					pDst[j] = 1;
					change = true;
				}

				A = 0;
				B = 0;
				///}

			}
		}
		dst &= ~marker;

		cv::absdiff(dst, prev, diff);
		if (cv::countNonZero(diff) == 0)
		{
			d = 0;
		}
		dst.copyTo(prev);

	}

	// PASS 2
	dst *= 255;

	dst /= 255;
	for (int i = 1; i < dst.rows - 1; ++i)
	{
		pDst = marker.ptr<uchar>(i);

		for (int j = 1; j < dst.cols - 1; ++j)
		{
			P1 = dst.at<uchar>(i, j);

			P2 = dst.at<uchar>(i - 1, j);
			P3 = dst.at<uchar>(i - 1, j + 1);
			P4 = dst.at<uchar>(i, j + 1);
			P5 = dst.at<uchar>(i + 1, j + 1);
			P6 = dst.at<uchar>(i + 1, j);
			P7 = dst.at<uchar>(i + 1, j - 1);
			P8 = dst.at<uchar>(i, j - 1);
			P9 = dst.at<uchar>(i - 1, j - 1);



			if (P1 == 1)
			{
				if (((P2*P4 == 1) && (P7 == 0)) ||
					((P4*P6 == 1) && (P9 == 0)) ||
					((P6*P8 == 1) && (P3 == 0)) ||
					((P8*P2 == 1) && (P5 == 0)))
				{
					dst.at<uchar>(i, j) = 0;
				}
			}
		}
	}

	dst *= 255;

}