#include "decode.h"

int main(int argc, char *argv[])
{
	Mat image, imageGray, imageGuussian, image_copy;
	Mat imageSobelX, imageSobelY, imageSobelOut, xuanzhuanImg;
	float angle, boxwidth, height;
	//image = imread("C:\\Users\\xiaodong\\Desktop\\123\\QRcode测试\\barcode.png");
	image = imread("C:\\Users\\xiaodong\\Desktop\\123\\QRcode测试\\xuanzhuan.png");
	image_copy = image.clone();
	xuanzhuanImg = image.clone();

	//1. 原图像大小调整，提高运算效率  
	//resize(image, image, Size(500, 300));
	imshow("1.原图像", image);

	//2. 转化为灰度图  
	cvtColor(image, imageGray, CV_RGB2GRAY);
	imshow("2.灰度图", imageGray);

	//3. 高斯平滑滤波  
	GaussianBlur(imageGray, imageGuussian, Size(3, 3), 0);
	imshow("3.高斯平衡滤波", imageGuussian);

	//4.求得水平和垂直方向灰度图像的梯度差,使用Sobel算子  
	Mat imageX16S, imageY16S;
	Sobel(imageGuussian, imageX16S, CV_16S, 1, 0, 3, 1, 0, 4);
	Sobel(imageGuussian, imageY16S, CV_16S, 0, 1, 3, 1, 0, 4);
	convertScaleAbs(imageX16S, imageSobelX, 1, 0);
	convertScaleAbs(imageY16S, imageSobelY, 1, 0);
	imageSobelOut = imageSobelX - imageSobelY;
	imshow("4.X方向梯度", imageSobelX);
	imshow("4.Y方向梯度", imageSobelY);
	imshow("4.XY方向梯度差", imageSobelOut);

	//5.均值滤波，消除高频噪声  
	blur(imageSobelOut, imageSobelOut, Size(3, 3));
	imshow("5.均值滤波", imageSobelOut);

	//6.二值化  
	Mat imageSobleOutThreshold;
	threshold(imageSobelOut, imageSobleOutThreshold, 50, 255, CV_THRESH_BINARY);
	imshow("6.二值化", imageSobleOutThreshold);

	//7.闭运算，填充条形码间隙  
	Mat  element = getStructuringElement(0, Size(7, 7));
	morphologyEx(imageSobleOutThreshold, imageSobleOutThreshold, MORPH_CLOSE, element);
	imshow("7.闭运算", imageSobleOutThreshold);

	//8. 腐蚀，去除孤立的点  
	erode(imageSobleOutThreshold, imageSobleOutThreshold, element);
	erode(imageSobleOutThreshold, imageSobleOutThreshold, element);

	imshow("8.腐蚀", imageSobleOutThreshold);

	//9. 膨胀，填充条形码间空隙，根据核的大小，有可能需要2~3次膨胀操作  
	dilate(imageSobleOutThreshold, imageSobleOutThreshold, element);
	dilate(imageSobleOutThreshold, imageSobleOutThreshold, element);
	dilate(imageSobleOutThreshold, imageSobleOutThreshold, element);
	dilate(imageSobleOutThreshold, imageSobleOutThreshold, element);

	imshow("9.膨胀", imageSobleOutThreshold);
	vector<vector<Point>> contours;
	vector<Vec4i> hiera;

	

	//10.通过findContours找到条形码区域的矩形边界  
	findContours(imageSobleOutThreshold, contours, hiera, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	
	vector<RotatedRect> box(contours.size());
	Point2f rotatedRect[4];

	for (int i = 0; i < contours.size(); i++)
	{
		Rect rect = boundingRect((Mat)contours[i]);
		rectangle(image, rect, Scalar(255), 2);

		box[i] = minAreaRect((Mat)contours[i]);
		box[i].points(rotatedRect);


		//绘制旋转最小矩形
		for (int j = 0; j < 4; j++) {
			line(image, rotatedRect[j], rotatedRect[(j + 1) % 4], Scalar(0, 255, 0), 2, 8);

		}


		angle = box[i].angle;
		boxwidth = box[i].size.width;
		height = box[i].size.height;
		cout << "最小矩形：" << angle << ";" << boxwidth << ";" << height << endl;

		//12. 旋转  利用仿射变换进行旋转 另一种方法，透视变换
		if (0 < abs(angle) && abs(angle) <= 45)
			angle = angle;//负数，顺时针旋转
		else if (45 < abs(angle) && abs(angle) < 90)
			angle = 90 - abs(angle);//正数，逆时针旋转
		Point2f center = box[i].center;  //定义旋转中心坐标
		double angle0 = angle;
		double scale = 1;
		Mat roateM = getRotationMatrix2D(center, angle0, scale);  //获得旋转矩阵,顺时针为负，逆时针为正
		warpAffine(image_copy, xuanzhuanImg, roateM, xuanzhuanImg.size()); //仿射变换


	}
	imshow("10.找出二维码矩形区域", image);

	


	//11.裁剪
	Rect myRect = boundingRect(contours[0]);
	Mat img = Mat(xuanzhuanImg, myRect);
	imshow("11.裁剪", img);
	imwrite("jietu.jpg", img);
	

	//12.旋转

	namedWindow("xuanzhuan Image", CV_WINDOW_AUTOSIZE);
	imshow("xuanzhuan Image", xuanzhuanImg);


	//13 识别

	Mat imgGray, imageBinary;
	int i = 0;
	list<int> width;
	vector<int> barSpaceWidth(60);

	cvtColor(img, imgGray, CV_RGB2GRAY);
	threshold(imgGray, imageBinary, 80, 255, CV_THRESH_BINARY);

	//提取条空宽度
	width = get_bar_space_width(imageBinary);
	for (list<int>::iterator iter = width.begin(); iter != width.end(); ++iter, i++) {
		barSpaceWidth[i] = *iter;
		cout << *iter << endl;

	}
	cout << "--------------" << endl;


	//计算相似边数值
	vector<int> similarEdge = cal_similar_edge(barSpaceWidth);
	for (vector<int>::iterator iter = similarEdge.begin(); iter != similarEdge.end(); ++iter) {
		cout << *iter << endl;
	}

	cout << similarEdge.size() << endl;
	cout << "--------------" << endl;

	//相似边译码
	vector<string> barCode_string = decode_similar_edge(similarEdge);

	//针对‘#’译码
	decode_sharp(barCode_string, barSpaceWidth);

	//将barCode的string转换为int
	vector<int> barCode;
	for (vector<string>::iterator iter = barCode_string.begin(); iter != barCode_string.end(); ++iter) {
		barCode.push_back(stoi(*iter));
	}
	cout << "--------------" << endl;

	//校验位
	cout << check_bar_code(barCode) << endl;
	cout << "--------------" << endl;


	//最终解码
	cout << "条形码解码：" << endl;
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
	//12.识别
	ImageScanner scanner;
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
	
	Mat imageGray2;
	cvtColor(img, imageGray2, CV_RGB2GRAY);
	int width = imageGray2.cols;
	int height = imageGray2.rows;
	uchar *raw = (uchar *)imageGray2.data;
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
	}
	imshow("Source Image", image);
	waitKey();
	imageZbar.set_data(NULL, 0);
	system("pause");


	waitKey();*/

	return 0;
}