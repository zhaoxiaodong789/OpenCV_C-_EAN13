#include "opencv2/opencv.hpp"
#include "zbar.h"
#include "cv.h"
#include "highgui.h"
#include <iostream>

using namespace std;
using namespace zbar;
using namespace cv;

int main(int argc, char** argv)
{
	zbar::ImageScanner scanner;
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
	string fileName = "C:\\Users\\xiaodong\\Desktop\\123\\QRcode\\zhongwen.png";
	Mat image = imread(fileName);
	double start = clock();
	if (!image.data)
	{
		cout << "ÇëÈ·ÈÏÍ¼Æ¬" << endl;
		system("pause");
		return 0;
	}
	Mat imageGray;
	cvtColor(image, imageGray, CV_RGB2GRAY);
	int width = imageGray.cols;
	int height = imageGray.rows;
	uchar *raw = (uchar *)imageGray.data;
	Image imageZbar(width, height, "Y800", raw, width * height);
	scanner.scan(imageZbar); //É¨ÃèÌõÂë
	Image::SymbolIterator symbol = imageZbar.symbol_begin();
	if (imageZbar.symbol_begin() == imageZbar.symbol_end())
	{
		cout << "²éÑ¯ÌõÂëÊ§°Ü£¬Çë¼ì²éÍ¼Æ¬£¡" << endl;
	}
	for (; symbol != imageZbar.symbol_end(); ++symbol)
	{
		cout << "ÀàÐÍ£º" << endl << symbol->get_type_name() << endl << endl;
		cout << "ÌõÂë£º" << endl << symbol->get_data() << endl << endl;
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