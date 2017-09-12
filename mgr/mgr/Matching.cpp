#include "Matching.h"
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;

// overloaded constructor
Matching::Matching(int x1, int y1, std::string Type1, std::string Direction1){
	x = x1;
	y = y1;
	Type = Type1;
	Direction = Direction1;
	if (Direction1 == "NW"){
		DirectionAngle = 135;
	}
	if (Direction1 == "N"){
		DirectionAngle = 90;
	}
	if (Direction1 == "NE"){
		DirectionAngle = 45;
	}
	if (Direction1 == "E"){
		DirectionAngle = 0;
	}
	if (Direction1 == "SE"){
		DirectionAngle = 315;
	}
	if (Direction1 == "S"){
		DirectionAngle = 270;
	}
	if (Direction1 == "SW"){
		DirectionAngle = 225;
	}
	if (Direction1 == "W"){
		DirectionAngle = 180;
	}
}
Matching::Matching(){

}

void Matching::GeneratePairs(std::vector<Matching> minutiae, Mat& PairsImage, std::vector<std::vector<Matching>>& Pairs){
	float Length;
	std::vector < Matching > temp;
	for (int i = 0; i < minutiae.size(); i++){
		for (int j = i+1; j < minutiae.size(); j++){
			if (i != j){
				Length = sqrt((minutiae[i].x - minutiae[j].x)*(minutiae[i].x - minutiae[j].x) + (minutiae[i].y - minutiae[j].y)*(minutiae[i].y - minutiae[j].y));
				//std::cout << "dlugosc" <<Length<< std::endl;
				if (Length < 80){
					//std::cout << "para  " << std::endl;
					line(PairsImage, Point(minutiae[i].x, minutiae[i].y), Point(minutiae[j].x, minutiae[j].y), Scalar(0, 255, 0), 1, 8);
					temp.push_back(minutiae[i]);
					temp.push_back(minutiae[j]);
					Pairs.push_back(temp);
					temp.clear();
				}
			}
		}
	}

}
int licznik = 0;
void Matching::SimilarPairs(std::vector<Matching> Pair, std::vector<Matching> Pair1, bool& Result){
	float Length;
	float Length1;
	Result = false;
	Length = sqrt((Pair[0].x - Pair[1].x)*(Pair[0].x - Pair[1].x) + (Pair[0].y - Pair[1].y)*(Pair[0].y - Pair[1].y));
	Length1 = sqrt((Pair1[0].x - Pair1[1].x)*(Pair1[0].x - Pair1[1].x) + (Pair1[0].y - Pair1[1].y)*(Pair1[0].y - Pair1[1].y));
	if ((Length1 > Length - 0.05*Length) && (Length1 < Length + 0.05*Length)){
		Result = true;
		licznik++;
	}
}
void Matching::SimilarAngles(float diff1, float diff2, bool& Result)
{
	float diff;
	Result = false;
	diff = diff2 - diff1;
	if ((diff <= 45) && (diff >= -45)){
		Result = true;
	}
}
void Matching::SimilarDiffs(float diff1, float diff2, bool& Result){
	float diff;
	Result = false;
	diff = abs(diff2 - diff1);
	if (diff < 10){
		Result = true;
	}
}

void Matching::ExtractTransformationParams(std::vector<Matching> Pair, std::vector<Matching> Pair1, bool& Result, float& Rotation,
	float& TranslationX, float& TranslationY){
	float angleDiff1;
	float angleDiff2;
	float xDiff1;
	float xDiff2;
	float yDiff1;
	float yDiff2;
	Result = false;
	/*angleDiff1 = Pair[0].DirectionAngle - Pair1[0].DirectionAngle;
	angleDiff2 = Pair[1].DirectionAngle - Pair1[1].DirectionAngle;
	if (angleDiff1 == 315)
	{
		angleDiff1 = 45;
	}
	if (angleDiff1 == -315)
	{
		angleDiff1 = -45;
	}
	if (angleDiff2 == 315)
	{
		angleDiff2 = 45;
	}
	if (angleDiff2 == -315)
	{
		angleDiff2 = -45;
	}*/

	//bool SimilarAnglesResult;
	//SimilarAngles(angleDiff1, angleDiff2, SimilarAnglesResult);
	//if (SimilarAnglesResult == true){
		//Rotation = (angleDiff1 + angleDiff2) / 2;
		Rotation = 0;
	//}
	//else{
	//	angleDiff1 = Pair[1].DirectionAngle - Pair1[0].DirectionAngle;
	//	angleDiff2 = Pair[0].DirectionAngle - Pair1[1].DirectionAngle;
	//	if (angleDiff1 == 315)
	//	{
	//		angleDiff1 = 45;
	//	}
	//	if (angleDiff1 == -315)
	//	{
	//		angleDiff1 = -45;
	//	}
	//	if (angleDiff2 == 315)
	//	{
	//		angleDiff2 = 45;
	//	}
	//	if (angleDiff2 == -315)
	//	{
	//		angleDiff2 = -45;
	//	}
	//	SimilarAngles(angleDiff1, angleDiff2, SimilarAnglesResult);
	//	if (SimilarAnglesResult == true){
			//Rotation = (angleDiff1 + angleDiff2) / 2;
			Rotation = 0;
	//	}
	//	else{
	//		goto failure;
	//	}
	//}
	xDiff1 = Pair[0].x - Pair1[0].x;
	xDiff2 = Pair[1].x - Pair1[1].x;
	yDiff1 = Pair[0].y - Pair1[0].y;
	yDiff2 = Pair[1].y - Pair1[1].y;
	bool SimilarDiffsXResult;
	bool SimilarDiffsYResult;
	SimilarDiffs(xDiff1, xDiff2, SimilarDiffsXResult);
	SimilarDiffs(yDiff1, yDiff2, SimilarDiffsYResult);
	if (SimilarDiffsXResult == true && SimilarDiffsYResult == true){
		TranslationX = (xDiff1 + xDiff2) / 2;
		TranslationY = (yDiff1 + yDiff2) / 2;
		Result = true;
	}
	else
	{
		xDiff1 = Pair[1].x - Pair1[0].x;
		xDiff2 = Pair[0].x - Pair1[1].x;
		yDiff1 = Pair[1].y - Pair1[0].y;
		yDiff2 = Pair[0].y - Pair1[1].y;
		SimilarDiffs(xDiff1, xDiff2, SimilarDiffsXResult);
		SimilarDiffs(yDiff1, yDiff2, SimilarDiffsYResult);
		if (SimilarDiffsXResult == true && SimilarDiffsYResult == true){
			TranslationX = (xDiff1 + xDiff2) / 2;
			TranslationY = (yDiff1 + yDiff2) / 2;
			Result = true;
		}
		else{
			Result = false;
		}
	}
//failure:Result = false;
}

void Matching::DoTranslation(float TranslationX, float TranslationY, std::vector<Matching>& MinutiaeList){
	for (int i = 0; i < MinutiaeList.size(); i++){
		MinutiaeList[i].x += (int)TranslationX;
		MinutiaeList[i].y += (int)TranslationY;
	}

}
void Matching::ExistSufficentMatches(std::vector<Matching>& MinutiaeList, std::vector<Matching>& MinutiaeList1, bool& Result, std::vector<int>& MatchedMinutiae){
	int MatchingMinutiae = 0;
	bool MatchesResult = false;
	for (int i = 0; i < MinutiaeList.size(); i++){
		for (int j = 0; j < MinutiaeList1.size(); j++){
			Matches(MinutiaeList[i], MinutiaeList1[j], MatchesResult);
			if (MatchesResult==true){
				MatchingMinutiae++;
			}
		}
	}
	if (MatchingMinutiae >= 12){
		Result = true;
	}
	else
		Result = false;
	MatchedMinutiae.push_back(MatchingMinutiae);
}
void Matching::Matches(Matching Minutiae, Matching Minutiae1, bool& Result){
	float diffX;
	float diffY;
	Result = false;
	diffX = abs(Minutiae.x - Minutiae1.x);
	diffY = abs(Minutiae.y - Minutiae1.y);
	if (diffX <= 10 && diffY <= 10 && Minutiae.Type == Minutiae1.Type && Minutiae.DirectionAngle == Minutiae1.DirectionAngle){
		Result = true;
	}
	else
		Result = false;
}
void Matching::GenerateGraph5(std::vector<Matching> minutiae, Mat& PairsImage, std::vector<std::vector<Matching>>& Pairs){
	float Length;
	std::vector < Matching > temp;
	std::vector < float > vectorLength;
	std::vector < int > vectorJ;
	int valueJ;
	for (int i = 0; i < minutiae.size(); i++){
		for (int j = i + 1; j < minutiae.size(); j++){
			Length = sqrt((minutiae[i].x - minutiae[j].x)*(minutiae[i].x - minutiae[j].x) + (minutiae[i].y - minutiae[j].y)*(minutiae[i].y - minutiae[j].y));
			vectorLength.push_back(Length);
			vectorJ.push_back(j);
		}
		for (int k = 0; k < 5; k++){
			int iterator = std::min_element(vectorLength.begin(), vectorLength.end()) - vectorLength.begin();
			Length = vectorLength[iterator];
			valueJ = vectorJ[iterator];
			//std::cout << "para  " << std::endl;
			line(PairsImage, Point(minutiae[i].x, minutiae[i].y), Point(minutiae[valueJ].x, minutiae[valueJ].y), Scalar(0, 255, 0), 1, 8);
			temp.push_back(minutiae[i]);
			temp.push_back(minutiae[valueJ]);
			Pairs.push_back(temp);
			temp.clear();
			vectorLength[iterator] = 1000;
		}
	}
}

void Matching::GenerateGraph10(std::vector<Matching> minutiae, Mat& PairsImage, std::vector<std::vector<Matching>>& Pairs){
	float Length;
	std::vector < Matching > temp;
	std::vector < float > vectorLength;
	std::vector < int > vectorJ;
	int valueJ;
	for (int i = 0; i < minutiae.size(); i++){
		for (int j = i + 1; j < minutiae.size(); j++){
			Length = sqrt((minutiae[i].x - minutiae[j].x)*(minutiae[i].x - minutiae[j].x) + (minutiae[i].y - minutiae[j].y)*(minutiae[i].y - minutiae[j].y));
			vectorLength.push_back(Length);
			vectorJ.push_back(j);
		}
		for (int k = 0; k < 10; k++){
			int iterator = std::min_element(vectorLength.begin(), vectorLength.end()) - vectorLength.begin();
			Length = vectorLength[iterator];
			valueJ = vectorJ[iterator];
			//std::cout << "para  " << std::endl;
			line(PairsImage, Point(minutiae[i].x, minutiae[i].y), Point(minutiae[valueJ].x, minutiae[valueJ].y), Scalar(0, 255, 0), 1, 8);
			temp.push_back(minutiae[i]);
			temp.push_back(minutiae[valueJ]);
			Pairs.push_back(temp);
			temp.clear();
			vectorLength[iterator] = 1000;
		}
	}
}
Matching::~Matching(){
}