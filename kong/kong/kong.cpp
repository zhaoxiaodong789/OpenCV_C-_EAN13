﻿#include <opencv2/opencv.hpp>
#include <zbar.h>
#include <cv.h>
#include <highgui.h>
#include <iostream>

using namespace std;
using namespace zbar;
using namespace cv;

int main(int argc, char** argv)
{
	zbar::ImageScanner scanner;
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
	string fileName = "C:\\Users\\xiaodong\\Desktop\\123\\kong\\1.png";
	Mat image = imread(fileName);
	double start = clock();
	if (!image.data)
	{
		cout << "请确认图片" << endl;
		system("pause");
		return 0;
	}
	Mat imageGray;
	cvtColor(image, imageGray, CV_RGB2GRAY);
	int width = imageGray.cols;
	int height = imageGray.rows;
	uchar *raw = (uchar *)imageGray.data;
	Image imageZbar(width, height, "Y800", raw, width * height);
	scanner.scan(imageZbar); //扫描条码
	Image::SymbolIterator symbol = imageZbar.symbol_begin();
	if (imageZbar.symbol_begin() == imageZbar.symbol_end())
	{
		cout << "查询条码失败，请检查图片！" << endl;
	}
	for (; symbol != imageZbar.symbol_end(); ++symbol)
	{
		cout << "类型：" << endl << symbol->get_type_name() << endl << endl;
		cout << "条码：" << endl << symbol->get_data() << endl << endl;
		string s = symbol->get_data();
	}
	double end = clock();
	double ts = end - start;
	cout << ts << endl;

	imshow("Source Image", image);
	waitKey();
	imageZbar.set_data(NULL, 0);
	system("pause");

	return 0;
}