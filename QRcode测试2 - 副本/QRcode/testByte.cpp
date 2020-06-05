#include "decode.h"
typedef unsigned char byte;

byte * matToBytes(Mat image)
{
	int size = image.total() * image.elemSize();
	byte * bytes = new byte[size];  // you will have to delete[] that later
	std::memcpy(bytes, image.data, size * sizeof(byte));
	return bytes;
}

Mat bytesToMat(byte * bytes, int width, int height)
{
	Mat image = Mat(height, width, CV_8UC3, bytes).clone(); // make a copy
	return image;
}

int main(int argc, char *argv[])
{
	Mat image, imageGray, imageGuussian, image_copy;
	Mat imageSobelX, imageSobelY, imageSobelOut, xuanzhuanImg;
	byte * b;
	float angle, boxwidth, height;
	//image = imread("C:\\Users\\xiaodong\\Desktop\\123\\QRcode≤‚ ‘\\barcode.png");
	image = imread("C:\\Users\\xiaodong\\Desktop\\123\\QRcode≤‚ ‘\\xuanzhuan.png");
	image_copy = Mat(image.rows, image.cols, CV_8UC3, Scalar(0,0,255));;
	xuanzhuanImg = image.clone();
	b = matToBytes(image);
	//cout << b << endl;
	imageGray = image_copy;
	image_copy = bytesToMat(b, image.cols, image.rows);
	cout << imageGray.cols << "---" << imageGray.rows << endl;
	cout << "------------" << endl;
	cout << image_copy.cols<<"---" <<image_copy.rows<< endl;

	//namedWindow("Source Image", CV_WINDOW_NORMAL);
	imshow("Source Image", image_copy);
	//namedWindow("Source Image2", CV_WINDOW_NORMAL);
	imshow("Source Image2", imageGray);
	waitKey(0);
	destroyAllWindows();





	return 0;
}
