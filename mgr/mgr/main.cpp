#include "opencv2/opencv.hpp"
#include "FingerprintsRecognition.h"
#include "Preprocessing.h"

using namespace cv;
using namespace std;

int main(int, char)
{
	Mat image;
	image = imread("Data/00111.bmp", 1);
	Size size(6 * image.cols, 6 * image.rows);
	resize(image, image, size);
	//namedWindow("okno", 1);
	//imshow("okno", image);

	Preprocessing object;

	//THRESHOLD
	Mat image_threshold = image.clone();
	object.Threshold(image_threshold);

	//MEDIAN FILTER
	Mat image_median = image_threshold.clone();
	object.Filter(image_median);
	object.Filter(image_median);
	object.Filter(image_median);
	object.Filter(image_median);

	//DILATE	
	Mat image_dilate = image_median.clone();
	object.Dilate(image_dilate);
	
	int a;
	cin >> a;
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}