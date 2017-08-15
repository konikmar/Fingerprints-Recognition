#include "FalseMinutiae.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <algorithm>

using namespace cv;

// overloaded constructor
FalseMinutiae::FalseMinutiae(){
}
void FalseMinutiae::DeltaDetectionCleaner(const std::vector<int> SrcDeltaListX, const std::vector<int> SrcDeltaListY, Mat& dst, std::vector<int> DeltaListX, std::vector<int> DeltaListY){

}
void FalseMinutiae::EndingDetectionCleaner(const std::vector<int> SrcEndListX, const std::vector<int> SrcEndListY, Mat& dst, std::vector<int> EndListX, std::vector<int> EndListY, std::vector<std::string> SrcDirection, std::vector<std::string> Direction){
	double Length;
	cvtColor(dst, dst, COLOR_GRAY2RGB);
	std::cout << "liczba minucji: " << SrcEndListX.size() << std::endl;
	std::vector<int> ListX = SrcEndListX;
	std::vector<int> ListY = SrcEndListY;
	std::vector<std::string> ListSrcDirection = SrcDirection;
	std::vector<int> IndexI;
	std::vector<int> IndexJ;

	int licznik = 0;
	for (int i = 0; i < SrcEndListX.size(); i++)
	{
		for (int j = 1; j < SrcEndListX.size(); j++)
		{
			if (!(i == j))
			{


				Length = sqrt((SrcEndListX[i] - SrcEndListX[j])*(SrcEndListX[i] - SrcEndListX[j]) + (SrcEndListY[i] - SrcEndListY[j]) * (SrcEndListY[i] - SrcEndListY[j])); //Wzor na dlugosc wektora
				//std::cout << "Dlugosc wektora: " << Length << std::endl;
				//circle(dst, Point(SrcEndListX[i], SrcEndListY[i]), 5, Scalar(255, 0, 255), 1, 8, 0);
				//circle(dst, Point(SrcEndListX[j], SrcEndListY[j]), 5, Scalar(0, 0, 255), 1, 8, 0);
				if (Length < 10)
				{
					/*ListX[i] = 0;
					ListY[i] = 0;
					ListX[j] = 0;
					ListY[j] = 0;*/
					IndexI.push_back(i);	//zapisanie indeksow falszywych minucji
					IndexJ.push_back(j);
					j = SrcEndListX.size();
				}
			}
		}
	}
	for (int i = 0; i < SrcEndListX.size(); i++)
	{
		for (int j = 1; j < SrcEndListX.size(); j++)
		{
			if (!(i == j))
			{
				Length = sqrt((SrcEndListX[i] - SrcEndListX[j])*(SrcEndListX[i] - SrcEndListX[j]) + (SrcEndListY[i] - SrcEndListY[j]) * (SrcEndListY[i] - SrcEndListY[j])); //Wzor na dlugosc wektora
			if (Length < 40)
			{
				if ((SrcDirection[i] == "N") && (SrcDirection[j] == "S" || SrcDirection[j] == "SE" || SrcDirection[j] == "SW")
					)
				{
					IndexI.push_back(i);	//zapisanie indeksow falszywych minucji
					IndexJ.push_back(j);
					//j = SrcEndListX.size();
					licznik++;
				}
				if ((SrcDirection[i] == "NW") && (SrcDirection[j] == "SE" || SrcDirection[j] == "S" || SrcDirection[j] == "E"))
				{
					IndexI.push_back(i);	//zapisanie indeksow falszywych minucji
					IndexJ.push_back(j);
					//j = SrcEndListX.size();
					licznik++;
				}
				if ((SrcDirection[i] == "W") && (SrcDirection[j] == "E" || SrcDirection[j] == "NW" || SrcDirection[j] == "SW"))
				{
					IndexI.push_back(i);	//zapisanie indeksow falszywych minucji
					IndexJ.push_back(j);
					//j = SrcEndListX.size();
					licznik++;
				}
				if ((SrcDirection[i] == "SW") && (SrcDirection[j] == "NE" || SrcDirection[j] == "N" || SrcDirection[j] == "E"))
				{
					IndexI.push_back(i);	//zapisanie indeksow falszywych minucji
					IndexJ.push_back(j);
					//j = SrcEndListX.size();
					licznik++;
				}
				if ((SrcDirection[i] == "S") && (SrcDirection[j] == "N" || SrcDirection[j] == "NW" || SrcDirection[j] == "NE"))
				{
					IndexI.push_back(i);	//zapisanie indeksow falszywych minucji
					IndexJ.push_back(j);
					j = SrcEndListX.size();
				}
				if ((SrcDirection[i] == "SE") && (SrcDirection[j] == "NW" || SrcDirection[j] == "N" || SrcDirection[j] == "W"))
				{
					IndexI.push_back(i);	//zapisanie indeksow falszywych minucji
					IndexJ.push_back(j);
					j = SrcEndListX.size();
				}
				if ((SrcDirection[i] == "E") && (SrcDirection[j] == "W" || SrcDirection[j] == "NW" || SrcDirection[j] == "SW"))
				{
					IndexI.push_back(i);	//zapisanie indeksow falszywych minucji
					IndexJ.push_back(j);
					j = SrcEndListX.size();
				}
				if ((SrcDirection[i] == "NE") && (SrcDirection[j] == "SW" || SrcDirection[j] == "S" || SrcDirection[j] == "W"))
				{
					IndexI.push_back(i);	//zapisanie indeksow falszywych minucji
					IndexJ.push_back(j);
					j = SrcEndListX.size();
				}

			}

			}
		}
	}
	//DOTAD CHYBA JEST OK///
	//PONIZEJ COS NIE GRA///
	std::cout << "ilosc falszywych minucji: " << IndexI.size()+IndexJ.size()<< std::endl;
	std::cout << "licznik: " << 2*licznik << std::endl;
	for (int i = 0; i < SrcEndListX.size(); i++)
	{
		for (int j = 0; j < IndexI.size(); j++)
		{
			if ((std::binary_search(IndexI.begin(), IndexI.end(), i) == false) && (std::binary_search(IndexJ.begin(), IndexJ.end(), i) == false))
			{
				EndListX.push_back(SrcEndListX[i]);
				EndListY.push_back(SrcEndListY[i]);
			}
		}

	}
	for (int i = 0; i < EndListX.size(); i++){
		circle(dst, Point(EndListX[i], EndListY[i]), 5, Scalar(255, 0, 255), 1, 8, 0);
	}
}
FalseMinutiae::~FalseMinutiae(){
}