// diplom_openCV.0.1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Lymphocytes.h"
using namespace Lympho;

IplImage* image = 0;
IplImage* dst = 0;

// ��� �������� ������� HSV
IplImage* hsv = 0;
IplImage* h_plane = 0;
IplImage* s_plane = 0;
IplImage* v_plane = 0;
// ��� �������� ������� HSV ����� ��������������
IplImage* h_range = 0;
IplImage* s_range = 0;
IplImage* v_range = 0;
// ��� �������� ��������� ��������
IplImage* hsv_and = 0;

int Hmin = 0;
int Hmax = 256;

int Smin = 0;
int Smax = 256;

int Vmin = 0;
int Vmax = 256;

int HSVmax = 256;

Mat src; Mat src_gray;
int thresh = 100;
int maxThresh = 255;
RNG rng(12345);

//
// �������-����������� ���������
//
void myTrackbarHmin(int pos) {
	Hmin = pos;
	cvInRangeS(h_plane, cvScalar(Hmin), cvScalar(Hmax), h_range);
}

void myTrackbarHmax(int pos) {
	Hmax = pos;
	cvInRangeS(h_plane, cvScalar(Hmin), cvScalar(Hmax), h_range);
}

void myTrackbarSmin(int pos) {
	Smin = pos;
	cvInRangeS(s_plane, cvScalar(Smin), cvScalar(Smax), s_range);
}

void myTrackbarSmax(int pos) {
	Smax = pos;
	cvInRangeS(s_plane, cvScalar(Smin), cvScalar(Smax), s_range);
}

void myTrackbarVmin(int pos) {
	Vmin = pos;
	cvInRangeS(v_plane, cvScalar(Vmin), cvScalar(Vmax), v_range);
}

void myTrackbarVmax(int pos) {
	Vmax = pos;
	cvInRangeS(v_plane, cvScalar(Vmin), cvScalar(Vmax), v_range);
}

void hsvFinder(int argc, char* argv[]){
	// ��� �������� ������� ������ ����������
	char* filename = argc == 2 ? argv[1] : "111.jpg";
	// �������� ��������
	image = cvLoadImage(filename, 1);

	printf("[i] image: %s\n", filename);
	assert(image != 0);

	// ������ ��������
	hsv = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 3);
	h_plane = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	s_plane = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	v_plane = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	h_range = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	s_range = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	v_range = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	hsv_and = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	//  ������������ � HSV 
	cvCvtColor(image, hsv, CV_BGR2HSV);
	// ��������� �� �������� ������
	cvSplit(hsv, h_plane, s_plane, v_plane, 0);

	//
	// ���������� ����������� � ������������ ��������
	// � ������� HSV
	double framemin = 0;
	double framemax = 0;

	cvMinMaxLoc(h_plane, &framemin, &framemax);
	printf("[H] %f x %f\n", framemin, framemax);
	Hmin = framemin;
	Hmax = framemax;
	cvMinMaxLoc(s_plane, &framemin, &framemax);
	printf("[S] %f x %f\n", framemin, framemax);
	Smin = framemin;
	Smax = framemax;
	cvMinMaxLoc(v_plane, &framemin, &framemax);
	printf("[V] %f x %f\n", framemin, framemax);
	Vmin = framemin;
	Vmax = framemax;

	// ���� ��� ����������� ��������
	cvNamedWindow("original", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("H", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("S", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("V", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("H range", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("S range", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("V range", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("hsv and", CV_WINDOW_AUTOSIZE);

	cvCreateTrackbar("Hmin", "H range", &Hmin, HSVmax, myTrackbarHmin);
	cvCreateTrackbar("Hmax", "H range", &Hmax, HSVmax, myTrackbarHmax);
	cvCreateTrackbar("Smin", "S range", &Smin, HSVmax, myTrackbarSmin);
	cvCreateTrackbar("Smax", "S range", &Smax, HSVmax, myTrackbarSmax);
	cvCreateTrackbar("Vmin", "V range", &Vmin, HSVmax, myTrackbarVmin);
	cvCreateTrackbar("Vmax", "V range", &Vmax, HSVmax, myTrackbarVmax);

	//
	// ��������� ���� �� �������� �����
	//
	if (image->width <1920 / 4 && image->height<1080 / 2) {
		cvMoveWindow("original", 0, 0);
		cvMoveWindow("H", image->width + 10, 0);
		cvMoveWindow("S", (image->width + 10) * 2, 0);
		cvMoveWindow("V", (image->width + 10) * 3, 0);
		cvMoveWindow("hsv and", 0, image->height + 30);
		cvMoveWindow("H range", image->width + 10, image->height + 30);
		cvMoveWindow("S range", (image->width + 10) * 2, image->height + 30);
		cvMoveWindow("V range", (image->width + 10) * 3, image->height + 30);
	}

	while (true) {

		// ���������� ��������
		cvShowImage("original", image);

		cvShowImage("H", h_plane);
		cvShowImage("S", s_plane);
		cvShowImage("V", v_plane);

		cvShowImage("H range", h_range);
		cvShowImage("S range", s_range);
		cvShowImage("V range", v_range);

		// ���������� 
		cvAnd(h_range, s_range, hsv_and);
		cvAnd(hsv_and, v_range, hsv_and);

		cvShowImage("hsv and", hsv_and);

		char c = cvWaitKey(33);
		if (c == 27) { // ���� ������ ESC - �������
			break;
		}
	}
	printf("\n[i] Results:\n");
	printf("[H] %d x %d\n", Hmin, Hmax);
	printf("[S] %d x %d\n", Smin, Smax);
	printf("[V] %d x %d\n", Vmin, Vmax);


	cvSaveImage("6.jpg", hsv_and);

	// ����������� �������
	cvReleaseImage(&image);
	cvReleaseImage(&hsv);
	cvReleaseImage(&h_plane);
	cvReleaseImage(&s_plane);
	cvReleaseImage(&v_plane);
	cvReleaseImage(&h_range);
	cvReleaseImage(&s_range);
	cvReleaseImage(&v_range);
	cvReleaseImage(&hsv_and);
	// ������� ����
	cvDestroyAllWindows();
}

Mat hsvImage;
Mat origImage;



void findingContours(const Mat& src) {
	Cell* cell = 0;
	vector<Cell*> lymphocyts;

	Mat thresholdOutput;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using Threshold
	threshold(src, thresholdOutput, thresh, maxThresh, THRESH_BINARY);
	/// Find contours
	findContours(src, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Approximate contours to polygons + get bounding rects and circles
	vector<vector<Point> > contoursPoly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contoursPoly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contoursPoly[i]));
		minEnclosingCircle((Mat)contoursPoly[i], center[i], radius[i]);
	}

	int counter = 0;

	Mat drawing = Mat::zeros(thresholdOutput.size(), CV_8UC3);

	for (int i = 0; i< contours.size(); i++)
	{
		double area = fabs(contourArea(contours[i]));
		double perim = arcLength(contours[i], true);

		if (area > 10) {
			Point coordinateOfContour = center[i];
			cell = new Lymphocytes(coordinateOfContour.x, coordinateOfContour.y);
			lymphocyts.push_back(cell);
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));															  // �������� ������
			drawContours(drawing, contoursPoly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
			rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);

			circle(origImage, Point(cell[counter].getX(), cell[counter].getY()), 40, color, 2, 8, 0);

			cout << endl << "contour with coordinates: x = " << cell[counter].getX() << " y = " << cell[counter].getY();
			counter++;
		}
	}
	
	/// Show in a window
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
	namedWindow("Contours1", CV_WINDOW_AUTOSIZE);
	imshow("Contours1", origImage);
	
	waitKey(0);
	delete cell;
	for (register int i = 0; i < lymphocyts.size(); i++) {
		delete(lymphocyts[i]);
	}
	lymphocyts.clear();
}

void myHSV(const Mat& src) {
	
	Mat tempHsvImage;
	cvtColor(src, hsvImage, COLOR_BGR2HSV);
			
	Mat upperRange;
	Mat lowerRange;

	inRange(hsvImage, cv::Scalar(121, 179, 195), cv::Scalar(141, 255, 229), upperRange);
	//inRange(hsvImage, cv::Scalar(100, 135, 100), cv::Scalar(255, 255, 255), lowerRange);
	
	Mat lympho;
	
	//addWeighted(lowerRange, 1.0, upperRange, 1.0, 0.0, lympho);
	//GaussianBlur(upperRange, upperRange, cv::Size(9, 9), 3, 3);
	
	namedWindow("hsv image", cv::WINDOW_AUTOSIZE);
	imshow("hsv image", upperRange);
	
	
	hsvImage = upperRange;
	
	waitKey(0);
	//imwrite("11.jpg", lower_red_hue_range);
}


int main(int argc, char* argv[])
{
	char* filename = argc == 2 ? argv[1] : "111.jpg";

	Mat srcImage = imread(filename);
	origImage = srcImage.clone();

	namedWindow("src", cv::WINDOW_AUTOSIZE);
	imshow("src", srcImage);

	myHSV(srcImage);
	findingContours(hsvImage);
	
	//hsvFinder(2,argv);
	return 0;
}