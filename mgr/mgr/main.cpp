#include "opencv2/opencv.hpp"
#include "FingerprintsRecognition.h"
#include "Preprocessing.h"
#include "Thinning.h"
#include "GaborFilter.h"
#include "Detection.h"
#include "FalseMinutiae.h"

#include <iostream>
#include <string>
#include <math.h>
#include <time.h>


using namespace cv;
using namespace std;

int main(int, char)
{
	///odicks wzorcowy
	Mat image;
	image = imread("Data/013_4_2.bmp", 1);
	Size size(3 * image.cols, 3 * image.rows); //wczesniej bylo 6
	resize(image, image, size);
	//namedWindow("okno", 1);
	//imshow("okno", image);

	cvtColor(image, image, CV_BGR2GRAY);

	///////////////////////////////////////////////////////////////
	//////////////////////////////////
	//odcisk 1
	Mat image1;
	image1 = imread("Data/013_4_4.bmp", 1);
	resize(image1, image1, size);
	cvtColor(image1, image1, CV_BGR2GRAY);
	/////////////////////////////////
	////PREPROCESSING
	Preprocessing PreprocesingObject;
	//WINDOWING wzorcowy
	Mat image_windowing = image.clone();
	PreprocesingObject.Windowing(image_windowing);

	//Windowing odiskc 1
	Mat image_windowing1 = image1.clone();
	PreprocesingObject.Windowing(image_windowing1);

	//MEDIAN FILTER odcisk wzorcowy
	Mat image_median = image_windowing.clone();
	PreprocesingObject.Filter(image_median);
	PreprocesingObject.Filter(image_median);
	PreprocesingObject.Filter(image_median);

	//MEDIAN FILTER odcisk 1
	Mat image_median1 = image_windowing1.clone();
	PreprocesingObject.Filter(image_median1);
	PreprocesingObject.Filter(image_median1);
	PreprocesingObject.Filter(image_median1);

	////THRESHOLD odcisk wzorcowy
	Mat image_threshold = image_median.clone();
	adaptiveThreshold(image_threshold, image_threshold, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 75, 1);

	//THRESHOLD odcisk 1
	Mat image_threshold1 = image_median1.clone();
	adaptiveThreshold(image_threshold1, image_threshold1, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 75, 1);

	////DILATE odcisk wzorcowy
	Mat image_dilate = image_threshold.clone();

	///DILATE odcisk 1
	Mat image_dilate1 = image_threshold1.clone();
	//////////////////////////////////////////////////////////////
	
	//RESIZE odcisk wzorcowy
	Size size1( image_dilate.cols/2,  image_dilate.rows/2); //bylo /3
	resize(image_dilate, image_dilate, size1);
	

	//RESIZE odcisk 1
	resize(image_dilate1, image_dilate1, size1);

	/////////////////////////////////////////////////////
	imwrite("Data/Program/Preprocessing.bmp", image_dilate); //save image after preprocessing - obraz wzorcowy
	
	imwrite("Data/Program/Preprocessing1.bmp", image_dilate1);	//save image after preprocessing - obraz 1

	//THINNING
	Thinning ThinningObject;
	
	Mat image_thinning = imread("Data/Program/Preprocessing.bmp");
	Mat image_thinning1 = imread("Data/Program/Preprocessing1.bmp");
	//Mat image_thinning = imread("Data/00111_a.bmp");
	//Size size3(0.2 * image_thinning.cols, 0.2 * image_thinning.rows);
	//resize(image_thinning, image_thinning, size3);

	if (!image_thinning.data)
		return -1;
	if (!image_thinning1.data)
		return -1;
	
	
	cvtColor(image_thinning, image_thinning, CV_BGR2GRAY);
	threshold(image_thinning, image_thinning, 10, 255, CV_THRESH_BINARY);
	Mat image_thinning_negative = image_thinning.clone();

	cvtColor(image_thinning1, image_thinning1, CV_BGR2GRAY);
	threshold(image_thinning1, image_thinning1, 10, 255, CV_THRESH_BINARY);
	Mat image_thinning_negative1 = image_thinning1.clone();

	//ThinningObject.Thinning1(image_thinning, image_thinning);
	//ThinningObject.ThinningNegative1(image_thinning_negative, image_thinning_negative);
	//ThinningObject.ZhangSuenThinning(image_thinning, image_thinning);
	//ThinningObject.GuoHallThinning(image_thinning, image_thinning);
	//ThinningObject.LuWangThinning(image_thinning, image_thinning);
//	cvtColor(test, test, CV_BGR2GRAY);
//	threshold(test, test,50, 255, CV_THRESH_BINARY);
	//ThinningObject.LuWangThinning(test, test);
	//ThinningObject.KwonWoongKangThinning(image_thinning, image_thinning); // DO SPRAWDZENIA PASS2
	//ThinningObject.ZhangWangThinning(image_thinning, image_thinning);
	//ThinningObject.ZhangWangThinning(test, test);
	//imwrite("Data/sign.bmp", test);
	//ThinningObject.HilditchThinning(image_thinning, image_thinning);
	//ThinningObject.HilditchThinning(test, test);
	//imwrite("Data/test.bmp", test);
	//ThinningObject.ArabicParallelThinning(image_thinning, image_thinning);
	//ThinningObject.EfficientParallelThinning(image_thinning, image_thinning);  //Do sprawdzenia warunki
	ThinningObject.ImprovedArabicParallelThinning(image_thinning, image_thinning);
	ThinningObject.ImprovedArabicParallelThinning(image_thinning1, image_thinning1);
	//ThinningObject.StentifordThinning(image_thinning, image_thinning);
	
	imwrite("Data/Program/Thinning.bmp", image_thinning);
	imwrite("Data/Program/Thinning1.bmp", image_thinning1);
	///////////////////////////////////////////////////////////////////////////

	//DETECTION
	Detection DetectionObject;
	Mat Detection;
	Mat DetectionNegative;
	std::vector<int> DeltaListX;
	std::vector<int> DeltaListY;
	std::vector<int> DeltaListX_Negative;
	std::vector<int> DeltaListY_Negative;
	std::vector<int> EndListX;
	std::vector<int> EndListY;
	std::vector<string> Direction_Ending;
	std::vector<string> Direction_Delta;
	std::vector<int> EndListX_Negative;
	std::vector<int> EndListY_Negative;
	std::vector<string> Direction_Ending_Negative;
	std::vector<string> Direction_Delta_Negative;
	Detection = image_thinning.clone();
	DetectionNegative = image_thinning_negative.clone();
	DetectionObject.EndingDetection(image_thinning, Detection, EndListX, EndListY, Direction_Ending);
	DetectionObject.DeltaDetection(image_thinning, Detection, DeltaListX, DeltaListY, Direction_Delta);


	//detection image1
	Mat Detection1;
	Mat DetectionNegative1;
	std::vector<int> DeltaListX1;
	std::vector<int> DeltaListY1;
	std::vector<int> DeltaListX_Negative1;
	std::vector<int> DeltaListY_Negative1;
	std::vector<int> EndListX1;
	std::vector<int> EndListY1;
	std::vector<string> Direction_Ending1;
	std::vector<string> Direction_Delta1;
	std::vector<int> EndListX_Negative1;
	std::vector<int> EndListY_Negative1;
	std::vector<string> Direction_Ending_Negative1;
	std::vector<string> Direction_Delta_Negative1;
	Detection1 = image_thinning1.clone();
	DetectionNegative1 = image_thinning_negative1.clone();
	DetectionObject.EndingDetection(image_thinning1, Detection1, EndListX1, EndListY1, Direction_Ending1);
	DetectionObject.DeltaDetection(image_thinning1, Detection1, DeltaListX1, DeltaListY1, Direction_Delta1);

	/*DetectionObject.EndingDetectionCN(image_thinning, Detection, EndListX, EndListY, Direction_Ending);
	DetectionObject.DeltaDetectionCN(image_thinning, Detection, DeltaListX, DeltaListY, Direction_Delta);*/
	/*float e1 = 0;
	float e2 = 0;
	float time = 0;
	e1 = cv::getTickCount();
	DetectionObject.DeltaDetectionCN(image_thinning, Detection, DeltaListX, DeltaListY, Direction_Delta);
	e2 = cv::getTickCount();
	time = (e2 - e1) / cv::getTickFrequency();
	std::cout << "czas: " << time << std::endl;*/
	/*Detection = image_thinning.clone();
	DetectionObject.EndingDetectionCN(image_thinning, Detection, EndListX, EndListY, Direction_Ending);
	time = 0;
	e1 = 0;
	e2 = 0;*/

	/*e1 = cv::getTickCount();
	DetectionObject.DeltaDetection2(image_thinning, Detection, DeltaListX, DeltaListY, Direction_Delta);
	e2 = cv::getTickCount();
	time = (e2 - e1) / cv::getTickFrequency();
	std::cout << "czas: " << time << std::endl;*/

	//detectopm functions for image - negative
	DetectionObject.EndingDetection(image_thinning_negative, DetectionNegative, EndListX_Negative, EndListY_Negative, Direction_Ending_Negative);
	DetectionObject.DeltaDetection(image_thinning_negative, DetectionNegative, DeltaListX_Negative, DeltaListY_Negative, Direction_Delta_Negative);
	imwrite("Data/Program/MinutiaeDetection.bmp", Detection);

	//detection functions dor image1 - negative
	DetectionObject.EndingDetection(image_thinning_negative1, DetectionNegative1, EndListX_Negative1, EndListY_Negative1, Direction_Ending_Negative1);
	DetectionObject.DeltaDetection(image_thinning_negative1, DetectionNegative1, DeltaListX_Negative1, DeltaListY_Negative1, Direction_Delta_Negative1);
	imwrite("Data/Program/MinutiaeDetection1.bmp", Detection1);
	////////////////////////////////////////////////////////////////////////////////////
	///TRZEBA BY TERAZ POROWNAC CZY ZAKONCZENIE ODPOWIADA DELCIE NA NEGATYWIE I ODWROTNIE
	///I EWENTUALNIE WYCZYSCIC TO CO SIE NIE POKRYWA - TO ALBO W BLOKU DETECTION ALBO FALSE MINUTIAE CLEANER
	///KOLEJNA SPRAWA TO KIERUNEK MINUCJI - TODO!
	///KWESTIA ZAPISANIA INFORMACJI O MINUCJACH - JAKI TYP DANYCH?
	///////////////////////////////////////////////////////////////////////////////////
	//FALSE MINUTIAE CLEANER///////////////////////// - image
	FalseMinutiae FalseMinutiaeObject;
	Mat Minutiae;
	Mat CleanMinutiae;
	Minutiae = image_thinning.clone();
	CleanMinutiae = Minutiae.clone();
	std::vector<int> OutEndListX;
	std::vector<int> OutEndListY;
	std::vector<int> OutDeltaListX;
	std::vector<int> OutDeltaListY;
	std::vector<int> CleanEndListX;
	std::vector<int> CleanEndListY;
	std::vector<int> CleanDeltaListX;
	std::vector<int> CleanDeltaListY;
	std::vector<string> OutDirection_Ending;
	FalseMinutiaeObject.EndingDetectionCleaner(EndListX, EndListY, Minutiae, OutEndListX, OutEndListY, Direction_Ending, OutDirection_Ending);
	FalseMinutiaeObject.DeltaDetectionCleaner(DeltaListX, DeltaListY, Minutiae, OutDeltaListX, OutDeltaListY);
	FalseMinutiaeObject.FrameMark(52, 174, 384, 546, OutEndListX, OutEndListY, CleanEndListX, CleanEndListY, OutDeltaListX, OutDeltaListY, CleanDeltaListX, CleanDeltaListY, CleanMinutiae);
	imwrite("Data/Program/Minutiae.bmp", CleanMinutiae);
	/////////////////////////////////////////////////////////////////////////

	//FALSE MINUTIAE CLEANER///////////////////////// - image1
	Mat Minutiae1;
	Mat CleanMinutiae1;
	Minutiae1 = image_thinning1.clone();
	CleanMinutiae1 = Minutiae1.clone();
	std::vector<int> OutEndListX1;
	std::vector<int> OutEndListY1;
	std::vector<int> OutDeltaListX1;
	std::vector<int> OutDeltaListY1;
	std::vector<int> CleanEndListX1;
	std::vector<int> CleanEndListY1;
	std::vector<int> CleanDeltaListX1;
	std::vector<int> CleanDeltaListY1;
	std::vector<string> OutDirection_Ending1;
	FalseMinutiaeObject.EndingDetectionCleaner(EndListX1, EndListY1, Minutiae1, OutEndListX1, OutEndListY1, Direction_Ending1, OutDirection_Ending1);
	FalseMinutiaeObject.DeltaDetectionCleaner(DeltaListX1, DeltaListY1, Minutiae1, OutDeltaListX1, OutDeltaListY1);
	FalseMinutiaeObject.FrameMark(52, 174, 384, 546, OutEndListX1, OutEndListY1, CleanEndListX1, CleanEndListY1, OutDeltaListX1, OutDeltaListY1, CleanDeltaListX1, CleanDeltaListY1, CleanMinutiae1);
	imwrite("Data/Program/Minutiae1.bmp", CleanMinutiae1);
	/////////////////////////////////////////////////////////////////////////

	int a;
	cin >> a;
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}