#include<opencv2/opencv.hpp>
#include<iostream>
#include<math.h>
#include <list>
#include <vector>
#include <map>
#include <stack>
using namespace cv;
using namespace std;
//练习2
int main()
{
	cv::Mat srcMat = imread("E:\\die_on_chip.png", 0);
	cv::Mat dspMat = imread("E:\\die_on_chip.png");
	
	cv::Mat binaryMat;
	cv::Mat labelMat;
	cv::Mat statsMat;
	cv::Mat centrMat;
	cv::Mat resultMat;
	//二值化
	cv::threshold(srcMat, binaryMat, 0, 255, THRESH_OTSU);//不取反

	//获得连通域
	Mat kernel = getStructuringElement(MORPH_RECT, Size(7, 7));//element
	morphologyEx(binaryMat, binaryMat, MORPH_OPEN, kernel, Point(-1, -1), 1);//闭运算MORPH_CLOSE、开MORPH_OPEN


	float square_area[10];
	vector<vector<Point>> contours;//寻找连通域
	vector<Vec4i> hierarchy;//2个输出结果的定义
	findContours(binaryMat, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	cout << "num=" << contours.size() << endl;
	//绘制轮廓
	for (int i = 0; i < contours.size(); i++)
	{
		//得轮廓的最小外接四边形
		RotatedRect rbox = minAreaRect(contours[i]);
		//绘制轮廓及最小外接四边形
		drawContours(dspMat, contours, i, Scalar(0,255,255), 1, 8);
		cv::Point2f vtx[4];
		rbox.points(vtx);
		for (int i = 0; i < 4; ++i)
		{
			cv::line(dspMat, vtx[i], vtx[i<3 ? i+1 : 0], cv::Scalar(0,0,255), 2, CV_AA);	
		}
	}

	imshow("srcMat", srcMat);
	imshow("dspMat", dspMat);
	imshow("binaryMat", binaryMat);
	waitKey(0);
	return 0;
}

