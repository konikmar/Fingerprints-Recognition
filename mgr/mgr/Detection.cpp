#include "Detection.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath> 

using namespace cv;

// overloaded constructor
Detection::Detection(){
}
void Detection::DeltaDetectionCN(const Mat& src, Mat& dst, std::vector<int> &DeltaListX, std::vector<int> &DeltaListY, std::vector<std::string> &Direction){
	int cols = src.cols;
	int rows = src.rows;
	std::string kierunek;
	std::vector<int> listaX;
	std::vector<int> listaY;

	float CN = 0;
	int P1, P2, P3, P4, P5, P6, P7, P8, P9;
	src /= 255;
	//cvtColor(dst, dst, COLOR_GRAY2RGB);
	for (int i = 1; i < rows-1; i++)
	{
		for (int j = 1; j < cols-1; ++j)
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
					circle(dst, Point(j, i), 5, Scalar(0, 0, 255), 1, 8, 0);
					DeltaListX.push_back(i);
					DeltaListY.push_back(j);
					std::cout << "wykryto delte: pkt: " << i << " ," << j << std::endl;
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
	for (int i = 1; i < rows - 1; i++)
	{
		for (int j = 1; j < cols - 1; ++j)
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
					EndListX.push_back(i);
					EndListY.push_back(j);
					std::cout << "wykryto zakonczenie: pkt: " << i << " ," << j << std::endl;
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
					DeltaListX.push_back(i);
					DeltaListY.push_back(j);
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
					circle(dst, Point(j, i), 5, Scalar(255, 0, 255), 1, 8, 0);
					EndListX.push_back(i);
					EndListY.push_back(j);
					Direction.push_back(kierunek);
				}

				licznik = 0;
			
		}
	}
}

Detection::~Detection(){
}