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

void Preprocessing::Normalize(Mat& image){
}

Preprocessing::~Preprocessing(){
}
