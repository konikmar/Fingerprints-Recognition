#include "opencv2/opencv.hpp"
#include "FingerprintsRecognition.h"
#include "Preprocessing.h"
#include "Thinning.h"
#include "GaborFilter.h"
#include "Detection.h"
#include "FalseMinutiae.h"
#include "Matching.h"

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
	//image = imread("Data/012_4_3.bmp", 1);
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
	//image1 = imread("Data/012_4_5.bmp", 1);
	resize(image1, image1, size);
	cvtColor(image1, image1, CV_BGR2GRAY);
	/////////////////////////////////
	////PREPROCESSING
	std::cout << "PREPROCESSING..." << std::endl;
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
	threshold(image_dilate, image_dilate, 240, 255, CV_THRESH_BINARY);
	///filtracja porow odcisk wzorcowy
	PreprocesingObject.FilterPores(image_dilate);
	

	//RESIZE odcisk 1
	resize(image_dilate1, image_dilate1, size1);
	threshold(image_dilate1, image_dilate1, 240, 255, CV_THRESH_BINARY);
	///filtracja porow odcisk 1
	PreprocesingObject.FilterPores(image_dilate1);

	/////////////////////////////////////////////////////
	imwrite("Data/Program/Preprocessing.bmp", image_dilate); //save image after preprocessing - obraz wzorcowy
	
	imwrite("Data/Program/Preprocessing1.bmp", image_dilate1);	//save image after preprocessing - obraz 1

	//THINNING
	std::cout<<"THINNING..." << std::endl;
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
	std::cout << "DETECTION..." << std::endl;
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
	DetectionObject.EndingDetectionCN(image_thinning, Detection, EndListX, EndListY, Direction_Ending);
	std::cout << "TARGET IMAGE - DETECTED ENDING: " << EndListX.size() << std::endl;
	DetectionObject.DeltaDetectionCN(image_thinning, Detection, DeltaListX, DeltaListY, Direction_Delta);
	std::cout << "TARGET IMAGE - DETECTED DELTA: " << DeltaListX.size() << std::endl;


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
	DetectionObject.EndingDetectionCN(image_thinning1, Detection1, EndListX1, EndListY1, Direction_Ending1);
	std::cout << "SOURCE IMAGE - DETECTED ENDING: " << EndListX1.size() << std::endl;
	DetectionObject.DeltaDetectionCN(image_thinning1, Detection1, DeltaListX1, DeltaListY1, Direction_Delta1);
	std::cout << "SOURCE IMAGE - DETECTED DELTA: " << DeltaListX1.size() << std::endl;

	//DetectionObject.EndingDetectionCN(image_thinning, Detection, EndListX, EndListY, Direction_Ending);
	//DetectionObject.DeltaDetectionCN(image_thinning, Detection, DeltaListX, DeltaListY, Direction_Delta);
	/*float e1 = 0;
	float e2 = 0;
	float time = 0;
	e1 = cv::getTickCount();
	DetectionObject.DeltaDetectionCN(image_thinning, Detection, DeltaListX, DeltaListY, Direction_Delta);
	e2 = cv::getTickCount();
	time = (e2 - e1) / cv::getTickFrequency();
	std::cout << "czas: " << time << std::endl;
	Detection = image_thinning.clone();
	time = 0;
	e1 = 0;
	e2 = 0;

	e1 = cv::getTickCount();
	DetectionObject.DeltaDetection2(image_thinning, Detection, DeltaListX, DeltaListY, Direction_Delta);
	e2 = cv::getTickCount();
	time = (e2 - e1) / cv::getTickFrequency();*/
	//std::cout << "czas: " << time << std::endl;

	//FALSE MINUTIAE CLEANER///////////////////////// - image
	std::cout << "FALSE MINUTIAE CLEANING..." << std::endl;
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
	std::vector<string> CleanDirection_Ending;
	std::vector<string> OutDirection_Delta;
	std::vector<string> CleanDirection_Delta;
	FalseMinutiaeObject.EndingDetectionCleaner(EndListX, EndListY, Minutiae, OutEndListX, OutEndListY, Direction_Ending, 
		OutDirection_Ending);
	FalseMinutiaeObject.DeltaDetectionCleaner(DeltaListX, DeltaListY, Minutiae, OutDeltaListX, OutDeltaListY, Direction_Delta,
		OutDirection_Delta);
	FalseMinutiaeObject.FrameMark(52, 174, 384, 546, OutEndListX, OutEndListY, CleanEndListX, CleanEndListY, OutDeltaListX, 
		OutDeltaListY, CleanDeltaListX, CleanDeltaListY, CleanMinutiae, OutDirection_Ending, CleanDirection_Ending, OutDirection_Delta, 
		CleanDirection_Delta);
	imwrite("Data/Program/Minutiae.bmp", CleanMinutiae);
	std::cout << "TARGET IMAGE - MINUTIAE ENDING AFTER FALSE MINUTIAE CLEANING: " << CleanEndListX.size() << std::endl;
	std::cout << "TARGET IMAGE - MINUTIAE DELTA AFTER FALSE MINUTIAE CLEANING: " << CleanDeltaListX.size() << std::endl;
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
	std::vector<string> CleanDirection_Ending1;
	std::vector<string> OutDirection_Delta1;
	std::vector<string> CleanDirection_Delta1;
	FalseMinutiaeObject.EndingDetectionCleaner(EndListX1, EndListY1, Minutiae1, OutEndListX1, OutEndListY1, Direction_Ending1,
		OutDirection_Ending1);
	FalseMinutiaeObject.DeltaDetectionCleaner(DeltaListX1, DeltaListY1, Minutiae1, OutDeltaListX1, OutDeltaListY1,Direction_Delta1,
		OutDirection_Delta1);
	FalseMinutiaeObject.FrameMark(52, 174, 384, 546, OutEndListX1, OutEndListY1, CleanEndListX1, CleanEndListY1, OutDeltaListX1,
		OutDeltaListY1, CleanDeltaListX1, CleanDeltaListY1, CleanMinutiae1, OutDirection_Ending1, CleanDirection_Ending1, 
		OutDirection_Delta1, CleanDirection_Delta1);
	imwrite("Data/Program/Minutiae1.bmp", CleanMinutiae1);
	std::cout << "SOURCE IMAGE - MINUTIAE ENDING AFTER FALSE MINUTIAE CLEANING: " << CleanEndListX1.size() << std::endl;
	std::cout << "SOURCE IMAGE - MINUTIAE DELTA AFTER FALSE MINUTIAE CLEANING: " << CleanDeltaListX1.size() << std::endl;



	///MATCHING
	std::cout << "MATCHING" << std::endl;
	//////////////////////////MinutiaeList image
	std::vector<Matching> MinutiaeList;
	for (int i = 0; i < CleanDeltaListX.size() + CleanEndListX.size(); i++){
		if (i < CleanEndListX.size()){
			Matching MatchingObject = Matching(CleanEndListX[i], CleanEndListY[i], "EndList", CleanDirection_Ending[i]);
			MinutiaeList.push_back(MatchingObject);
			MatchingObject.~Matching();
		}
		if (i >= CleanEndListX.size()){
			Matching MatchingObject = Matching(CleanDeltaListX[i - CleanEndListX.size()], CleanDeltaListY[i - CleanEndListX.size()], 
				"DeltaList", CleanDirection_Delta[i - CleanEndListX.size()]);
			MinutiaeList.push_back(MatchingObject);
			MatchingObject.~Matching();
		}
		
	}

	////////////////////MinutiaeList image1
	std::vector<Matching> MinutiaeList1;
	for (int i = 0; i < CleanDeltaListX1.size() + CleanEndListX1.size(); i++){
		if (i < CleanEndListX1.size()){
			Matching MatchingObject1 = Matching(CleanEndListX1[i], CleanEndListY1[i], "EndList", CleanDirection_Ending1[i]);
			MinutiaeList1.push_back(MatchingObject1);
			MatchingObject1.~Matching();
		}
		if (i >= CleanEndListX1.size()){
			Matching MatchingObject1 = Matching(CleanDeltaListX1[i - CleanEndListX1.size()], CleanDeltaListY1[i - CleanEndListX1.size()], 
				"DeltaList", CleanDirection_Delta1[i - CleanEndListX1.size()]);
			MinutiaeList1.push_back(MatchingObject1);
			MatchingObject1.~Matching();
		}

	}
	///////////////////////////
	///Matching
	///////////////generate pairs image
	Matching MatchingObject;
	Mat PairsImage = CleanMinutiae.clone();
	std::vector<std::vector<Matching>> Pairs;
	MatchingObject.GenerateGraph10(MinutiaeList, PairsImage, Pairs);
	imwrite("Data/Program/Pairs.bmp", PairsImage);
	std::cout << "TARGET IMAGE - GENERATED MINUTIAE PAIRS: " << Pairs.size() << std::endl;
	////////////////////

	/////////generate pairs image1
	Mat PairsImage1 = CleanMinutiae1.clone();
	std::vector<std::vector<Matching>> Pairs1;
	MatchingObject.GenerateGraph10(MinutiaeList1, PairsImage1, Pairs1);
	imwrite("Data/Program/Pairs1.bmp", PairsImage1);
	std::cout << "SOURCE IMAGE - GENERATED MINUTIAE PAIRS: " << Pairs1.size() << std::endl;
	////////////////////////////////////////////
	
	
	bool SimilarPairsResult;
	bool ExtractTransformationResult;
	bool SuffcientMatchesResult;
	float Rotation;
	float TranslationX;
	float TranslationY;
	std::vector<Matching> MinutiaeListSource;
	std::vector<int> MatchedMinutiae;
	MinutiaeListSource = MinutiaeList1;
	for (int i = 0; i < Pairs.size(); i++){
		for (int j = 0; j < Pairs1.size(); j++){
			///similar pairs
			MatchingObject.SimilarPairs(Pairs[i], Pairs1[j], SimilarPairsResult);
			if (SimilarPairsResult == true){
				MatchingObject.ExtractTransformationParams(Pairs[i], Pairs1[j], ExtractTransformationResult, Rotation,
					TranslationX, TranslationY);
				if (ExtractTransformationResult == true){
					MatchingObject.DoTranslation(TranslationX, TranslationY, MinutiaeListSource);
					MatchingObject.ExistSufficentMatches(MinutiaeListSource, MinutiaeList, SuffcientMatchesResult, MatchedMinutiae);
					if (SuffcientMatchesResult == true){
					}
					//restore
					MinutiaeListSource = MinutiaeList1;

				}
			}
		}
	}
	std::cout << "MATCHED MINUTIAE :" << *std::max_element(MatchedMinutiae.begin(), MatchedMinutiae.end())<< std::endl;


	///////////////////////////////////
	int a;
	cin >> a;
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}