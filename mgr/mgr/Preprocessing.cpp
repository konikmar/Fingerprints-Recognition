#include "Preprocessing.h"
#include "opencv2/opencv.hpp"

using namespace cv;

// overloaded constructor
Preprocessing::Preprocessing(){
}

void Preprocessing::Filter(Mat& image){
	medianBlur(image, image, 7);
}

void Preprocessing::Threshold(Mat& image){

	threshold(image, image, 120, 255, 0);
}

void Preprocessing::Dilate(Mat& image){
	Mat element = getStructuringElement(MORPH_ELLIPSE,
		Size(2 * 3 + 1, 2 * 3 + 1),
		Point(3, 3));
	/// Apply the erosion operation
	dilate(image, image, element);
}
void Preprocessing::Normalize(Mat& image){
}

Preprocessing::~Preprocessing(){
}
