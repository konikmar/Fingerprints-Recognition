#include "GaborFilter.h"


GaborFilter::GaborFilter()
{
	kernel_size = 31;
	sigma = 3.8;
	theta = 0.0;
	lm = 10.0;
	gm = 2.0;
	psi = 0.0;
	method = 3;
}


GaborFilter::~GaborFilter()
{
}

int GaborFilter::findPixelOrientation(Mat Gx, Mat Gy, int pixelX, int pixelY)
{
	double Vx = 0.0, Vy = 0.0;
	for (int i = pixelX - kernel_size / 2; i < (pixelX + kernel_size / 2); ++i)
	{
		for (int j = pixelY - kernel_size / 2; j < (pixelY + kernel_size / 2); ++j)
		{
			float gx = Gx.at<float>(j, i);
			float gy = Gy.at<float>(j, i);
			Vx += 2 * gx * gy;
			Vy += gx*gx * gy*gy;
		}
	}

	int orientation = 0.5*fastAtan2(Vy, Vx);
	return orientation;
}
void GaborFilter::getOrientationMap(Mat image, Mat Gx, Mat Gy, Mat &orientation_map, Mat &orientation_image)
{
	orientation_map = Mat(image.rows, image.cols, CV_32F);
	orientation_image = image.clone();
	for (int i = kernel_size / 2; i < (image.rows - kernel_size / 2); i += kernel_size)
	{
		for (int j = kernel_size / 2; j < (image.cols - kernel_size / 2); j += kernel_size)
		{
			Mat ROI = image(Rect(j - kernel_size / 2, i - kernel_size / 2, kernel_size, kernel_size));
			double theta;
			if (method == 1) theta = findPixelOrientation(Gx, Gy, j, i);
			else if (method == 2) theta = findRotation(ROI, 0);


			orientation_map.at<float>(i, j) = theta;

			int length = 15;
			Point P1(j, i);
			Point P2;
			P2.x = (int)round(P1.x + length * cos(theta * CV_PI / 180.0));
			P2.y = (int)round(P1.y + length * sin(theta * CV_PI / 180.0));
			line(orientation_image, P1, P2, Scalar(0, 0, 255), 2);
		}
	}
}
void GaborFilter::fingerprintMeanVar(Mat image, double &mean, double &var)
{
	double N = image.cols*image.rows;
	mean = 0;
	var = 0;
	for (int i = 0; i < image.rows; ++i)
	{
		for (int j = 0; j < image.cols; ++j)
		{
			uchar intensity = image.at<uchar>(i, j);
			mean += intensity;
		}
	}
	mean = mean / N;
	for (int i = 0; i < image.rows; ++i)
	{
		for (int j = 0; j < image.cols; ++j)
		{
			uchar intensity = image.at<uchar>(i, j);
			var += (intensity - mean)*(intensity - mean);
		}
	}
	var = var / N;
}
void GaborFilter::normalizeImage(Mat &image, double m0, double var0)
{
	double  m, var;

	fingerprintMeanVar(image, m, var);

	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			uchar intensity = image.at<uchar>(i, j), new_intensity = 0;
			if (intensity>m)
			{
				new_intensity = m0 + sqrt((var0*(intensity - m)*(intensity - m)) / var);
			}
			else
			{
				new_intensity = m0 - sqrt((var0*(intensity - m)*(intensity - m)) / var);
			}

			image.at<uchar>(i, j) = new_intensity;
		}
	}

}
double GaborFilter::getFrequency(Mat s_img, double rotationDegrees){
	//
	const int safe_kernel = 50; //Square Area size that is going to be rotated
	const int kernel_size = 31; //Square Area size that is going to be placed at the center of rotated safe_kernel and cropped
	//const int kernel_size = s_img.cols;
	Mat rotImg;
	cv::Point2f pc((float)s_img.cols / 2., (float)s_img.rows / 2.); //center of rotation
	cv::Mat r = cv::getRotationMatrix2D(pc, rotationDegrees, 1.0);
	cv::warpAffine(s_img, rotImg, r, s_img.size()); //rotating

#if defined(PRINT_STEPS)
	cv::namedWindow("rot", WINDOW_NORMAL);
	//cv::resizeWindow("source", 400, 400);
	imshow("rot", rotImg);
#endif

	cv::Rect myROI(10, 10, kernel_size, kernel_size);
	Mat crop_img = rotImg(myROI);

#if defined(PRINT_STEPS)
	cv::namedWindow("crop", WINDOW_NORMAL);
	cv::resizeWindow("crop", 400, 400);
	imshow("crop", crop_img);
#endif

	Mat procImg;
	if (crop_img.type() == CV_8UC3) cvtColor(crop_img, crop_img, CV_BGR2GRAY);
	cv::reduce(crop_img, procImg, 0, CV_REDUCE_SUM, CV_32S);

#if defined(PRINT_STEPS)
	cv::namedWindow("proc", WINDOW_NORMAL);
	cv::resizeWindow("proc", 300, 300);
	imshow("proc", procImg);
#endif
	double average = getAvgExtremaDistance(procImg);
	//	cout << "Average of differences = " << average;
	return average;
}

//Find an average distance in pixels between nearest lines in a given area
//Note that filtering was used to avoid counting multiple maxima in a very tiny area
double GaborFilter::getAvgExtremaDistance(Mat procImg){

	const int MY_TRESH = 1000; //Minimum value of sum of f(i,j) in a specyfic j column above which found Maximum can be accepted as valid
	vector<double> extrema;

# if defined(USE_MINIMA)
	for (int row = 1; row < procImg.cols - 1; row++){
		int oldVal = procImg.at<int>(row - 1);
		int val = procImg.at<int>(row);
		int nextVal = procImg.at<int>(row + 1);

		//Check if local minimum &&  minimum below treshold
		if (oldVal - val > 0 && nextVal - val > 0 && val < MY_TRESH){
			//extrema.push_back(row);
			cout << "Minimum" << row << endl;
			//	system("pause");
		}
	}
#endif

	// Recommended way of seaching distances
# if defined(USE_MAXIMA)
	for (int row = 1; row < procImg.cols - 1; row++){
		int oldVal = procImg.at<int>(row - 1);
		int val = procImg.at<int>(row);
		int nextVal = procImg.at<int>(row + 1);

		//Check if local maximum was foud &&  maximum is placed above treshold
		if (oldVal - val < 0 && nextVal - val < 0 && val > MY_TRESH){
			extrema.push_back((double)row);
# if defined(CMD_PRINT)
			cout << "Maximum" << row << endl;
			//	system("pause");
#endif
		}
	}
#endif

	// Distance in which extrema will be merged (2 passes for merging are applied
	const double mergeDist[] = { 2, 5 };
	vector<double> weight(extrema.size(), 1);

# if defined(CMD_PRINT)
	for (int temp = 0; temp < extrema.size(); temp++)
		cout << "Max " << temp << "Pos " << extrema.at(temp) << endl;
#endif

	// In 2 passes maxima that are located near to themselves are going to be reduced to one maxima located in between them
	// Thanks to that, several maxima placed in a very close nieghborhood will not affect the average maxima value
	for (int pass = 0; pass < 2; pass++){
		int removed = 0;
		for (int cnt = 0; (cnt) < (extrema.size() - 1); cnt++){
# if defined(CMD_PRINT)
			cout << " current difference ************** " << extrema.at(cnt + 1) - extrema.at(cnt) << endl;
			cout << " current object ************** " << extrema.at(cnt) << endl << endl;
			//	system("pause");
#endif
			//Merge when the distance between a maxima in nieghborhood is smaller that allowed
			if (extrema.at(cnt + 1) - extrema.at(cnt) <= mergeDist[pass]){
				extrema.at(cnt) = (extrema.at(cnt) * weight.at(cnt) + extrema.at(cnt + 1) * weight.at(cnt + 1)) / (weight.at(cnt) + weight.at(cnt + 1));
				weight.at(cnt) += weight.at(cnt + 1);
				extrema.erase(extrema.begin() + cnt + 1); //earse maximum that was merged (it means that it influenced a location of another one and is not needed anymore)
				weight.erase(weight.begin() + cnt + 1); //earse wieght related with earsed maximum
				removed++;
# if defined(CMD_PRINT)
				cout << " extrema pass" << pass << " el " << cnt << " val " << extrema.at(cnt) << " enchancement" << " weight " << weight.at(cnt) << endl;
#endif
			}
		}
# if defined(CMD_PRINT)
		for (int temp = 0; temp < extrema.size(); temp++)
			cout << "Max " << temp << "Pos " << extrema.at(temp) << endl;
#endif
	}
	//When extrema are already found and filtered, calculate distances between neighbours
	vector<double> distances;
	for (int iter = 0; iter < extrema.size() - 1; iter++){
		distances.push_back(extrema.at(iter + 1) - extrema.at(iter));
# if defined(CMD_PRINT)
		cout << "Difference beetwen extremum" << iter << " and " << iter + 1 << " = " << distances.at(iter) << endl;
#endif
	}
	double average;

	//An average distance is calculated and returned
	return average = std::accumulate(distances.begin(), distances.end(), 0.0) / distances.size();
}
void GaborFilter::gradientImage(Mat image, Mat &Gx, Mat &Gy)
{

	Gx = Mat(Size(image.rows, image.cols), CV_32F);
	Gy = Mat(image.rows, image.cols, CV_32F);

	Sobel(image, Gx, CV_32FC1, 1, 0, 7);
	Sobel(image, Gy, CV_32FC1, 0, 1, 7);

}

void GaborFilter::getOrientationMapGradient(Mat angle_matrix, Mat magnitude_matrix, int block_size, Mat &orientation_map, Mat &orientation_image)
{
	orientation_map = Mat(angle_matrix.rows, angle_matrix.cols, CV_32F);
	for (int i = block_size / 2; i< (angle_matrix.rows - block_size / 2); i++)
	{
		for (int j = block_size / 2; j< (angle_matrix.cols - block_size / 2); j++)
		{

			//float angle = findBlockAngle(magnitude_matrix(Rect(j, i, block_size, block_size)), angle_matrix(Rect(j, i, block_size, block_size)));
			float angle = findBlockAngle(magnitude_matrix(Rect(j - block_size / 2, i - block_size / 2, block_size, block_size)), angle_matrix(Rect(j - block_size / 2, i - block_size / 2, block_size, block_size)));
			orientation_map.at<float>(i, j) = angle;

			float dx = (float)block_size / 2.0*cos(angle);
			float dy = (float)block_size / 2.0*sin(angle);
			int x = j;
			int y = i;

			if ((!(i % 10)) && (!(j % 10))) line(orientation_image, cv::Point(x, y), cv::Point(x + dx, y + dy), Scalar(0, 0, 255), 1, CV_AA);
		}
	}
}
float GaborFilter::findBlockAngle(Mat& magnitude_matrix, Mat& angle_matrix)
{
	float sum = 0;
	float n = 0;
	float average;

	for (int i = 0; i< magnitude_matrix.rows; ++i)
	{
		for (int j = 0; j< magnitude_matrix.cols; ++j)
		{
			sum += angle_matrix.at<float>(i, j)*magnitude_matrix.at<float>(i, j);
			n += magnitude_matrix.at<float>(i, j);
		}
	}
	average = sum / n;
	return average;
}

void GaborFilter::enhanceImage(Mat image_src, Mat &image_dst, Mat &orientation_map, Mat &orientation_image)
{
	Mat part_image;

	image_dst = Mat(image_src.rows, image_src.cols, CV_8U);

	for (int i = kernel_size / 2; i < image_src.rows - kernel_size / 2; i += kernel_size)
	{
		for (int j = kernel_size / 2; j <image_src.cols - kernel_size / 2; j += kernel_size)
		{
			Mat ROI = Mat(image_src, Rect(j - kernel_size / 2, i - kernel_size / 2, kernel_size, kernel_size));
			theta = orientation_map.at<float>(i, j);

			if ((i>50) && (j>50) && ((image_src.cols - j) > 0) && ((image_src.rows - i) > 50))
			{
				Mat ROI_frequency = Mat(image_src, Rect(image_src.cols / 2 - 25, image_src.rows / 2 - 25, 50, 50));
				lm = getFrequency(ROI_frequency, theta);
			}
			Mat rotated = 255 * Mat::ones(ROI.rows, ROI.cols, CV_8U);
			rotateImage(ROI, rotated, -theta);
			if (theta = 90) theta -= 90;
			else theta += 90;
			Mat kernel = getGaborKernel(Size(31, 31), sigma, CV_PI*theta / 180, lm, gm, psi);
			filter2D(ROI, part_image, image_src.depth(), kernel);
			part_image.copyTo(image_dst(Rect(j - kernel_size / 2, i - kernel_size / 2, kernel_size, kernel_size)));
		}
	}
}

void GaborFilter::rotateImage(Mat src, Mat dst, double alpha)
{
	Point2f center = Point2f(src.cols / 2.0F, src.rows / 2.0F);
	Mat R = getRotationMatrix2D(center, alpha, 1.0);
	warpAffine(src, dst, R, Size(src.size()), INTER_LINEAR, BORDER_TRANSPARENT);

}

//funkcje do wyznaczania k¹ta z wykorzystaniem transformaty Hougha

vector<float> GaborFilter::calcAVGandSTD(vector<float> v)
{
	double sum = std::accumulate(v.begin(), v.end(), 0.0);
	double mean = sum / v.size();

	std::vector<double> diff(v.size());
	std::transform(v.begin(), v.end(), diff.begin(),
		std::bind2nd(std::minus<double>(), mean));
	double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
	double stdev = std::sqrt(sq_sum / v.size());

	vector<float> result = { (float)stdev, (float)mean };
	return result;
}

//Set "units" to 1 if input is described in rad. Else set "units" to 0 
float GaborFilter::getLineAngle(float angle, int units){

	if (units == 1) angle = angle * 180 / CV_PI; //convert to deg when input is rad
	else if (units == 0) angle = angle; //do not convert when input is deg

	float line_Angle;
	if (angle > 180) angle = angle - 180; //change angle range from 0-360 to 0-180;

	//change angle so that it's value is growing in counterclockwise direction and a line parallel to X axis has angle of 0 deg
	if (angle < 90) line_Angle = 90 - angle;
	else if (angle >= 90 && angle <= 180) line_Angle = 270 - angle;
	return line_Angle;
}

//get the average rotation angle value from a group of lines found in a given picture
//The function handles special cases, it means: counting AVG angle for 2 similar looking lines with angles such as: 179deg, 3deg will return result angle 2deg whis is valid, not something around 90deg (according to founding mean values only without handling special cases)
float GaborFilter::findRotation(Mat src, bool show_steps)
{
	cv::cvtColor(src, src, CV_RGB2GRAY);
	threshold(src, src, 50, 255, THRESH_BINARY_INV);
	//cv::bitwise_not(img, img);

	// Canny to find edges
	Mat canny_cont;
	int lowThreshold = 8, kernel_size = 3, ratio = 3;
	Canny(src, src, lowThreshold, (lowThreshold)*(ratio), kernel_size);

	if (show_steps)
	{
		// Create matix to display colored lines
		cv::namedWindow("Window1", 0);
		cv::imshow("Window1", src);
		cv::waitKey(0);
	}

	//find best matching lines
	vector<Point2f> lines;
	int pix_in_line_thresh = 20; // Min number of points that are members of valid line. Increase this value if to many false positive lines found.  
	HoughLines(src, lines, 1, CV_PI / 180, pix_in_line_thresh);

	if (show_steps) cout << "Number of lines found = " << lines.size() << endl;

	//Method good for counting mean angle for lines with almoust VERTICAL orientation
	vector<float> line_angles;
	for (size_t i = 0; i < lines.size(); i++)
	{
		float line_Angle = getLineAngle(lines[i].y, 1);
		if (show_steps) cout << "Line angle = " << line_Angle << endl;
		line_angles.push_back(line_Angle);
	}
	vector<float> result_lines = calcAVGandSTD(line_angles);

	if (show_steps) cout << "*** STD deviation in method 1 (based on lines) = " << result_lines.at(0) << endl;

	//Method good for counting mean angle for lines with almoust HORIZONTAL orientation
	vector<float> line_normals;
	for (size_t i = 0; i < lines.size(); i++)
	{
		float line_Normal = lines[i].y * 180 / CV_PI;
		if (line_Normal > 180) line_Normal -= 180;
		if (show_steps) cout << "Line normal = " << line_Normal << endl;
		line_normals.push_back(line_Normal);
	}
	vector<float> result_normals = calcAVGandSTD(line_normals);

	if (show_steps) cout << "*** STD deviation in method 1 (based on normals) = " << result_normals.at(0) << endl;


	//High standard deviation if exists, it indicates an invalid result of one of a two used methods.
	//An average angular vaule associated with SMALLER standard deviation will be a valid result


	float valid_angle;
	if (lines.size() > 0)
	{
		if (result_lines.at(0) < result_normals.at(0)) // Checking which standard deviation is smaller
			valid_angle = result_lines.at(1);
		else
			valid_angle = getLineAngle(result_normals.at(1), 0); //Converting a mean of angular values of normals to an average angular line value
	}
	else
	{
		valid_angle = 0; //wrong angle - too few lines - information not to perform Gabor filterization
	}

	if (show_steps)
	{
		cout << endl << "************************" << endl;
		cout << "Valid angle (obtained from valid method) is: " << valid_angle << endl; // Prints final valid result
		cout << "************************" << endl << endl;

		//For showing intermediate steps including printing separate lines on a copied image
		Mat color_dst;
		cvtColor(src, color_dst, CV_GRAY2BGR); //Create a copy of image ready to be overwriten with colors for demonstration

		cv::namedWindow("Window2", 0);
		//Drawing lines in red color on a reference window
		for (size_t i = 0; i < lines.size(); i++)
		{
			float rho = lines[i].x;
			float theta = lines[i].y;
			double a = cos(theta), b = sin(theta);
			double x0 = a*rho, y0 = b*rho;
			Point pt1(cvRound(x0 + 1000 * (-b)),
				cvRound(y0 + 1000 * (a)));
			Point pt2(cvRound(x0 - 1000 * (-b)),
				cvRound(y0 - 1000 * (a)));
			line(color_dst, pt1, pt2, Scalar(0, 0, 255), 1, 8);
			cv::imshow("Window2", color_dst);

			float currentAngle = lines[i].y * 180 / CV_PI;
			cout << "current line normal angle = " << currentAngle << endl;

			if (currentAngle > 180) currentAngle -= 180;
			if (currentAngle < 90) currentAngle = 90 - currentAngle;
			else if (currentAngle >= 90 && currentAngle <= 180) currentAngle = 270 - currentAngle;
			cout << "current line angle = " << currentAngle << endl;
			waitKey(0);
		}
	}

	//Angle that is an average angle of all lines found in a region
	return valid_angle;
}