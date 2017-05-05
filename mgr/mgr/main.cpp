#include "opencv2/opencv.hpp"
#include "FingerprintsRecognition.h"
#include "Preprocessing.h"
#include "Thinning.h"

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

	Preprocessing PreprocesingObject;

	//THRESHOLD
	Mat image_threshold = image.clone();
	PreprocesingObject.Threshold(image_threshold);

	//MEDIAN FILTER
	Mat image_median = image_threshold.clone();
	PreprocesingObject.Filter(image_median);
	PreprocesingObject.Filter(image_median);
	PreprocesingObject.Filter(image_median);
	PreprocesingObject.Filter(image_median);

	//DILATE	
	Mat image_dilate = image_median.clone();
	PreprocesingObject.Dilate(image_dilate);
	
	imwrite("Data/00111_a.bmp", image_dilate);	//save image after preprocessing


	Thinning ThinningObject;
	Mat image_thinning = imread("Data/00111_a.bmp");

	if (!image_thinning.data)
		return -1;

	Mat bw;
	cvtColor(image_thinning, bw, CV_BGR2GRAY);
	threshold(bw, bw, 10, 255, CV_THRESH_BINARY);

	ThinningObject.Thinning1(bw, bw);
	int a;
	cin >> a;
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}