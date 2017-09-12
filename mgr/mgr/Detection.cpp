#include "Detection.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath> 

using namespace cv;

// overloaded constructor
Detection::Detection(){
}
void Detection::DeltaDetection2(const Mat& src, Mat& dst, std::vector<int> &DeltaListX, std::vector<int> &DeltaListY, std::vector<std::string> &Direction){
	int cols = src.cols;
	int rows = src.rows;
	std::string kierunek;
	std::vector<int> listaX;
	std::vector<int> listaY;

	float CN = 0;
	int P1, P2, P3, P4, P5, P6, P7, P8, P9;
	int suma = 0;
	int licznik = 0;
	src /= 255;
	//cvtColor(dst, dst, COLOR_GRAY2RGB);
	for (int i = 1; i < rows - 1; i++)
	{
		for (int j = 1; j < cols - 1; ++j)
		{
			if (src.at<uchar>(i, j) == 1)
			{
				
				P2 = src.at<uchar>(i - 1, j);
				P3 = src.at<uchar>(i - 1, j + 1);
				P4 = src.at<uchar>(i, j + 1);
				P5 = src.at<uchar>(i + 1, j + 1);
				P6 = src.at<uchar>(i + 1, j);
				P7 = src.at<uchar>(i + 1, j - 1);
				P8 = src.at<uchar>(i, j - 1);
				P9 = src.at<uchar>(i - 1, j - 1);

				if (P2 == 1 && P3 == 0){
					licznik++;
				}
				if (P3 == 1 && P4 == 0){
					licznik++;
				}
				if (P4 == 1 && P5 == 0){
					licznik++;
				}
				if (P5 == 1 && P6 == 0){
					licznik++;
				}
						
				if (P6 == 1 && P7 == 0){
					licznik++;
				}
						
				if (P7 == 1 && P8 == 0){
					licznik++;
				}
						
				if (P8 == 1 && P9 == 0){
					licznik++;
				}
						
				if (P9 == 1 && P2 == 0){
					licznik++;
				}
					if (licznik == 3){
						circle(dst, Point(j, i), 5, Scalar(255, 0, 255), 1, 8, 0);
						DeltaListX.push_back(j);
						DeltaListY.push_back(i);
						std::cout << "wykryto delte: pkt: " << i << " ," << j << std::endl;
					}
			
					licznik = 0;
			}
		}
	}
	src *= 255;
}


void Detection::DeltaDetectionCN(const Mat& src, Mat& dst, std::vector<int> &DeltaListX, std::vector<int> &DeltaListY, std::vector<std::string> &Direction){
	int cols = src.cols;
	int rows = src.rows;
	std::string kierunek;
	std::vector<int> listaX;
	std::vector<int> listaY;
	std::vector<int> VectorX;
	std::vector<int> VectorY;
	std::vector<std::string> VectorDirection;
	float Length, Length1, Length2;

	float CN = 0;
	int P1, P2, P3, P4, P5, P6, P7, P8, P9;
	src /= 255;
	//cvtColor(dst, dst, COLOR_GRAY2RGB);
	for (int i = 3; i < rows-3; i++)
	{
		for (int j = 3; j < cols-3; ++j)
		{
			if (src.at<uchar>(i, j) == 1)
			{
				P1 = src.at<uchar>(i, j);

				P2 = src.at<uchar>(i - 1, j);
				P3 = src.at<uchar>(i - 1, j + 1);
				P4 = src.at<uchar>(i, j + 1);
				P5 = src.at<uchar>(i + 1, j + 1);
				P6 = src.at<uchar>(i + 1, j);
				P7 = src.at<uchar>(i + 1, j - 1);
				P8 = src.at<uchar>(i, j - 1);
				P9 = src.at<uchar>(i - 1, j - 1);
				
				

				CN = 0.5*(std::abs(P2 - P3) + std::abs(P3 - P4) + std::abs(P4 - P5) +
					std::abs(P5 - P6) + std::abs(P6 - P7) + std::abs(P7 - P8) + std::abs(P8 - P9) + std::abs(P9 - P2));

				if (CN == 3){
					if ((src.at<uchar>(i - 3, j - 3) == 1) && (src.at<uchar>(i - 3, j - 2) == 0)){
						VectorX.push_back(i - 3);
						VectorY.push_back(j - 3);
						VectorDirection.push_back("NW");
					}
					if ((src.at<uchar>(i - 3, j - 2) == 1) && (src.at<uchar>(i - 3, j - 1) == 0)){
						VectorX.push_back(i - 3);
						VectorY.push_back(j - 2);
						VectorDirection.push_back("NW");
					}
					if ((src.at<uchar>(i - 3, j - 1) == 1) && (src.at<uchar>(i - 3, j) == 0)){
						VectorX.push_back(i - 3);
						VectorY.push_back(j - 1);
						VectorDirection.push_back("N");
					}
					if ((src.at<uchar>(i - 3, j) == 1) && (src.at<uchar>(i - 3, j + 1) == 0)){
						VectorX.push_back(i - 3);
						VectorY.push_back(j);
						VectorDirection.push_back("N");
					}
					if ((src.at<uchar>(i - 3, j + 1) == 1) && (src.at<uchar>(i - 3, j + 2) == 0)){
						VectorX.push_back(i - 3);
						VectorY.push_back(j + 1);
						VectorDirection.push_back("N");
					}
					if ((src.at<uchar>(i - 3, j + 2) == 1) && (src.at<uchar>(i - 3, j + 3) == 0)){
						VectorX.push_back(i - 3);
						VectorY.push_back(j + 2);
						VectorDirection.push_back("NE");
					}
					if ((src.at<uchar>(i - 3, j + 3) == 1) && (src.at<uchar>(i - 2, j + 3) == 0)){
						VectorX.push_back(i - 3);
						VectorY.push_back(j + 3);
						VectorDirection.push_back("NE");
					}
					if ((src.at<uchar>(i - 2, j + 3) == 1) && (src.at<uchar>(i - 1, j + 3) == 0)){
						VectorX.push_back(i - 2);
						VectorY.push_back(j + 3);
						VectorDirection.push_back("NE");
					}
					if ((src.at<uchar>(i - 1, j + 3) == 1) && (src.at<uchar>(i, j + 3) == 0)){
						VectorX.push_back(i - 1);
						VectorY.push_back(j + 3);
						VectorDirection.push_back("E");
					}
					if ((src.at<uchar>(i, j + 3) == 1) && (src.at<uchar>(i + 1, j + 3) == 0)){
						VectorX.push_back(i);
						VectorY.push_back(j + 3);
						VectorDirection.push_back("E");
					}
					if ((src.at<uchar>(i + 1, j + 3) == 1) && (src.at<uchar>(i + 2, j + 3) == 0)){
						VectorX.push_back(i + 1);
						VectorY.push_back(j + 3);
						VectorDirection.push_back("E");
					}
					if ((src.at<uchar>(i + 2, j + 3) == 1) && (src.at<uchar>(i + 3, j + 3) == 0)){
						VectorX.push_back(i + 2);
						VectorY.push_back(j + 3);
						VectorDirection.push_back("SE");
					}
					if ((src.at<uchar>(i + 3, j + 3) == 1) && (src.at<uchar>(i + 3, j + 2) == 0)){
						VectorX.push_back(i + 3);
						VectorY.push_back(j + 3);
						VectorDirection.push_back("SE");
					}
					if ((src.at<uchar>(i + 3, j + 2) == 1) && (src.at<uchar>(i + 3, j + 1) == 0)){
						VectorX.push_back(i + 3);
						VectorY.push_back(j + 2);
						VectorDirection.push_back("SE");
					}
					if ((src.at<uchar>(i + 3, j + 1) == 1) && (src.at<uchar>(i + 3, j) == 0)){
						VectorX.push_back(i + 3);
						VectorY.push_back(j + 1);
						VectorDirection.push_back("S");
					}
					if ((src.at<uchar>(i + 3, j) == 1) && (src.at<uchar>(i + 3, j - 1) == 0)){
						VectorX.push_back(i + 3);
						VectorY.push_back(j);
						VectorDirection.push_back("S");
					}
					if ((src.at<uchar>(i + 3, j - 1) == 1) && (src.at<uchar>(i + 3, j - 2) == 0)){
						VectorX.push_back(i + 3);
						VectorY.push_back(j - 1);
						VectorDirection.push_back("S");
					}
					if ((src.at<uchar>(i + 3, j - 2) == 1) && (src.at<uchar>(i + 3, j - 3) == 0)){
						VectorX.push_back(i + 3);
						VectorY.push_back(j - 2);
						VectorDirection.push_back("SW");
					}
					if ((src.at<uchar>(i + 3, j - 3) == 1) && (src.at<uchar>(i + 2, j - 3) == 0)){
						VectorX.push_back(i + 3);
						VectorY.push_back(j - 3);
						VectorDirection.push_back("SW");
					}
					if ((src.at<uchar>(i + 2, j - 3) == 1) && (src.at<uchar>(i + 1, j - 3) == 0)){
						VectorX.push_back(i + 2);
						VectorY.push_back(j - 3);
						VectorDirection.push_back("SW");
					}
					if ((src.at<uchar>(i + 1, j - 3) == 1) && (src.at<uchar>(i, j - 3) == 0)){
						VectorX.push_back(i + 1);
						VectorY.push_back(j - 3);
						VectorDirection.push_back("W");
					}
					if ((src.at<uchar>(i, j - 3) == 1) && (src.at<uchar>(i - 1, j - 3) == 0)){
						VectorX.push_back(i);
						VectorY.push_back(j - 3);
						VectorDirection.push_back("W");
					}
					if ((src.at<uchar>(i - 1, j - 3) == 1) && (src.at<uchar>(i - 2, j - 3) == 0)){
						VectorX.push_back(i - 1);
						VectorY.push_back(j - 3);
						VectorDirection.push_back("W");
					}
					if ((src.at<uchar>(i - 2, j - 3) == 1) && (src.at<uchar>(i - 3, j - 3) == 0)){
						VectorX.push_back(i - 2);
						VectorY.push_back(j - 3);
						VectorDirection.push_back("NW");
					}
					
					if (VectorX.size() == 3){
						circle(dst, Point(j, i), 5, Scalar(0, 0, 255), 1, 8, 0);
						DeltaListX.push_back(j);
						DeltaListY.push_back(i);
						Length = sqrt((VectorX[0] - VectorX[1])*(VectorX[0] - VectorX[1]) + (VectorY[0] - VectorY[1])*(VectorY[0] - VectorY[1]));
						Length1 = sqrt((VectorX[1] - VectorX[2])*(VectorX[1] - VectorX[2]) + (VectorY[1] - VectorY[2])*(VectorY[1] - VectorY[2]));
						Length2 = sqrt((VectorX[2] - VectorX[0])*(VectorX[2] - VectorX[0]) + (VectorY[2] - VectorY[0])*(VectorY[2] - VectorY[0]));

						if ((Length <= Length1) && (Length <= Length2))
						{
							Direction.push_back(VectorDirection[2]);
						}
						else if ((Length1 <= Length) && (Length1 <= Length2))
						{
							Direction.push_back(VectorDirection[0]);
						}
						else if ((Length2 <= Length) && (Length2 <= Length1))
						{
							Direction.push_back(VectorDirection[1]);
						}
						if (Direction[Direction.size()-1] == "NW")
						{
							line(dst, Point(j, i), Point(j - 3, i - 3), Scalar(0, 255, 0));
						}
						if (Direction[Direction.size() - 1] == "N")
						{
							line(dst, Point(j, i), Point(j, i - 3), Scalar(0, 255, 0));
						}
						if (Direction[Direction.size() - 1] == "NE")
						{
							line(dst, Point(j, i), Point(j + 3, i - 3), Scalar(0, 255, 0));
						}
						if (Direction[Direction.size() - 1] == "E")
						{
							line(dst, Point(j, i), Point(j + 3, i), Scalar(0, 255, 0));
						}
						if (Direction[Direction.size() - 1] == "SE")
						{
							line(dst, Point(j, i), Point(j + 3, i + 3), Scalar(0, 255, 0));
						}
						if (Direction[Direction.size() - 1] == "S")
						{
							line(dst, Point(j, i), Point(j, i + 3), Scalar(0, 255, 0));
						}
						if (Direction[Direction.size() - 1] == "SW")
						{
							line(dst, Point(j, i), Point(j - 3, i + 3), Scalar(0, 255, 0));
						}
						if (Direction[Direction.size() - 1] == "W")
						{
							line(dst, Point(j, i), Point(j - 3, i), Scalar(0, 255, 0));
						}
					}
					VectorX.clear();
					VectorY.clear();
					VectorDirection.clear();
					
				}

			}
		}
	}
	src *= 255;

}
void Detection::EndingDetectionCN(const Mat& src, Mat& dst, std::vector<int> &EndListX, std::vector<int> &EndListY, std::vector<std::string> &Direction){
	int cols = src.cols;
	int rows = src.rows;
	std::string kierunek;
	std::vector<int> listaX;
	std::vector<int> listaY;

	float CN = 0;
	int P1, P2, P3, P4, P5, P6, P7, P8, P9;
	src /= 255;
	cvtColor(dst, dst, COLOR_GRAY2RGB);
	for (int i = 3; i < rows - 3; i++)
	{
		for (int j = 3; j < cols - 3; ++j)
		{
			if (src.at<uchar>(i, j) == 1)
			{
				P1 = src.at<uchar>(i, j);

				P2 = src.at<uchar>(i - 1, j);
				P3 = src.at<uchar>(i - 1, j + 1);
				P4 = src.at<uchar>(i, j + 1);
				P5 = src.at<uchar>(i + 1, j + 1);
				P6 = src.at<uchar>(i + 1, j);
				P7 = src.at<uchar>(i + 1, j - 1);
				P8 = src.at<uchar>(i, j - 1);
				P9 = src.at<uchar>(i - 1, j - 1);


				CN = 0.5*(std::abs(P2 - P3) + std::abs(P3 - P4) + std::abs(P4 - P5) +
					std::abs(P5 - P6) + std::abs(P6 - P7) + std::abs(P7 - P8) + std::abs(P8 - P9) + std::abs(P9 - P2));

				if (CN == 1){
					circle(dst, Point(j, i), 5, Scalar(255, 0, 255), 1, 8, 0);
					EndListX.push_back(j);
					EndListY.push_back(i);
					//std::cout << "wykryto zakonczenie: pkt: " << i << " ," << j << std::endl;

					if ((src.at<uchar>(i - 3, j - 3) == 1) && (src.at<uchar>(i - 3, j - 2) == 0)){
						kierunek = "NW";
						line(dst, Point(j, i), Point(j - 3, i - 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i - 3, j - 2) == 1) && (src.at<uchar>(i - 3, j - 1) == 0)){
						kierunek = "NW";
						line(dst, Point(j, i), Point(j - 3, i - 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i - 3, j - 1) == 1) && (src.at<uchar>(i - 3, j) == 0)){
						kierunek = "N";
						line(dst, Point(j, i), Point(j, i - 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i - 3, j) == 1) && (src.at<uchar>(i - 3, j + 1) == 0)){
						kierunek = "N";
						line(dst, Point(j, i), Point(j, i - 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i - 3, j + 1) == 1) && (src.at<uchar>(i - 3, j + 2) == 0)){
						kierunek = "N";
						line(dst, Point(j, i), Point(j, i - 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i - 3, j + 2) == 1) && (src.at<uchar>(i - 3, j + 3) == 0)){
						kierunek = "NE";
						line(dst, Point(j, i), Point(j + 3, i - 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i - 3, j + 3) == 1) && (src.at<uchar>(i - 2, j + 3) == 0)){
						kierunek = "NE";
						line(dst, Point(j, i), Point(j + 3, i - 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i - 2, j + 3) == 1) && (src.at<uchar>(i - 1, j + 3) == 0)){
						kierunek = "NE";
						line(dst, Point(j, i), Point(j + 3, i - 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i - 1, j + 3) == 1) && (src.at<uchar>(i, j + 3) == 0)){
						kierunek = "E";
						line(dst, Point(j, i), Point(j + 3, i), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i, j + 3) == 1) && (src.at<uchar>(i + 1, j + 3) == 0)){
						kierunek = "E";
						line(dst, Point(j, i), Point(j + 3, i), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i + 1, j + 3) == 1) && (src.at<uchar>(i + 2, j + 3) == 0)){
						kierunek = "E";
						line(dst, Point(j, i), Point(j + 3, i), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i + 2, j + 3) == 1) && (src.at<uchar>(i + 3, j + 3) == 0)){
						kierunek = "SE";
						line(dst, Point(j, i), Point(j + 3, i + 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i + 3, j + 3) == 1) && (src.at<uchar>(i + 3, j + 2) == 0)){
						kierunek = "SE";
						line(dst, Point(j, i), Point(j + 3, i + 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i + 3, j + 2) == 1) && (src.at<uchar>(i + 3, j + 1) == 0)){
						kierunek = "SE";
						line(dst, Point(j, i), Point(j + 3, i + 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i + 3, j + 1) == 1) && (src.at<uchar>(i + 3, j) == 0)){
						kierunek = "S";
						line(dst, Point(j, i), Point(j, i + 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i + 3, j) == 1) && (src.at<uchar>(i + 3, j - 1) == 0)){
						kierunek = "S";
						line(dst, Point(j, i), Point(j, i + 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i + 3, j - 1) == 1) && (src.at<uchar>(i + 3, j - 2) == 0)){
						kierunek = "S";
						line(dst, Point(j, i), Point(j, i + 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i + 3, j - 2) == 1) && (src.at<uchar>(i + 3, j - 3) == 0)){
						kierunek = "SW";
						line(dst, Point(j, i), Point(j - 3, i + 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i + 3, j - 3) == 1) && (src.at<uchar>(i + 2, j - 3) == 0)){
						kierunek = "SW";
						line(dst, Point(j, i), Point(j - 3, i + 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i + 2, j - 3) == 1) && (src.at<uchar>(i + 1, j - 3) == 0)){
						kierunek = "SW";
						line(dst, Point(j, i), Point(j - 3, i + 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i + 1, j - 3) == 1) && (src.at<uchar>(i, j - 3) == 0)){
						kierunek = "W";
						line(dst, Point(j, i), Point(j - 3, i), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i, j - 3) == 1) && (src.at<uchar>(i - 1, j - 3) == 0)){
						kierunek = "W";
						line(dst, Point(j, i), Point(j - 3, i), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i - 1, j - 3) == 1) && (src.at<uchar>(i - 2, j - 3) == 0)){
						kierunek = "W";
						line(dst, Point(j, i), Point(j - 3, i), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i - 2, j - 3) == 1) && (src.at<uchar>(i - 3, j - 3) == 0)){
						kierunek = "NW";
						line(dst, Point(j, i), Point(j - 3, i - 3), Scalar(0, 255, 0));
					}
					Direction.push_back(kierunek);
				}

			}
		}
	}
	src *= 255;

}
void Detection::DeltaDetection(const Mat& src, Mat& dst, std::vector<int> &DeltaListX, std::vector<int> &DeltaListY, std::vector<std::string> &Direction){
	int cols = src.cols;
	int rows = src.rows;
	int licznik = 0;
	int kontur = 0;
	std::string kierunek;
	std::vector<int> listaX;
	std::vector<int> listaY;

	//cvtColor(dst, dst, COLOR_GRAY2RGB);


	for (int i = 10; i < rows - 10; i++)
	{
		for (int j = 55; j < cols - 20; ++j)
		{
			if (src.at<uchar>(i, j) == 255)
			{
				if (src.at<uchar>(i - 1, j - 1) == 255 && src.at<uchar>(i, j - 1) == 0){
					licznik++;
				}
				if (src.at<uchar>(i, j - 1) == 255 && src.at<uchar>(i + 1, j - 1) == 0){
					licznik++;
				}
				if (src.at<uchar>(i + 1, j - 1) == 255 && src.at<uchar>(i + 1, j) == 0){
					licznik++;
				}
				if (src.at<uchar>(i + 1, j) == 255 && src.at<uchar>(i + 1, j + 1) == 0){
					licznik++;
				}
				if (src.at<uchar>(i + 1, j + 1) == 255 && src.at<uchar>(i, j + 1) == 0){
					licznik++;
				}
				if (src.at<uchar>(i, j + 1) == 255 && src.at<uchar>(i - 1, j + 1) == 0){
					licznik++;
				}
				if (src.at<uchar>(i - 1, j + 1) == 255 && src.at<uchar>(i - 1, j) == 0){
					licznik++;
				}
				if (src.at<uchar>(i - 1, j) == 255 && src.at<uchar>(i - 1, j - 1) == 0){
					licznik++;
				}

			}
			if (licznik == 3){
				if (src.at<uchar>(i-3, j-3) == 255){
					kontur++;
					goto piksel1;
				}
				if (src.at<uchar>(i - 3, j - 2) == 255){
					kontur++;
					goto piksel2;
				}
				piksel1:if (src.at<uchar>(i - 3, j - 1) == 255){
					kontur++;
					goto piksel3;
				}
				piksel2:if (src.at<uchar>(i - 3, j) == 255){
					kontur++;
					goto piksel4;
				}
				piksel3:if (src.at<uchar>(i - 3, j+1) == 255){
					kontur++;
					goto piksel5;
				}
				piksel4:if (src.at<uchar>(i - 3, j + 2) == 255){
					kontur++;
					goto piksel6;
				}
				piksel5:if (src.at<uchar>(i - 3, j + 3) == 255){
					kontur++;
					goto piksel7;
				}
				piksel6:if (src.at<uchar>(i - 2, j + 3) == 255){
					kontur++;
					goto piksel8;
				}
				piksel7:if (src.at<uchar>(i - 1, j + 3) == 255){
					kontur++;
					goto piksel9;
				}
				piksel8:if (src.at<uchar>(i, j + 3) == 255){
					kontur++;
					goto piksel10;
				}
				piksel9:if (src.at<uchar>(i+1, j + 3) == 255){
					kontur++;
					goto piksel11;
				}
				piksel10:if (src.at<uchar>(i+2, j + 3) == 255){
					kontur++;
					goto piksel12;
				}
				piksel11:if (src.at<uchar>(i + 3, j + 3) == 255){
					kontur++;
					goto piksel13;
				}
				piksel12:if (src.at<uchar>(i + 3, j + 2) == 255){
					kontur++;
					goto piksel14;
				}
				piksel13:if (src.at<uchar>(i + 3, j + 1) == 255){
					kontur++;
					goto piksel15;
				}
				piksel14:if (src.at<uchar>(i + 3, j ) == 255){
					kontur++;
					goto piksel16;
				}
				piksel15:if (src.at<uchar>(i + 3, j - 1) == 255){
					kontur++;
					goto piksel17;
				}
				piksel16:if (src.at<uchar>(i + 3, j - 2) == 255){
					kontur++;
					goto piksel18;
				}
				piksel17:if (src.at<uchar>(i + 3, j - 3) == 255){
					kontur++;
					goto piksel19;
				}
				piksel18:if (src.at<uchar>(i + 2, j - 3) == 255){
					kontur++;
					goto piksel20;
				}
				piksel19:if (src.at<uchar>(i + 1, j - 3) == 255){
					kontur++;
					goto piksel21;
				}
				piksel20:if (src.at<uchar>(i , j - 3) == 255){
					kontur++;
					goto piksel22;
				}
				piksel21:if (src.at<uchar>(i - 1, j - 3) == 255){
					kontur++;
				}
				piksel22:if (src.at<uchar>(i - 2, j - 3) == 255){
					kontur++;
				}
				int pomoc=0;
					listaX.push_back(j);
					listaY.push_back(i);
					for (int i = 0; i < listaX.size() - 1; i++){
						if ((listaX[listaX.size() - 1] > listaX[i] - 5) && (listaX[listaX.size() - 1]<listaX[i] + 5) && (listaY[listaY.size() - 1]>listaY[i] - 5) && (listaY[listaY.size() - 1] < listaY[i] + 5))
						{
							pomoc = 1;
						}
						else
						{

						}						
					}
					if ((pomoc == 0) && (kontur>=3))
				{
					circle(dst, Point(j, i), 5, Scalar(255, 0, 255), 1, 8, 0);
					DeltaListX.push_back(j);
					DeltaListY.push_back(i);
					std::cout << "wykryto delte: pkt: " << i << " ," << j << std::endl;
				}
			}
			licznik = 0;
			kontur = 0;
		}
	}
}

void Detection::EndingDetection(const Mat& src, Mat& dst, std::vector<int> &EndListX, std::vector<int> &EndListY, std::vector<std::string> &Direction){
	int cols = src.cols;
	int rows = src.rows;
	int licznik = 0;
	int przejscie = 0;
	std::string kierunek;
	cvtColor(dst, dst, COLOR_GRAY2RGB);



	for (int i = 10; i < rows - 10; i++)
	{
		for (int j = 55; j < cols - 20; ++j)
		{
			
				if (src.at<uchar>(i, j) == 255)
				{
					if (src.at<uchar>(i - 1, j - 1) == 255)
					{
						licznik++;
					}
					if (src.at<uchar>(i - 1, j) == 255) 
					{
						licznik++;
					}
					if (src.at<uchar>(i - 1, j + 1) == 255)
					{
						licznik++;
					}
					if (src.at<uchar>(i, j + 1) == 255)
					{
						licznik++;
					}
					if (src.at<uchar>(i + 1, j + 1) == 255)
					{
						licznik++;
					}
					if (src.at<uchar>(i + 1, j) == 255)
					{
						licznik++;
					}
					if (src.at<uchar>(i + 1, j - 1) == 255)
					{
						licznik++;
					}
					if (src.at<uchar>(i, j - 1) == 255)
					{
						licznik++;
					}
				}

				if (licznik == 1){
					if ((src.at<uchar>(i - 3, j - 3) == 255) && (src.at<uchar>(i - 3, j - 2) == 0)){
						kierunek = "NW";
						line(dst, Point(j, i), Point(j - 3, i - 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i - 3, j - 2) == 255) && (src.at<uchar>(i - 3, j - 1) == 0)){
						kierunek = "NW";
						line(dst, Point(j, i), Point(j - 3, i - 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i - 3, j - 1) == 255) && (src.at<uchar>(i - 3, j) == 0)){
						kierunek = "N";
						line(dst, Point(j, i), Point(j, i-3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i - 3, j) == 255) && (src.at<uchar>(i - 3, j + 1) == 0)){
						kierunek = "N";
						line(dst, Point(j, i), Point(j, i-3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i - 3, j + 1) == 255) && (src.at<uchar>(i - 3, j + 2) == 0)){
						kierunek = "N";
						line(dst, Point(j, i), Point(j, i-3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i - 3, j + 2) == 255) && (src.at<uchar>(i - 3, j + 3) == 0)){
						kierunek = "NE";
						line(dst, Point(j, i), Point(j + 3, i - 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i - 3, j + 3) == 255) && (src.at<uchar>(i - 2, j + 3) == 0)){
						kierunek = "NE";
						line(dst, Point(j, i), Point(j + 3, i - 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i - 2, j + 3) == 255) && (src.at<uchar>(i - 1, j + 3) == 0)){
						kierunek = "NE";
						line(dst, Point(j, i), Point(j + 3, i - 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i - 1, j + 3) == 255) && (src.at<uchar>(i, j + 3) == 0)){
						kierunek = "E";
						line(dst, Point(j, i), Point(j+3, i), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i, j + 3) == 255) && (src.at<uchar>(i + 1, j + 3) == 0)){
						kierunek = "E";
						line(dst, Point(j, i), Point(j+3, i), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i + 1, j + 3) == 255) && (src.at<uchar>(i + 2, j + 3) == 0)){
						kierunek = "E";
						line(dst, Point(j, i), Point(j+3, i), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i + 2, j + 3) == 255) && (src.at<uchar>(i + 3, j + 3) == 0)){
						kierunek = "SE";
						line(dst, Point(j, i), Point(j + 3, i+3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i + 3, j + 3) == 255) && (src.at<uchar>(i + 3, j + 2) == 0)){
						kierunek = "SE";
						line(dst, Point(j, i), Point(j + 3, i + 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i + 3, j + 2) == 255) && (src.at<uchar>(i + 3, j + 1) == 0)){
						kierunek = "SE";
						line(dst, Point(j, i), Point(j + 3, i + 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i + 3, j + 1) == 255) && (src.at<uchar>(i + 3, j) == 0)){
						kierunek = "S";
						line(dst, Point(j, i), Point(j, i + 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i + 3, j) == 255) && (src.at<uchar>(i + 3, j - 1) == 0)){
						kierunek = "S";
						line(dst, Point(j, i), Point(j, i + 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i + 3, j - 1) == 255) && (src.at<uchar>(i + 3, j - 2) == 0)){
						kierunek = "S";
						line(dst, Point(j, i), Point(j, i + 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i + 3, j - 2) == 255) && (src.at<uchar>(i + 3, j - 3) == 0)){
						kierunek = "SW";
						line(dst, Point(j, i), Point(j-3, i + 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i + 3, j - 3) == 255) && (src.at<uchar>(i + 2, j - 3) == 0)){
						kierunek = "SW";
						line(dst, Point(j, i), Point(j-3, i + 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i + 2, j - 3) == 255) && (src.at<uchar>(i + 1, j - 3) == 0)){
						kierunek = "SW";
						line(dst, Point(j, i), Point(j-3, i + 3), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i + 1, j - 3) == 255) && (src.at<uchar>(i, j - 3) == 0)){
						kierunek = "W";
						line(dst, Point(j, i), Point(j - 3, i), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i, j - 3) == 255) && (src.at<uchar>(i - 1, j - 3) == 0)){
						kierunek = "W";
						line(dst, Point(j, i), Point(j - 3, i), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i - 1, j - 3) == 255) && (src.at<uchar>(i - 2, j - 3) == 0)){
						kierunek = "W";
						line(dst, Point(j, i), Point(j - 3, i), Scalar(0, 255, 0));
					}
					if ((src.at<uchar>(i - 2, j - 3) == 255) && (src.at<uchar>(i - 3, j - 3) == 0)){
						kierunek = "NW";
						line(dst, Point(j, i), Point(j - 3, i - 3), Scalar(0, 255, 0));
					}
					circle(dst, Point(j, i), 5, Scalar(0, 0, 255), 1, 8, 0);
					EndListX.push_back(j);
					EndListY.push_back(i);
					Direction.push_back(kierunek);
				}

				licznik = 0;
			
		}
	}
}

Detection::~Detection(){
}