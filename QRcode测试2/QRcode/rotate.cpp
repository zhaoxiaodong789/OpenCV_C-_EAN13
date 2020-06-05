#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>
#include <cmath>

using namespace cv;

Mat imgRotate(Mat matSrc, float angle, bool direction)
{
	float theta = angle * CV_PI / 180.0;
	int nRowsSrc = matSrc.rows;
	int nColsSrc = matSrc.cols;
	// �����˳ʱ����ת
	if (!direction)
		theta = 2 * CV_PI - theta;
	// ȫ������ʱ����ת������
	// ��ʱ����ת����
	float matRotate[3][3]{
		{std::cos(theta), -std::sin(theta), 0},
		{std::sin(theta), std::cos(theta), 0 },
		{0, 0, 1}
	};
	float pt[3][2]{
		{ 0, nRowsSrc },
		{nColsSrc, nRowsSrc},
		{nColsSrc, 0}
	};
	for (int i = 0; i < 3; i++)
	{
		float x = pt[i][0] * matRotate[0][0] + pt[i][1] * matRotate[1][0];
		float y = pt[i][0] * matRotate[0][1] + pt[i][1] * matRotate[1][1];
		pt[i][0] = x;
		pt[i][1] = y;
	}
	// �������ת��ͼ��ļ�ֵ��ͳߴ�
	float fMin_x = min(min(min(pt[0][0], pt[1][0]), pt[2][0]), (float)0.0);
	float fMin_y = min(min(min(pt[0][1], pt[1][1]), pt[2][1]), (float)0.0);
	float fMax_x = max(max(max(pt[0][0], pt[1][0]), pt[2][0]), (float)0.0);
	float fMax_y = max(max(max(pt[0][1], pt[1][1]), pt[2][1]), (float)0.0);
	int nRows = cvRound(fMax_y - fMin_y + 0.5) + 1;
	int nCols = cvRound(fMax_x - fMin_x + 0.5) + 1;
	int nMin_x = cvRound(fMin_x + 0.5);
	int nMin_y = cvRound(fMin_y + 0.5);

	IplImage tmp = IplImage(matSrc);
	CvArr* arr = (CvArr*)&tmp;
	Scalar s = cvGet2D(arr, 2, 2);
	// �������ͼ��
	//Mat matRet(nRows, nCols, matSrc.type(), Scalar(255,255,255));
	Mat matRet(nRows, nCols, matSrc.type(), s);
	for (int j = 0; j < nRows; j++)
	{
		for (int i = 0; i < nCols; i++)
		{
			// ��������ͼ����ԭͼ���еĶ�Ӧ������꣬Ȼ���Ƹ�����ĻҶ�ֵ
			// ��Ϊ����ʱ��ת������������ӳ�䵽ԭͼ���ʱ����Կ����ǣ����ͼ��
			// ��˳ʱ����ת��ԭͼ��ģ���˳ʱ����ת����պ�����ʱ����ת�����ת��
			// ͬʱ��Ҫ���ǵ�Ҫ����ת���ͼ������Ͻ��ƶ�������ԭ�㡣
			int x = (i + nMin_x) * matRotate[0][0] + (j + nMin_y) * matRotate[0][1];
			int y = (i + nMin_x) * matRotate[1][0] + (j + nMin_y) * matRotate[1][1];
			if (x >= 0 && x < nColsSrc && y >= 0 && y < nRowsSrc)
			{
				matRet.at<Vec3b>(j, i) = matSrc.at<Vec3b>(y, x);
			}
		}
	}
	return matRet;
}

int main()
{
	//std::string strPath = "D:\\MyDocuments\\My Pictures\\OpenCV\\";
	Mat matSrc = imread("C:\\Users\\xiaodong\\Desktop\\123\\QRcode����\\xuanzhuan.png");
	if (matSrc.empty())
		return 1;
	float angle = 30;
	Mat matRet = imgRotate(matSrc, angle, false);
	imshow("src", matSrc);
	imshow("rotate", matRet);
	// ����ͼ��
	//imwrite(strPath + "rotate_panda.jpg", matRet);

	waitKey();
	return 0;
}