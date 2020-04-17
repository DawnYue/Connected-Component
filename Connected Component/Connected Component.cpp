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
	//cv::Mat srcMat = imread("E:\\rim.png", 0);
	cv::Mat srcMat = imread("E:\\die_on_chip.png", 0);
	//cv::Mat srcMat = imread("E:\\topic1.jpg", 0);

	//cv::Mat dspMat = imread("E:\\rim.png");
	cv::Mat dspMat = imread("E:\\die_on_chip.png");
	//cv::Mat dspMat = imread("E:\\topic1.jpg");

	cv::Mat binaryMat;
	cv::Mat labelMat;
	cv::Mat statsMat;
	cv::Mat centrMat;
	cv::Mat resultMat;
	//二值化
	cv::threshold(srcMat, binaryMat, 0, 255, THRESH_OTSU);//不取反
	cv::Mat inversedMat = ~srcMat;
	//cv::threshold(inversedMat, binaryMat, 150, 255, THRESH_OTSU);
	//获得连通域

	Mat kernel = getStructuringElement(MORPH_RECT, Size(7, 7));//element
	morphologyEx(binaryMat, binaryMat, MORPH_OPEN, kernel, Point(-1, -1), 1);//闭运算MORPH_CLOSE、开MORPH_OPEN


	float square_area[10];
	vector<vector<Point>> contours;//寻找连通域
	vector<Vec4i> hierarchy;//2个输出结果的定义
	findContours(binaryMat, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);// hierarchy,
	cout << "num=" << contours.size() << endl;
	//绘制轮廓
	for (int i = 0; i < contours.size(); i++)
	{
		//char num[10];
		//sprintf_s(num, "%d", i);

		

		//得轮廓的最小外接四边形
		RotatedRect rbox = minAreaRect(contours[i]);
		//绘制轮廓及最小外接四边形
		drawContours(dspMat, contours, i, Scalar(0,255,255), 1, 8);
		cv::Point2f vtx[4];
		rbox.points(vtx);
		//cv::putText(dspMat, num, vtx[0], font_face, 1, CV_RGB(255, 0, 0), thickness, 8, 0);//
		for (int i = 0; i < 4; ++i)
		{
			cv::line(dspMat, vtx[i], vtx[i<3 ? i+1 : 0], cv::Scalar(0,0,255), 2, CV_AA);
		
		}
		 //计算外接矩形面积
		//两点间距离公式求边长
		float Y = sqrt((vtx[0].y - vtx[1].y) * (vtx[0].y - vtx[1].y) + (vtx[0].x - vtx[1].x) * (vtx[0].x - vtx[1].x));
		float X = sqrt((vtx[1].y - vtx[2].y) * (vtx[1].y - vtx[2].y) + (vtx[1].x - vtx[2].x) * (vtx[1].x - vtx[2].x));
		square_area[i] = X * Y;
		cout << "num=" << i << "矩形面积square area=" << square_area[i] << endl;
	}

	int font_face = cv::FONT_HERSHEY_COMPLEX;
	double font_scale = 1.5;
	int thickness = 1;
	int baseline;
	

	int num_area[10];


	int nComp = cv::connectedComponentsWithStats(binaryMat, 
		labelMat,
		statsMat,
		centrMat, 
		8, 
		CV_32S);

	cout << "the total of connected Components = " << nComp - 1 << endl;//-1,nComp包括背景
	resultMat = cv::Mat::zeros(srcMat.size(), CV_8UC3);	//显示用图像
	std::vector<cv::Vec3b> colors(nComp);
	colors[0] = cv::Vec3b(0, 0, 0);//背景黑色

//绘制bounding box
	for (int i = 1; i < nComp; i++)
	{
		Rect bndbox;
		//左上角坐标
		bndbox.x = statsMat.at<int>(i, 0);
		bndbox.y = statsMat.at<int>(i, 1);
		//宽和长
		bndbox.width = statsMat.at<int>(i, 2);
		bndbox.height = statsMat.at<int>(i, 3);
		//绘制
		rectangle(resultMat, bndbox, CV_RGB(255, 255, 255), 1, 8, 0);
	}

/*
	//绘制bounding box
	for (int i = 1; i < nComp; i++)
	{
		char num[10];
		sprintf_s(num, "%d", i);
		//连通域编号
		int x = statsMat.at<int>(i, 0);
		int y = statsMat.at<int>(i, 1);
		cv::putText(dspMat, num, Point(x, y), font_face, 1, cv::Scalar(255, 0, 0), thickness, 8, 0);
		//依次获取每个连通域的面积
		num_area[i] = statsMat.at<int>(i, 4);
		cout << "mum=" << i << " num_area=" << num_area[i] << endl;
	}
	*/
	//计算矩形度
	int h = 0;
	for (int k = 0; k < contours.size(); k++)
	{	
		float rectangularity = (float)num_area[12 - k] / (float)square_area[k];
		cout << "num=" << k << "矩形度=" << rectangularity << endl;
	}
	imshow("srcMat", srcMat);
	imshow("dspMat", dspMat);
	imshow("binaryMat", binaryMat);
	waitKey(0);
	return 0;
}

