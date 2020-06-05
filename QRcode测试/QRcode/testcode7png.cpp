/*

#include<opencv2/opencv.hpp>
#include "zbar.h"    
#include "cv.h"    
#include "highgui.h"    
#include <iostream>    

using namespace std;
using namespace zbar;  //���zbar���ƿռ�  
using namespace cv;

int main(int argc, char*argv[])
{
	ImageScanner scanner;
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
	Mat image = imread("1.png");
	Mat imageGray;
	cvtColor(image, imageGray, CV_RGB2GRAY);
	int width = imageGray.cols;
	int height = imageGray.rows;
	uchar *raw = (uchar *)imageGray.data;
	Image imageZbar(width, height, "Y800", raw, width * height);
	scanner.scan(imageZbar); //ɨ������  
	Image::SymbolIterator symbol = imageZbar.symbol_begin();
	if (imageZbar.symbol_begin() == imageZbar.symbol_end())
	{
		cout << "��ѯ����ʧ�ܣ�����ͼƬ��" << endl;
	}
	for (; symbol != imageZbar.symbol_end(); ++symbol)
	{
		cout << "���ͣ�" << endl << symbol->get_type_name() << endl << endl;
		cout << "���룺" << endl << symbol->get_data() << endl << endl;
	}
	imshow("Source Image", image);
	waitKey(0);
	imageZbar.set_data(NULL, 0);
	return 0;
}

*/
/*
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
	string fileName = "C:\\Users\\xiaodong\\Desktop\\123\\QRcode����\\1.png";
	Mat image = imread(fileName);
	double start = clock();
	if (!image.data)
	{
		cout << "��ȷ��ͼƬ" << endl;
		system("pause");
		return 0;
	}
	Mat imageGray;
	cvtColor(image, imageGray, CV_RGB2GRAY);
	int width = imageGray.cols;
	int height = imageGray.rows;
	uchar *raw = (uchar *)imageGray.data;
	Image imageZbar(width, height, "Y800", raw, width * height);
	scanner.scan(imageZbar); //ɨ������
	Image::SymbolIterator symbol = imageZbar.symbol_begin();
	if (imageZbar.symbol_begin() == imageZbar.symbol_end())
	{
		cout << "��ѯ����ʧ�ܣ�����ͼƬ��" << endl;
	}
	for (; symbol != imageZbar.symbol_end(); ++symbol)
	{
		cout << "���ͣ�" << endl << symbol->get_type_name() << endl << endl;
		cout << "���룺" << endl << symbol->get_data() << endl << endl;
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

*/

/*
#include<opencv2/opencv.hpp>
#include "zbar.h"    
#include "cv.h"    
#include "highgui.h"    
#include <iostream>    

using namespace std;
using namespace zbar;  //���zbar���ƿռ�  
using namespace cv;

int main(int argc, char*argv[])
{
	ImageScanner scanner;
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
	Mat image = imread("C:\\Users\\xiaodong\\Desktop\\123\\QRcode����\\1.png");
	Mat imageGray;
	cvtColor(image, imageGray, CV_RGB2GRAY);
	int width = imageGray.cols;
	int height = imageGray.rows;
	uchar *raw = (uchar *)imageGray.data;
	Image imageZbar(width, height, "Y800", raw, width * height);
	scanner.scan(imageZbar); //ɨ������  
	Image::SymbolIterator symbol = imageZbar.symbol_begin();
	if (imageZbar.symbol_begin() == imageZbar.symbol_end())
	{
		cout << "��ѯ����ʧ�ܣ�����ͼƬ��" << endl;
	}
	for (; symbol != imageZbar.symbol_end(); ++symbol)
	{
		cout << "���ͣ�" << endl << symbol->get_type_name() << endl << endl;
		cout << "���룺" << endl << symbol->get_data() << endl << endl;
	}
	imshow("Source Image", image);
	waitKey();
	imageZbar.set_data(NULL, 0); 
	system("pause");
	return 0;
}*/
/*
#include <opencv2/opencv.hpp>
#include "core/core.hpp"  
#include "highgui/highgui.hpp"  
#include "imgproc/imgproc.hpp"  
#include "zbar.h"
#include "decode.cpp"


using namespace cv;
using namespace std;
using namespace zbar;  //���zbar���ƿռ�  
*/

#include "decode.h"

int main(int argc, char *argv[])
{
	Mat image, imageGray, imageGuussian, image_copy;
	Mat imageSobelX, imageSobelY, imageSobelOut;
	//image = imread("C:\\Users\\xiaodong\\Desktop\\123\\QRcode����\\barcode.png");
	image = imread("C:\\Users\\xiaodong\\Desktop\\123\\QRcode����\\7.png");
	image_copy = image.clone();
	//1. ԭͼ���С�������������Ч��  
	//resize(image, image, Size(500, 300));
	imshow("1.ԭͼ��", image);

	//2. ת��Ϊ�Ҷ�ͼ  
	cvtColor(image, imageGray, CV_RGB2GRAY);
	imshow("2.�Ҷ�ͼ", imageGray);

	//3. ��˹ƽ���˲�  
	GaussianBlur(imageGray, imageGuussian, Size(3, 3), 0);
	imshow("3.��˹ƽ���˲�", imageGuussian);

	//4.���ˮƽ�ʹ�ֱ����Ҷ�ͼ����ݶȲ�,ʹ��Sobel����  
	Mat imageX16S, imageY16S;
	Sobel(imageGuussian, imageX16S, CV_16S, 1, 0, 3, 1, 0, 4);
	Sobel(imageGuussian, imageY16S, CV_16S, 0, 1, 3, 1, 0, 4);
	convertScaleAbs(imageX16S, imageSobelX, 1, 0);
	convertScaleAbs(imageY16S, imageSobelY, 1, 0);
	imageSobelOut = imageSobelX - imageSobelY;
	imshow("4.X�����ݶ�", imageSobelX);
	imshow("4.Y�����ݶ�", imageSobelY);
	imshow("4.XY�����ݶȲ�", imageSobelOut);

	//5.��ֵ�˲���������Ƶ����  
	blur(imageSobelOut, imageSobelOut, Size(3, 3));
	imshow("5.��ֵ�˲�", imageSobelOut);

	//6.��ֵ��  
	Mat imageSobleOutThreshold;
	threshold(imageSobelOut, imageSobleOutThreshold, 100, 255, CV_THRESH_BINARY);
	imshow("6.��ֵ��", imageSobleOutThreshold);

	//7.�����㣬����������϶  
	Mat  element = getStructuringElement(0, Size(7, 7));
	morphologyEx(imageSobleOutThreshold, imageSobleOutThreshold, MORPH_CLOSE, element);
	imshow("7.������", imageSobleOutThreshold);

	//8. ��ʴ��ȥ�������ĵ�  
	erode(imageSobleOutThreshold, imageSobleOutThreshold, element);
	erode(imageSobleOutThreshold, imageSobleOutThreshold, element);

	imshow("8.��ʴ", imageSobleOutThreshold);

	//9. ���ͣ������������϶�����ݺ˵Ĵ�С���п�����Ҫ2~3�����Ͳ���  
	dilate(imageSobleOutThreshold, imageSobleOutThreshold, element);
	dilate(imageSobleOutThreshold, imageSobleOutThreshold, element);
	dilate(imageSobleOutThreshold, imageSobleOutThreshold, element);
	dilate(imageSobleOutThreshold, imageSobleOutThreshold, element);

	imshow("9.����", imageSobleOutThreshold);
	vector<vector<Point>> contours;
	vector<Vec4i> hiera;

	//10.ͨ��findContours�ҵ�����������ľ��α߽�  
	findContours(imageSobleOutThreshold, contours, hiera, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	for (int i = 0; i < contours.size(); i++)
	{
		Rect rect = boundingRect((Mat)contours[i]);
		rectangle(image, rect, Scalar(255), 2);
	}
	imshow("10.�ҳ���ά���������", image);

	//11.�ü�
	Rect myRect = boundingRect(contours[0]);
	Mat img = Mat(image_copy, myRect);
	imshow("11.�ü�", img);
	imwrite("jietu.jpg", img);
	
	//12 ʶ��

	Mat imgGray, imageBinary;
	int i = 0;
	list<int> width;
	vector<int> barSpaceWidth(60);

	cvtColor(img, imgGray, CV_RGB2GRAY);
	threshold(imgGray, imageBinary, 80, 255, CV_THRESH_BINARY);

	//��ȡ���տ��
	width = get_bar_space_width(imageBinary);
	for (list<int>::iterator iter = width.begin(); iter != width.end(); ++iter, i++) {
		barSpaceWidth[i] = *iter;
		cout << *iter << endl;

	}
	cout << "--------------" << endl;


	//�������Ʊ���ֵ
	vector<int> similarEdge = cal_similar_edge(barSpaceWidth);
	for (vector<int>::iterator iter = similarEdge.begin(); iter != similarEdge.end(); ++iter) {
		cout << *iter << endl;
	}

	cout << similarEdge.size() << endl;
	cout << "--------------" << endl;

	//���Ʊ�����
	vector<string> barCode_string = decode_similar_edge(similarEdge);

	//��ԡ�#������
	decode_sharp(barCode_string, barSpaceWidth);

	//��barCode��stringת��Ϊint
	vector<int> barCode;
	for (vector<string>::iterator iter = barCode_string.begin(); iter != barCode_string.end(); ++iter) {
		barCode.push_back(stoi(*iter));
	}
	cout << "--------------" << endl;

	//У��λ
	cout << check_bar_code(barCode) << endl;
	cout << "--------------" << endl;


	//���ս���
	cout << "��������룺" << endl;
	for (vector<int>::iterator iter = barCode.begin(); iter != barCode.end(); ++iter) {
		cout << *iter;
	}
	cout << endl;
	cout << "--------------" << endl;


	namedWindow("Source Image", CV_WINDOW_NORMAL);
	imshow("Source Image", imageBinary);
	waitKey(0);
	destroyAllWindows();

	

	/*
	//12.ʶ��
	ImageScanner scanner;
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
	
	Mat imageGray2;
	cvtColor(img, imageGray2, CV_RGB2GRAY);
	int width = imageGray2.cols;
	int height = imageGray2.rows;
	uchar *raw = (uchar *)imageGray2.data;
	Image imageZbar(width, height, "Y800", raw, width * height);
	scanner.scan(imageZbar); //ɨ������  
	Image::SymbolIterator symbol = imageZbar.symbol_begin();
	if (imageZbar.symbol_begin() == imageZbar.symbol_end())
	{
		cout << "��ѯ����ʧ�ܣ�����ͼƬ��" << endl;
	}
	for (; symbol != imageZbar.symbol_end(); ++symbol)
	{
		cout << "���ͣ�" << endl << symbol->get_type_name() << endl << endl;
		cout << "���룺" << endl << symbol->get_data() << endl << endl;
	}
	imshow("Source Image", image);
	waitKey();
	imageZbar.set_data(NULL, 0);
	system("pause");


	waitKey();*/

	return 0;
}