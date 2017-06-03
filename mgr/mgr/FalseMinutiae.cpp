#include "FalseMinutiae.h"
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;

// overloaded constructor
FalseMinutiae::FalseMinutiae(){
}
void FalseMinutiae::DeltaDetectionCleaner(const std::vector<int> SrcDeltaListX, const std::vector<int> SrcDeltaListY, Mat& dst, std::vector<int> DeltaListX, std::vector<int> DeltaListY){

}
void FalseMinutiae::EndingDetectionCleaner(const std::vector<int> SrcEndListX, const std::vector<int> SrcEndListY, Mat& dst, std::vector<int> EndListX, std::vector<int> EndListY){
	double Length;
	cvtColor(dst, dst, COLOR_GRAY2RGB);
	std::cout << "liczba minucji: " << SrcEndListX.size() << std::endl;
	std::vector<int> ListX = SrcEndListX;
	std::vector<int> ListY = SrcEndListY;
	std::vector<int> IndexI;
	std::vector<int> IndexJ;
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
					i++;
					j = SrcEndListX.size();
				}
			}
		}
	}
	//DOTAD CHYBA JEST OK///
	//PONIZEJ COS NIE GRA///
	std::cout << "ilosc falszywych minucji: " << IndexI.size()+IndexJ.size()<< std::endl;
	int k;
	for (int i = 0; i < SrcEndListX.size(); i++)
	{
		for (int j = 0; j < IndexI.size(); j++)
		{
			k = 0;
			if (i == IndexI[j])
			{
				k = 1;
			}
			if (k == 0){
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