#include<opencv2/opencv.hpp>
#include<iostream>
#include<math.h>
#include <list>
#include <vector>
#include <map>
#include <stack>
using namespace cv;
using namespace std;
//课前准备
int main()
{
	cv::Mat srcMat = imread("E:\\coin.png", 0);
	cv::Mat dspMat = imread("E:\\coin.png");
	cv::Mat binaryMat;
	cv::Mat labelMat;
	cv::Mat statsMat;
	cv::Mat centrMat;
	cv::Mat resultMat;
	//二值化
	cv::Mat inversedMat = ~srcMat;
	/*	
法一	cv::Mat inversedMat = 255 - srcMat;
法二	// 对图像进行所有像素用 （255- 像素值）
	Mat invertImage;
	srcMat.copyTo(invertImage);
	// 获取图像宽、高
	int channels = srcMat.channels();
	int rows = srcMat.rows; //高---行
	int col = srcMat.cols;//宽---列
	int cols = srcMat.cols * channels;
	if (srcMat.isContinuous()) {
		cols *= rows;
		rows = 1;
	}
	// 每个像素点的每个通道255取反（0-255（黑-白））
	uchar* p1;
	uchar* p2;
	for (int row = 0; row < rows; row++) {
		p1 = srcMat.ptr<uchar>(row);// 获取像素指针
		p2 = invertImage.ptr<uchar>(row);
		for (int col = 0; col < cols; col++) {
			*p2 = 255 - *p1; // 取反
			p2++;
			p1++;
		}
	}	*/
	//cv::threshold(srcMat, binaryMat, 0, 255, THRESH_OTSU);//不取反
	cv::threshold(inversedMat, binaryMat, 150, 255, THRESH_OTSU);

	//获得连通域
	Mat kernel = getStructuringElement(MORPH_RECT, Size(7, 7));
	morphologyEx(binaryMat, binaryMat, MORPH_CLOSE, kernel, Point(-1, -1), 1);//闭运算

	/*	Mat dstImage_Erode, dstImage_Dilate, dstImage_Open, dstImage_Close;
	Mat kernel= getStructuringElement(MORPH_RECT, Size(3, 3));
	erode(binaryMat, dstImage_Erode, kernel);
	dilate(binaryMat, dstImage_Dilate, kernel);

	Mat kernel = getStructuringElement(MORPH_RECT, Size(11, 11));
	morphologyEx(binaryMat, dstImage_Open, MORPH_OPEN, kernel, Point(-1, -1), 1);//开运算  */

	int font_face = cv::FONT_HERSHEY_COMPLEX;
	double font_scale = 1.5;
	int thickness = 1;
	int baseline;
	float square_area[10];
	float center_area[10];
	int num_area[10];
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(binaryMat, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	//findContours(binaryMat, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
	cout << "num=" << contours.size() << endl;
	for (int i = 0; i < contours.size(); i++)
	{
		char num[10];
		sprintf_s(num, "%d", i);

		drawContours(dspMat, contours, i, Scalar(255), 1, 8, hierarchy);

		RotatedRect rect = minAreaRect(contours[i]);
		Point2f P[4];
		rect.points(P);
		cv::putText(dspMat, num, P[0], font_face, 1, CV_RGB(255, 0, 0), thickness, 8, 0);
		for (int j = 0; j <= 3; j++)
		{
			line(dspMat, P[j], P[(j + 1) % 4], Scalar(255), 1);
		}
		 //计算外接矩形面积
		//两点间距离公式求边长
		float Y = sqrt((P[0].y - P[1].y) * (P[0].y - P[1].y) + (P[0].x - P[1].x) * (P[0].x - P[1].x));
		float X = sqrt((P[1].y - P[2].y) * (P[1].y - P[2].y) + (P[1].x - P[2].x) * (P[1].x - P[2].x));
		square_area[i] = X * Y;
		cout << "num=" << i << "矩形面积square area=" << square_area[i] << endl;

		//绘制轮廓的最小外结圆
		Point2f center;
		float radius;//半径
		minEnclosingCircle(contours[i], center, radius);
		circle(dspMat, center, radius, Scalar(255), 1);
		//计算外接圆形面积
		float Π = 3.141596;
		center_area[i] = (radius*radius*Π);
		cout << "num=" << i << "圆形面积center_area=" << center_area[i] << endl;
	}

	int nComp = cv::connectedComponentsWithStats(binaryMat, 
		labelMat,
		statsMat,
		centrMat, 
		8, 
		CV_32S);

	// 输出连通域信息(can ignore)
		for (int i = 0; i < nComp; i++)
		{
			//各个连通域的统计信息保存在stasMat中
			cout << "connected Components NO. " << i << endl;
			cout << "pixels = " << statsMat.at<int>(i, 4) << endl;
			cout << "width = " << statsMat.at<int>(i, 2) << endl;
			cout << "height = " << statsMat.at<int>(i, 3) << endl;
			cout << endl;
		}

	cout << "the total of connected Components = " << nComp - 1 << endl;//-1,nComp包括背景
	resultMat = cv::Mat::zeros(srcMat.size(), CV_8UC3);	//显示用图像
	std::vector<cv::Vec3b> colors(nComp);
	colors[0] = cv::Vec3b(0, 0, 0);//背景黑色


/*
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
*/

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
	//计算圆形度，矩形度
	int h = 0;
	for (int k = 0; k < contours.size(); k++)
	{
		float circularity = (float)num_area[12 - k] / (float)center_area[k];
		float rectangularity = (float)num_area[12 - k] / (float)square_area[k];
		cout << "num=" << k << "圆形度=" << circularity << "矩形度=" << rectangularity << endl;
		if (circularity >= 0.98)
		{
			h++;
		}
	}
	cout << "最接近圆的图形有" << h << "个" << endl;
	imshow("srcMat", srcMat);
	imshow("dspMat", dspMat);
	imshow("binaryMat", binaryMat);
	waitKey(0);
	return 0;
}

