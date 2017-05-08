#pragma once

#include "opencv2/opencv.hpp"
#include <numeric>
#include <functional>

using namespace cv;
using namespace std;

//#define PRINT_STEPS 1
//#define USE_MINIMA
#define USE_MAXIMA
//#define CMD_PRINT

class GaborFilter
{
	/*!
	*Implementation of Gabor Filter for fingerprint image enhancement. Supports 3 different methods of
	*ridge orientation estimation.
	*/
	int kernel_size; ///size of filter's kernel


	double sigma; ///parameter sigma (see Gabor Filter definition)
	double theta; ///parameter theta - ridge orientation
	double lm;	  ///parameter lambda - distance between two valleys 
	double gm;	  ///parameter gamma (see Gabor Filter definition)
	double psi;	  ///parameter psi (in this implementation should be 0)

	//preprocessing

	void fingerprintMeanVar(Mat image, double &mean, double &var);	///computes mean and variance from image
	void normalizeImage(Mat &image, double m0 = 100, double var0 = 100.0);	///image normalization

	int findPixelOrientation(Mat Gx, Mat Gy, int pixelX, int pixelY);	///estimating ridge orientation (method 1)


	double getFrequency(Mat, double);
	double getAvgExtremaDistance(Mat);

	void rotateImage(Mat src, Mat dst, double alpha);	///image rotation with angle given in alpha


	vector<float> calcAVGandSTD(vector<float> v);
	float getLineAngle(float angle, int units);
	float findRotation(Mat src, bool show_steps);

	float findBlockAngle(Mat& magnitude_matrix, Mat& angle_matrix);	///estimation of angle for one block (gradient method -> 3)


public:

	int method; /// method of orientation map calculation: 1 - Z. Yuanyuan publication, 2 - Hough Transform, 3 - another gradient method  

	void gradientImage(Mat image, Mat &Gx, Mat &Gy); ///calculates gradient image using Sobel filter
	void getOrientationMap(Mat image, Mat Gx, Mat Gy, Mat &orientation_map, Mat &orientation_image); ///calculation of orientation map using method 1
	void getOrientationMapGradient(Mat angle_matrix, Mat magnitude_matrix, int block_size, Mat &orientation_map, Mat &orientation_image);	///calculation of orientation map using method 3
	void enhanceImage(Mat image_src, Mat &image_dst, Mat &orientation_map, Mat &orientation_image);	///image enhancement using Gabor Filter with selected parametres
	GaborFilter();
	~GaborFilter();
};

