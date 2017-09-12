#include "Preprocessing.h"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

// overloaded constructor
Preprocessing::Preprocessing(){
}

void Preprocessing::Windowing(Mat& image){
	std::vector<int> listaX;
	std::vector<int> listaY;
	int index = 0;
	for (int i = 1; i < image.rows - 1; ++i)
	{
		for (int j = 1; j < image.cols - 1; ++j)
		{
			if (image.at<uchar>(i, j) < 200)
			{
				listaX.push_back(j);
				listaY.push_back(i);
			}
		}
	}
	int Xmin = *min_element(listaX.begin(), listaX.end());
	int Xmax = *max_element(listaX.begin(), listaX.end());
	int Ymin = *min_element(listaY.begin(), listaY.end());
	int Ymax = *max_element(listaY.begin(), listaY.end());

	image = image(Rect(Xmin, Ymin, (Xmax-Xmin), (Ymax-Ymin)));
}

void Preprocessing::Filter(Mat& image){
	medianBlur(image, image, 7);
}

void Preprocessing::Threshold(Mat& image){

	threshold(image, image, 120, 255, 0);
}

void Preprocessing::Erode(Mat& image){
	Mat element = getStructuringElement(MORPH_ELLIPSE,
		Size(2 * 3 + 1, 2 * 3 + 1),
		Point(3, 3));
	/// Apply the erosion operation
	erode(image, image, element);
}

void Preprocessing::Dilate(Mat& image){
	Mat element = getStructuringElement(MORPH_ELLIPSE,
		Size(2 * 5 + 1, 2 * 5 + 1),
		Point(3, 3));
	/// Apply the erosion operation
	dilate(image, image, element);
}

void Preprocessing::FilterPores(Mat& image){
	//mask declaration
	int P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15, P16;
	image /= 255;
	for (int i = 3; i < image.rows - 3; ++i)
	{
		for (int j = 3; j < image.cols - 3; ++j)
		{
			//edge mask
			P1 = image.at<uchar>(i - 3, j - 1);
			P2 = image.at<uchar>(i - 3, j);
			P3 = image.at<uchar>(i - 3, j + 1);
			P4 = image.at<uchar>(i - 2, j + 2);
			P5 = image.at<uchar>(i - 1, j + 3);
			P6 = image.at<uchar>(i, j + 3);
			P7 = image.at<uchar>(i + 1, j + 3);
			P8 = image.at<uchar>(i + 2, j + 2);
			P9 = image.at<uchar>(i + 3, j + 1);
			P10 = image.at<uchar>(i + 3, j);
			P11 = image.at<uchar>(i + 3, j - 1);
			P12 = image.at<uchar>(i + 2, j - 2);
			P13 = image.at<uchar>(i + 1, j - 3);
			P14 = image.at<uchar>(i, j - 3);
			P15 = image.at<uchar>(i - 1, j - 3);
			P16 = image.at<uchar>(i - 2, j - 2);
			P0 = image.at<uchar>(i, j);
			int suma = P1 + P2 + P3 + P4 + P5 + P6 + P7 + P8 + P9 + P10 + P11 + P12 + P13 + P14 + P15 + P16;
			if (P0 == 1 && suma == 0){
				//fill interior mask
				image.at<uchar>(i - 2, j - 1) = 0;
				image.at<uchar>(i - 2, j) = 0;
				image.at<uchar>(i - 2, j + 1) = 0;
				image.at<uchar>(i - 1, j + 2) = 0;
				image.at<uchar>(i, j + 2) = 0;
				image.at<uchar>(i + 1, j + 2) = 0;
				image.at<uchar>(i + 2, j + 1) = 0;
				image.at<uchar>(i + 2, j) = 0;
				image.at<uchar>(i + 2, j - 1) = 0;
				image.at<uchar>(i + 1, j - 2) = 0;
				image.at<uchar>(i, j - 2) = 0;
				image.at<uchar>(i - 1, j - 2) = 0;
				image.at<uchar>(i - 1, j - 1) = 0;
				image.at<uchar>(i - 1, j) = 0;
				image.at<uchar>(i - 1, j + 1) = 0;
				image.at<uchar>(i, j + 1) = 0;
				image.at<uchar>(i + 1, j + 1) = 0;
				image.at<uchar>(i + 1, j) = 0;
				image.at<uchar>(i + 1, j - 1) = 0;
				image.at<uchar>(i, j - 1) = 0;
				image.at<uchar>(i, j) = 0;
			}
		}
	}
	image *= 255;
	
}

Preprocessing::~Preprocessing(){
}
