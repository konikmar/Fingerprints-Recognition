#ifndef _MATCHING_H_
#define _MATCHING_H_

#include "opencv2/opencv.hpp"
using namespace cv;

class Matching{
public:
	// overloaded constructor
	Matching(int x, int y, std::string Type, std::string Direction);
	//constructor
	Matching();

	//Generate pairs
	void GeneratePairs(std::vector<Matching> minutiae, Mat& PairsImage, std::vector<std::vector<Matching>>& Pairs);

	//Similar Pairs
	void SimilarPairs(std::vector<Matching> Pair, std::vector<Matching> Pair1, bool& Result);

	//Similar Angles
	void SimilarAngles(float diff1, float diff2, bool& Result);

	//Similar Diffs
	void SimilarDiffs(float diff1, float diff2, bool& Result);

	//DoTranslation on Source Data
	void DoTranslation(float TranslationX, float TranslationY, std::vector<Matching>& MinutiaeList);

	//ExtractTransformationParams
	void ExtractTransformationParams(std::vector<Matching> Pair, std::vector<Matching> Pair1, bool& Result,
		float& Rotation, float& TranslationX, float& TranslationY);

	//Exsist Sufficent Matches
	void ExistSufficentMatches(std::vector<Matching>& MinutiaeList, std::vector<Matching>& MinutiaeList1, bool& Result, std::vector<int>& MatchedMinutiae);

	//Matches
	void Matches(Matching Minutiae, Matching Minutiae1, bool& Result);

	//GenerateGraph5
	void GenerateGraph5(std::vector<Matching> minutiae, Mat& PairsImage, std::vector<std::vector<Matching>>& Pairs);

	//GenerateGraph10
	void GenerateGraph10(std::vector<Matching> minutiae, Mat& PairsImage, std::vector<std::vector<Matching>>& Pairs);

	// Virtual descrutor
	~Matching();

	int x;
	int y;
	std::string Type;
	std::string Direction;
	float DirectionAngle;
private:
	
};

#endif