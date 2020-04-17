#include<opencv2/opencv.hpp>
#include<iostream>
#include<math.h>
#include <list>
#include <vector>
#include <map>
#include <stack>
using namespace cv;
using namespace std;
//练习1
int main()
{
	cv::Mat srcMat = imread("E:\\rim.png", 0);
	cv::Mat dspMat = imread("E:\\rim.png");
	cv::Mat binaryMat;
	
	//二值化
	cv::Mat inversedMat = ~srcMat;
	cv::threshold(inversedMat, binaryMat, 150, 255, THRESH_OTSU);
	//获得连通域

	Mat kernel = getStructuringElement(MORPH_RECT, Size(11, 11));//element
	morphologyEx(binaryMat, binaryMat, MORPH_CLOSE, kernel, Point(-1, -1), 1);//闭运算MORPH_CLOSE、开MORPH_OPEN
	vector<vector<Point>> contours;//寻找连通域
	vector<Vec4i> hierarchy;//2个输出结果的定义
	findContours(binaryMat, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	float k[10];
	//绘制轮廓
	for (int i = 0; i < contours.size(); i++)
	{
		//得轮廓的最小外接四边形
		RotatedRect rbox = minAreaRect(contours[i]);
		
		drawContours(dspMat, contours, i, Scalar(0,255,255), 1, 8);
		cv::Point2f vtx[4];
		rbox.points(vtx);
		
		 //计算宽长比
		//两点间距离公式求边长
		float Y = sqrt((vtx[0].y - vtx[1].y) * (vtx[0].y - vtx[1].y) + (vtx[0].x - vtx[1].x) * (vtx[0].x - vtx[1].x));
		float X = sqrt((vtx[1].y - vtx[2].y) * (vtx[1].y - vtx[2].y) + (vtx[1].x - vtx[2].x) * (vtx[1].x - vtx[2].x));
		k[i] = X / Y;
		//绘制轮廓及最小外接四边形
		if (k[i] >= 0.9 ){
			if (k[i] <= 1.1) {
				for (int i = 0; i < 4; ++i)
				{
					cv::line(dspMat, vtx[i], vtx[i < 3 ? i + 1 : 0], cv::Scalar(0, 0, 255), 2, CV_AA);
				}
			}
}
	}

	imshow("srcMat", srcMat);
	imshow("dspMat", dspMat);
	imshow("binaryMat", binaryMat);
	waitKey(0);
	return 0;
}

