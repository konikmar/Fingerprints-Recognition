#include "FalseMinutiae.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <algorithm>

using namespace cv;

// overloaded constructor
FalseMinutiae::FalseMinutiae(){
}
void FalseMinutiae::DeltaDetectionCleaner(const std::vector<int> SrcDeltaListX, const std::vector<int> SrcDeltaListY, Mat& dst, std::vector<int>& DeltaListX, std::vector<int>& DeltaListY, std::vector<std::string> SrcDirection, std::vector<std::string>& Direction){
	double Length;
//	cvtColor(dst, dst, COLOR_GRAY2RGB);
	//std::cout << "liczba minucji: " << SrcDeltaListX.size() << std::endl;
	std::vector<int> ListX = SrcDeltaListX;
	std::vector<int> ListY = SrcDeltaListY;
	std::vector<int> IndexI;
	std::vector<int> IndexJ;

	int licznik = 0;
	for (int i = 0; i < SrcDeltaListX.size(); i++)
	{
		for (int j = 1; j < SrcDeltaListX.size(); j++)
		{
			if (!(i == j))
			{


				Length = sqrt((SrcDeltaListX[i] - SrcDeltaListX[j])*(SrcDeltaListX[i] - SrcDeltaListX[j]) + (SrcDeltaListY[i] - SrcDeltaListY[j]) * (SrcDeltaListY[i] - SrcDeltaListY[j])); //Wzor na dlugosc wektora
				//std::cout << "Dlugosc wektora: " << Length << std::endl;
				//circle(dst, Point(SrcEndListX[i], SrcEndListY[i]), 5, Scalar(255, 0, 255), 1, 8, 0);
				//circle(dst, Point(SrcEndListX[j], SrcEndListY[j]), 5, Scalar(0, 0, 255), 1, 8, 0);
				if (Length < 15)
				{
					IndexI.push_back(i);	//zapisanie indeksow falszywych minucji
					IndexJ.push_back(j);
					j = SrcDeltaListX.size();
				}
			}
		}
	}
	//std::cout << "ilosc falszywych minucji: " << IndexI.size() + IndexJ.size() << std::endl;
	//std::cout << "licznik: " << 2 * licznik << std::endl;
	for (int i = 0; i < SrcDeltaListX.size(); i++)
	{
		//for (int j = 0; j < IndexI.size(); j++)
		//{
			if ((std::binary_search(IndexI.begin(), IndexI.end(), i) == false) && (std::binary_search(IndexJ.begin(), IndexJ.end(), i) == false))
			{
				DeltaListX.push_back(SrcDeltaListX[i]);
				DeltaListY.push_back(SrcDeltaListY[i]);
				Direction.push_back(SrcDirection[i]);
			}
		//}

	}
	for (int i = 0; i < DeltaListX.size(); i++){
		
		circle(dst, Point(DeltaListX[i], DeltaListY[i]), 5, Scalar(0, 0, 255), 1, 8, 0);

		if (Direction[i] == "NW")
		{
			line(dst, Point(DeltaListX[i], DeltaListY[i]), Point(DeltaListX[i] - 3, DeltaListY[i] - 3), Scalar(0, 255, 0));
		}
		if (Direction[i] == "N")
		{
			line(dst, Point(DeltaListX[i], DeltaListY[i]), Point(DeltaListX[i], DeltaListY[i] - 3), Scalar(0, 255, 0));
		}
		if (Direction[i] == "NE")
		{
			line(dst, Point(DeltaListX[i], DeltaListY[i]), Point(DeltaListX[i] + 3, DeltaListY[i] - 3), Scalar(0, 255, 0));
		}
		if (Direction[i] == "E")
		{
			line(dst, Point(DeltaListX[i], DeltaListY[i]), Point(DeltaListX[i] + 3, DeltaListY[i]), Scalar(0, 255, 0));
		}
		if (Direction[i] == "SE")
		{
			line(dst, Point(DeltaListX[i], DeltaListY[i]), Point(DeltaListX[i] + 3, DeltaListY[i] + 3), Scalar(0, 255, 0));
		}
		if (Direction[i] == "S")
		{
			line(dst, Point(DeltaListX[i], DeltaListY[i]), Point(DeltaListX[i], DeltaListY[i] + 3), Scalar(0, 255, 0));
		}
		if (Direction[i] == "SW")
		{
			line(dst, Point(DeltaListX[i], DeltaListY[i]), Point(DeltaListX[i] - 3, DeltaListY[i] + 3), Scalar(0, 255, 0));
		}
		if (Direction[i] == "W")
		{
			line(dst, Point(DeltaListX[i], DeltaListY[i]), Point(DeltaListX[i] - 3, DeltaListY[i]), Scalar(0, 255, 0));
		}
	}
}
void FalseMinutiae::EndingDetectionCleaner(const std::vector<int> SrcEndListX, const std::vector<int> SrcEndListY, Mat& dst, std::vector<int>& EndListX, std::vector<int>& EndListY, std::vector<std::string> SrcDirection, std::vector<std::string>& Direction){
	double Length;
	cvtColor(dst, dst, COLOR_GRAY2RGB);
	//std::cout << "liczba minucji: " << SrcEndListX.size() << std::endl;
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
			if (Length < 30)
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
	//std::cout << "ilosc falszywych minucji: " << IndexI.size()+IndexJ.size()<< std::endl;
	//std::cout << "licznik: " << 2*licznik << std::endl;
	for (int i = 0; i < SrcEndListX.size(); i++)
	{
		//for (int j = 0; j < IndexI.size(); j++)
		//{
			if ((std::binary_search(IndexI.begin(), IndexI.end(), i) == false) && (std::binary_search(IndexJ.begin(), IndexJ.end(), i) == false))
			{
				EndListX.push_back(SrcEndListX[i]);
				EndListY.push_back(SrcEndListY[i]);
				Direction.push_back(SrcDirection[i]);
			}
		//}

	}
	for (int i = 0; i < EndListX.size(); i++){
		circle(dst, Point(EndListX[i], EndListY[i]), 5, Scalar(255, 0, 255), 1, 8, 0);

		if (Direction[i] == "NW")
		{
			line(dst, Point(EndListX[i], EndListY[i]), Point(EndListX[i] - 3, EndListY[i] - 3), Scalar(0, 255, 0));
		}
		if (Direction[i] == "N")
		{
			line(dst, Point(EndListX[i], EndListY[i]), Point(EndListX[i], EndListY[i] - 3), Scalar(0, 255, 0));
		}
		if (Direction[i] == "NE")
		{
			line(dst, Point(EndListX[i], EndListY[i]), Point(EndListX[i] + 3, EndListY[i] - 3), Scalar(0, 255, 0));
		}
		if (Direction[i] == "E")
		{
			line(dst, Point(EndListX[i], EndListY[i]), Point(EndListX[i] + 3, EndListY[i]), Scalar(0, 255, 0));
		}
		if (Direction[i] == "SE")
		{
			line(dst, Point(EndListX[i], EndListY[i]), Point(EndListX[i] + 3, EndListY[i] + 3), Scalar(0, 255, 0));
		}
		if (Direction[i] == "S")
		{
			line(dst, Point(EndListX[i], EndListY[i]), Point(EndListX[i], EndListY[i] + 3), Scalar(0, 255, 0));
		}
		if (Direction[i] == "SW")
		{
			line(dst, Point(EndListX[i], EndListY[i]), Point(EndListX[i] - 3, EndListY[i] + 3), Scalar(0, 255, 0));
		}
		if (Direction[i] == "W")
		{
			line(dst, Point(EndListX[i], EndListY[i]), Point(EndListX[i] - 3, EndListY[i]), Scalar(0, 255, 0));
		}
	}
}
void FalseMinutiae::FrameMark(int x0, int y0, int x1, int y1, const std::vector<int> SrcEndListX, const std::vector<int> SrcEndListY, 
	std::vector<int>& OutEndListX, std::vector<int>& OutEndListY, const std::vector<int> SrcDeltaListX, const std::vector<int> SrcDeltaListY,
	std::vector<int>& OutDeltaListX, std::vector<int>& OutDeltaListY, Mat& dst, const std::vector<std::string> SrcEndingDirection, 
	std::vector<std::string>& EndingDirection, const std::vector<std::string> SrcDeltaDirection, std::vector<std::string>& DeltaDirection){
	std::vector<int> IndexEndList;
	std::vector<int> IndexDeltaList;
	cvtColor(dst, dst, COLOR_GRAY2RGB);
	//std::cout << "SrcEndListSize: " << SrcEndListX.size();
	for (int i = 0; i < SrcEndListX.size(); i++)
	{
		//std::cout << "SrcEndList: " << SrcEndListX[i];
		//std::cout << ", " << SrcEndListY[i] << std::endl;
		if ((SrcEndListY[i] >= y0) && (SrcEndListY[i] <= y1) && (SrcEndListX[i] >= x0) && (SrcEndListX[i] <= x1)){
			OutEndListX.push_back(SrcEndListX[i]);
			OutEndListY.push_back(SrcEndListY[i]);
			EndingDirection.push_back(SrcEndingDirection[i]);
			
		}
	}
	for (int i = 0; i < SrcDeltaListX.size(); i++)
	{
		if ((SrcDeltaListY[i] >= y0) && (SrcDeltaListY[i] <= y1) && (SrcDeltaListX[i] >= x0) && (SrcDeltaListX[i] <= x1)){
			OutDeltaListX.push_back(SrcDeltaListX[i]);
			OutDeltaListY.push_back(SrcDeltaListY[i]);
			DeltaDirection.push_back(SrcDeltaDirection[i]);
		}
	}
	for (int i = 0; i < OutEndListX.size(); i++){
		circle(dst, Point(OutEndListX[i], OutEndListY[i]), 5, Scalar(255, 0, 255), 1, 8, 0);
		if (EndingDirection[i] == "NW")
		{
			line(dst, Point(OutEndListX[i], OutEndListY[i]), Point(OutEndListX[i] - 3, OutEndListY[i] - 3), Scalar(0, 255, 0));
		}
		if (EndingDirection[i] == "N")
		{
			line(dst, Point(OutEndListX[i], OutEndListY[i]), Point(OutEndListX[i], OutEndListY[i] - 3), Scalar(0, 255, 0));
		}
		if (EndingDirection[i] == "NE")
		{
			line(dst, Point(OutEndListX[i], OutEndListY[i]), Point(OutEndListX[i] + 3, OutEndListY[i] - 3), Scalar(0, 255, 0));
		}
		if (EndingDirection[i] == "E")
		{
			line(dst, Point(OutEndListX[i], OutEndListY[i]), Point(OutEndListX[i] + 3, OutEndListY[i]), Scalar(0, 255, 0));
		}
		if (EndingDirection[i] == "SE")
		{
			line(dst, Point(OutEndListX[i], OutEndListY[i]), Point(OutEndListX[i] + 3, OutEndListY[i] + 3), Scalar(0, 255, 0));
		}
		if (EndingDirection[i] == "S")
		{
			line(dst, Point(OutEndListX[i], OutEndListY[i]), Point(OutEndListX[i], OutEndListY[i] + 3), Scalar(0, 255, 0));
		}
		if (EndingDirection[i] == "SW")
		{
			line(dst, Point(OutEndListX[i], OutEndListY[i]), Point(OutEndListX[i] - 3, OutEndListY[i] + 3), Scalar(0, 255, 0));
		}
		if (EndingDirection[i] == "W")
		{
			line(dst, Point(OutEndListX[i], OutEndListY[i]), Point(OutEndListX[i] - 3, OutEndListY[i]), Scalar(0, 255, 0));
		}
	}
	for (int i = 0; i < OutDeltaListX.size(); i++){
		circle(dst, Point(OutDeltaListX[i], OutDeltaListY[i]), 5, Scalar(0, 0, 255), 1, 8, 0);
		if (DeltaDirection[i] == "NW")
		{
			line(dst, Point(OutDeltaListX[i], OutDeltaListY[i]), Point(OutDeltaListX[i] - 3, OutDeltaListY[i] - 3), Scalar(0, 255, 0));
		}
		if (DeltaDirection[i] == "N")
		{
			line(dst, Point(OutDeltaListX[i], OutDeltaListY[i]), Point(OutDeltaListX[i], OutDeltaListY[i] - 3), Scalar(0, 255, 0));
		}
		if (DeltaDirection[i] == "NE")
		{
			line(dst, Point(OutDeltaListX[i], OutDeltaListY[i]), Point(OutDeltaListX[i] + 3, OutDeltaListY[i] - 3), Scalar(0, 255, 0));
		}
		if (DeltaDirection[i] == "E")
		{
			line(dst, Point(OutDeltaListX[i], OutDeltaListY[i]), Point(OutDeltaListX[i] + 3, OutDeltaListY[i]), Scalar(0, 255, 0));
		}
		if (DeltaDirection[i] == "SE")
		{
			line(dst, Point(OutDeltaListX[i], OutDeltaListY[i]), Point(OutDeltaListX[i] + 3, OutDeltaListY[i] + 3), Scalar(0, 255, 0));
		}
		if (DeltaDirection[i] == "S")
		{
			line(dst, Point(OutDeltaListX[i], OutDeltaListY[i]), Point(OutDeltaListX[i], OutDeltaListY[i] + 3), Scalar(0, 255, 0));
		}
		if (DeltaDirection[i] == "SW")
		{
			line(dst, Point(OutDeltaListX[i], OutDeltaListY[i]), Point(OutDeltaListX[i] - 3, OutDeltaListY[i] + 3), Scalar(0, 255, 0));
		}
		if (DeltaDirection[i] == "W")
		{
			line(dst, Point(OutDeltaListX[i], OutDeltaListY[i]), Point(OutDeltaListX[i] - 3, OutDeltaListY[i]), Scalar(0, 255, 0));
		}
	}
}

FalseMinutiae::~FalseMinutiae(){
}