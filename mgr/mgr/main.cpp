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
	Mat image;
	image = imread("Data/013_4_2.bmp", 1);
	//image = imread("Data/fingerprint.png", 1); // non-real, simple fingerprint
	//image = imread("Data/ukos.jpg", 1);
	Mat test;
	test = imread("Data/ukos.jpg", 1);
	Size size(3 * image.cols, 3 * image.rows); //wczesniej bylo 6
	resize(image, image, size);
	//namedWindow("okno", 1);
	//imshow("okno", image);

	cvtColor(image, image, CV_BGR2GRAY);

	///////////////////////////////////////////////////////////////
	
	////PREPROCESSING
	//Preprocessing PreprocesingObject;
	
	////THRESHOLD
	//Mat image_threshold = image.clone();
	//PreprocesingObject.Threshold(image_threshold);

	////MEDIAN FILTER
	//Mat image_median = image_threshold.clone();
	//PreprocesingObject.Filter(image_median);
	//PreprocesingObject.Filter(image_median);
	//PreprocesingObject.Filter(image_median);
	//PreprocesingObject.Filter(image_median);

	////DILATE	
	//Mat image_dilate = image_median.clone();
	//PreprocesingObject.Dilate(image_dilate);

	//////////////////////////////////////////////////////////////

	////PREPROCESSING 2 na razie robiony za pomoc¹ prostych operacji, metod¹ prób i b³êdów, na tych nowych fotach s¹ mocne pory, co przeszkadza i ciê¿ko je w 100% wyeliminowaæ bez popsucia odcisku :( 
	Preprocessing PreprocesingObject;

	//WINDOWING
	Mat image_windowing = image.clone();
	PreprocesingObject.Windowing(image_windowing);

	//MEDIAN FILTER
	Mat image_median = image_windowing.clone();
	PreprocesingObject.Filter(image_median);
	PreprocesingObject.Filter(image_median);
	PreprocesingObject.Filter(image_median);

	////THRESHOLD
	Mat image_threshold = image_median.clone();
	adaptiveThreshold(image_threshold, image_threshold, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 75, 1);

	////DILATE	
	Mat image_dilate = image_threshold.clone();
	//PreprocesingObject.Erode(image_dilate);
	//PreprocesingObject.Dilate(image_dilate);

	//////////////////////////////////////////////////////////////
	
	Size size1( image_dilate.cols/2,  image_dilate.rows/2); //bylo /3
	resize(image_dilate, image_dilate, size1);
	PreprocesingObject.Threshold(image_dilate);
	imwrite("Data/00111_a.bmp", image_dilate);	//save image after preprocessing

	//////////////////////////////////////////////////////////////

	//GABOR FILTER
	//GaborFilter filter;

	////string file_name;
	////cout << "Podaj nazwe pliku (z rozszerzeniem): ";
	////cin >> file_name;

	////std::string imgDest = "./obrazy/";
	////imgDest.append(file_name);

	////Mat image = imread("obrazy/" + file_name, 0);
	//
	//Mat image_dilate2 = imread("Data/00111_a.bmp", 0);
	//Size size2(0.2 * image_dilate2.cols, 0.2 * image_dilate2.rows);
	//resize(image_dilate2, image_dilate2, size2);

	//
	////if (!image.data)                              // Check for invalid input
	////{
	////	cout << endl << "Brak pliku o podanej nazwie." << std::endl;
	////	system("pause");
	////	return -1;
	////}

	////cout << endl << "Obliczenia..." << std::endl;

	//Mat image_color;
	//Mat Gx, Gy;	///gradient images
	//Mat orientation_map; /// matrix containing orientation of (group of) pixels
	//Mat image_enhanced; /// image after enhancement
	//Mat orientation_image; /// image with marked orientation of group of pixels
	//Mat angle_matrix;	///angle matrix for method 3
	//Mat magnitude_matrix; ///magnitude matrix for method 3

	//cvtColor(image_dilate2, image_color, CV_GRAY2BGR);
	//filter.gradientImage(image_dilate2, Gx, Gy);	///gradient computation

	//switch (filter.method) ///choosing right computation of orientation method
	//{
	//case 1:
	//{
	//	filter.getOrientationMap(image_color, Gx, Gy, orientation_map, orientation_image);
	//	break;
	//}
	//case 2:
	//{
	//	filter.getOrientationMap(image_color, Gx, Gy, orientation_map, orientation_image);
	//	break;
	//}
	//case 3:
	//{
	//	phase(Gx, Gy, angle_matrix, false);
	//	magnitude(Gx, Gy, magnitude_matrix);

	//	cvtColor(image_dilate2, orientation_image, CV_GRAY2BGR);
	//	normalize(magnitude_matrix, magnitude_matrix, 0, 1, NORM_MINMAX);
	//	filter.getOrientationMapGradient(angle_matrix, magnitude_matrix, 31, orientation_map, orientation_image);
	//	break;
	//}
	//default:
	//{
	//	cout << "Blad. Wybrano nieistniejaca metode tworzenia mapy orientacji. Sprawdz parametr 'method' obiektu klasy GaborFilter" << endl;
	//	system("pause");
	//	return 0;
	//}
	//}



	//filter.enhanceImage(image_dilate2, image_enhanced, orientation_map, orientation_image);

	//cout << endl << "SPACJA - zmiana widoku" << endl;
	//cout << "ESC - koniec" << endl;
	//int key = 0;
	//while (1)	///displaying results
	//{
	//	imshow("Image before/after", image_dilate2);
	//	key = waitKey(0);
	//	if (key == 27) break;
	//	imshow("Image before/after", orientation_image);
	//	key = waitKey(0);
	//	if (key == 27) break;
	//	imshow("Image before/after", image_enhanced);
	//	key = waitKey(0);
	//	if (key == 27) break;
	//}
	/////////////////////////////////////////////////////
	imwrite("Data/Preprocessing.bmp", image_dilate);

	//THINNING
	Thinning ThinningObject;
	
	Mat image_thinning = imread("Data/Preprocessing.bmp");
	//Mat image_thinning = imread("Data/00111_a.bmp");
	//Size size3(0.2 * image_thinning.cols, 0.2 * image_thinning.rows);
	//resize(image_thinning, image_thinning, size3);

	if (!image_thinning.data)
		return -1;

	
	
	cvtColor(image_thinning, image_thinning, CV_BGR2GRAY);
	threshold(image_thinning, image_thinning, 10, 255, CV_THRESH_BINARY);
	Mat image_thinning_negative = image_thinning.clone();

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
	//ThinningObject.StentifordThinning(image_thinning, image_thinning);
	
	imwrite("Data/Thinning.bmp", image_thinning);
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
	//DetectionObject.EndingDetection(image_thinning, Detection, EndListX, EndListY, Direction_Ending);
	//DetectionObject.DeltaDetection(image_thinning, Detection, DeltaListX, DeltaListY, Direction_Delta);

	DetectionObject.EndingDetectionCN(image_thinning, Detection, EndListX, EndListY, Direction_Ending);
	DetectionObject.DeltaDetectionCN(image_thinning, Detection, DeltaListX, DeltaListY, Direction_Delta);
	DetectionObject.EndingDetection(image_thinning_negative, DetectionNegative, EndListX_Negative, EndListY_Negative, Direction_Ending_Negative);
	DetectionObject.DeltaDetection(image_thinning_negative, DetectionNegative, DeltaListX_Negative, DeltaListY_Negative, Direction_Delta_Negative);
	imwrite("Data/MinutiaeDetection.bmp", Detection);
	////////////////////////////////////////////////////////////////////////////////////
	///TRZEBA BY TERAZ POROWNAC CZY ZAKONCZENIE ODPOWIADA DELCIE NA NEGATYWIE I ODWROTNIE
	///I EWENTUALNIE WYCZYSCIC TO CO SIE NIE POKRYWA - TO ALBO W BLOKU DETECTION ALBO FALSE MINUTIAE CLEANER
	///KOLEJNA SPRAWA TO KIERUNEK MINUCJI - TODO!
	///KWESTIA ZAPISANIA INFORMACJI O MINUCJACH - JAKI TYP DANYCH?
	///////////////////////////////////////////////////////////////////////////////////
	//FALSE MINUTIAE CLEANER/////////////////////////
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
	FalseMinutiaeObject.FrameMark(195, 50, 470, 500, OutEndListX, OutEndListY, CleanEndListX, CleanEndListY, OutDeltaListX, OutDeltaListY, CleanDeltaListX, CleanDeltaListY, CleanMinutiae);
	/////////////////////////////////////////////////////////////////////////

	int a;
	cin >> a;
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}