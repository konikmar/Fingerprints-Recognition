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
															// tak jak mowilem, dziala troche dziwnie, duzo z tym kombinowalem ale jeszcze nie zdiagnozowalem dlaczego nie dziala tak samo jak ten z internetu (moze to byc jakas prosta rzecz, ktorej nie widze)
{
	dst = src.clone();
	Negative(dst);
	dst /= 255;
	int P1, P2, P3, P4, P5, P6, P7, P8, P9;
	int m1, m2;
	int A=0; // number of transitions from 255 to 0 in the sequence P2,P3,P4,P5,P6,P7,P8,P9,P2
	int B=0; // number of black pixel neighbours of P1
	bool change = true;
	cv::Mat prev = cv::Mat::zeros(dst.size(), CV_8UC1);
	cv::Mat diff;
	cv::Mat marker = cv::Mat::zeros(dst.size(), CV_8UC1);
	uchar *pDst;

	do
	{

		change = false;

		// STEP 1
		for (int i = 1; i < src.rows - 1; ++i)
		{
			pDst = marker.ptr<uchar>(i);

			for (int j = 1; j < src.cols - 1; ++j)
			{

				P1 = src.at<uchar>(i, j);

				if (P1 == 1)
				{

					P2 = src.at<uchar>(i - 1, j);
					P3 = src.at<uchar>(i - 1, j + 1);
					P4 = src.at<uchar>(i, j + 1);
					P5 = src.at<uchar>(i + 1, j + 1);
					P6 = src.at<uchar>(i + 1, j);
					P7 = src.at<uchar>(i + 1, j - 1);
					P8 = src.at<uchar>(i, j - 1);
					P9 = src.at<uchar>(i - 1, j - 1);
				

					////count A
					//if (P2 == 0 && P3 == 1) A++;
					//if (P3 == 0 && P4 == 1) A++;
					//if (P4 == 0 && P5 == 1) A++;
					//if (P5 == 0 && P6 == 1) A++;
					//if (P6 == 0 && P7 == 1) A++;
					//if (P7 == 0 && P8 == 1) A++;
					//if (P8 == 0 && P9 == 1) A++;
					//if (P9 == 0 && P2 == 1) A++;

					////count B
					//if (P2 == 1) B++;
					//if (P3 == 1) B++;
					//if (P4 == 1) B++;
					//if (P5 == 1) B++;
					//if (P6 == 1) B++;
					//if (P7 == 1) B++;
					//if (P8 == 1) B++;
					//if (P9 == 1) B++;

					A = (P2 == 0 && P3 == 1) + (P3 == 0 && P4 == 1) +
						(P4 == 0 && P5 == 1) + (P5 == 0 && P6 == 1) +
						(P6 == 0 && P7 == 1) + (P7 == 0 && P8 == 1) +
						(P8 == 0 && P9 == 1) + (P9 == 0 && P2 == 1);
					B = P2 + P3 + P4 + P5 + P6 + P7 + P8 + P9;

					m1 = (P2 * P4 * P6);
					m2 = (P4 * P6 * P8);

					if (A == 1 && (B >= 2 && B <= 6) && m1==0 && m2 == 0)
					{
						//dst.at<uchar>(i, j) = 0;
						pDst[j] = 1;
						change = true;
					}

					A = 0;
					B = 0;
				}

			}
		}

		// STEP 2
		for (int i = 1; i < src.rows - 1; ++i)
		{
			pDst = marker.ptr<uchar>(i);
			for (int j = 1; j < src.cols - 1; ++j)
			{

				P1 = src.at<uchar>(i, j);

				if (P1 == 1)
				{

					P2 = src.at<uchar>(i - 1, j);
					P3 = src.at<uchar>(i - 1, j + 1);
					P4 = src.at<uchar>(i, j + 1);
					P5 = src.at<uchar>(i + 1, j + 1);
					P6 = src.at<uchar>(i + 1, j);
					P7 = src.at<uchar>(i + 1, j - 1);
					P8 = src.at<uchar>(i, j - 1);
					P9 = src.at<uchar>(i - 1, j - 1);


					////count A
					//if (P2 == 0 && P3 == 1) A++;
					//if (P3 == 0 && P4 == 1) A++;
					//if (P4 == 0 && P5 == 1) A++;
					//if (P5 == 0 && P6 == 1) A++;
					//if (P6 == 0 && P7 == 1) A++;
					//if (P7 == 0 && P8 == 1) A++;
					//if (P8 == 0 && P9 == 1) A++;
					//if (P9 == 0 && P2 == 1) A++;

					////count B
					//if (P2 == 1) B++;
					//if (P3 == 1) B++;
					//if (P4 == 1) B++;
					//if (P5 == 1) B++;
					//if (P6 == 1) B++;
					//if (P7 == 1) B++;
					//if (P8 == 1) B++;
					//if (P9 == 1) B++;

					A = (P2 == 0 && P3 == 1) + (P3 == 0 && P4 == 1) +
						(P4 == 0 && P5 == 1) + (P5 == 0 && P6 == 1) +
						(P6 == 0 && P7 == 1) + (P7 == 0 && P8 == 1) +
						(P8 == 0 && P9 == 1) + (P9 == 0 && P2 == 1);
					B = P2 + P3 + P4 + P5 + P6 + P7 + P8 + P9;

					m1 = (P2 * P4 * P8);
					m2 = (P2 * P6 * P8);

					if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
					{
						//dst.at<uchar>(i, j) = 0;
						pDst[j] = 1;
						change = true;
					}

					A = 0;
					B = 0;
				}

			}
		}
		dst &= ~marker;

		cv::absdiff(dst, prev, diff);
		dst.copyTo(prev);

	} while (cv::countNonZero(diff) > 0);

	dst *= 255;
	
}

void Thinning::GuoHallThinning(const Mat& src, Mat& dst) //tez na razie dziala troche dziwnie (i dlugo) - poki co jest na bazie tego co tu: http://answers.opencv.org/question/3207/what-is-a-good-thinning-algorithm-for-getting-the-skeleton-of-characters-for-ocr/
														 //ale widze, ze tu sa niezle rozpisane algorytmy, wiec pozniej sprobuje zrobic na podstawie tego: http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.232.1226&rep=rep1&type=pdf 
{	
	src.copyTo(dst);
	Negative(dst);
	bool change = true;

	int P1, P2, P3, P4, P5, P6, P7, P8, P9;
	
	dst /= 255;

	cv::Mat prev = cv::Mat::zeros(dst.size(), CV_8UC1);
	cv::Mat diff;

	do
	{
		// STEP 1
		for (int i = 1; i < src.rows - 1; i++)
		{
			for (int j = 1; j < src.cols - 1; j++)
			{
				//if (src.at<uchar>(i, j) == 255)
				//{
					P2 = src.at<uchar>(i, j - 1);
					P3 = src.at<uchar>(i + 1, j - 1);
					P4 = src.at<uchar>(i + 1, j);
					P5 = src.at<uchar>(i + 1, j + 1);
					P6 = src.at<uchar>(i, j + 1);
					P7 = src.at<uchar>(i - 1, j + 1);
					P8 = src.at<uchar>(i - 1, j);
					P9 = src.at<uchar>(i - 1, j - 1);

					int C = int(~P8 & (P7 | P6)) +
						int(~P6 & (P5 | P4)) +
						int(~P4 & (P3 | P2)) +
						int(~P2 & (P9 | P8));

				if (C == 1)
				{
					/// calculate N
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
						/// calculate criteria 3
						int c3 = (P8 | P7 | ~P5) & P6;
						if (c3 == 0)
						{
							dst.at<uchar>(i, j) = 0;
							change = true;
						}
					}
				}
				//}
			}
		}

		// STEP 2
		for (int i = 1; i < src.rows - 1; i++)
		{
			for (int j = 1; j < src.cols - 1; j++)
			{
				/*if (src.at<uchar>(i, j) == 255)
				{*/
				/// get 8 neighbors
				/// calculate C(p)

				P2 = src.at<uchar>(i, j - 1);
				P3 = src.at<uchar>(i + 1, j - 1);
				P4 = src.at<uchar>(i + 1, j);
				P5 = src.at<uchar>(i + 1, j + 1);
				P6 = src.at<uchar>(i, j + 1);
				P7 = src.at<uchar>(i - 1, j + 1);
				P8 = src.at<uchar>(i - 1, j);
				P9 = src.at<uchar>(i - 1, j - 1);

				int C = int(~P8 & (P7 | P6)) +
					int(~P6 & (P5 | P4)) +
					int(~P4 & (P3 | P2)) +
					int(~P2 & (P9 | P8));

				if (C == 1)
				{
					/// calculate N
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
							dst.at<uchar>(i, j) = 0;
							change = true;
						}
					}
				}
				//}
			}
		}

		cv::absdiff(dst, prev, diff);
		dst.copyTo(prev);

	} while (cv::countNonZero(diff) > 0);

	dst *= 255;
}