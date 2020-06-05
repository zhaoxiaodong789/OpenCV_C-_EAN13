#include <iostream>
#include <map>
#include <string>
#include <list>
#include <opencv2/opencv.hpp>
#include "core/core.hpp"  
#include "highgui/highgui.hpp"  
#include "imgproc/imgproc.hpp" 

using namespace std;
using namespace cv;


map<string, int> DECODING_TABLE = {
	{"0001101" , 0}, {"0100111" , 0}, {"1110010" , 0},
	{"0011001" , 1}, {"0110011" , 1}, {"1100110" , 1},
	{"0010011" , 2}, {"0011011" , 2}, {"1101100" , 2},
	{"0111101" , 3}, {"0100001" , 3}, {"1000010" , 3},
	{"0100011" , 4}, {"0011101" , 4}, {"1011100" , 4},
	{"0110001" , 5}, {"0111001" , 5}, {"1001110" , 5},
	{"0101111" , 6}, {"0000101" , 6}, {"1010000" , 6},
	{"0111011" , 7}, {"0010001" , 7}, {"1000100" , 7},
	{"0110111" , 8}, {"0001001" , 8}, {"1001000" , 8},
	{"0001011" , 9}, {"0010111" , 9}, {"1110100" , 9}
};

map<int, map<int, string>> EDGE_TABLE = {
	{2, {{2, "6"}, {3, "0"}, {4, "4"}, {5, "3"}}},
	{3, {{2, "9"}, {3, "33"}, {4, "34"}, {5, "5"}}},
	{4, {{2, "9"}, {3, "43"}, {4, "44"}, {5, "5"}}},
	{5, {{2, "6"}, {3, "0"}, {4, "4"}, {5, "3"}}}

};

map<int, map<int, string>> FIRST_TABLE = {
	{2, {{2, "O"}, {3, "E"}, {4, "O"}, {5, "E"}}},
	{3, {{2, "E"}, {3, "O"}, {4, "E"}, {5, "O"}}},
	{4, {{2, "O"}, {3, "E"}, {4, "O"}, {5, "E"}}},
	{5, {{2, "E"}, {3, "O"}, {4, "E"}, {5, "O"}}}
};

map<string, int> FIRST_DETECT_TABLE = {
	{"OOOOOO" , 0}, {"OOEOEE" , 1}, {"OOEEOE" , 2}, {"OOEEEO" , 3}, {"OEOOEE" , 4},
	{"OEEOOE" , 5}, {"OEEEOO" , 6}, {"OEOEOE" , 7}, {"OEOEEO" , 8 },{"OEEOEO" , 9}
};

const int INDEX_IN_WIDTH[13] = { 0, 4, 8, 12, 16, 20, 24, 33, 37, 41, 45, 49, 53 };



list<int> get_bar_space_width(Mat img) {
	int currentPix = -1;
	int lastPix = -1;
	int pos = 0;
	int i, j;
	list<int> width;
	Scalar s;
	for (j = 0; j < img.rows; j++) {
		width.clear();
		for (i = 0; i < img.cols; i++) {
			s = img.at<uchar>(j, i);
			currentPix = s[0];
			if (currentPix != lastPix) {
				if (lastPix == -1) {
					lastPix = currentPix;
					pos = i;
				}
				else {
					width.push_back(i - pos);
					pos = i;
					lastPix = currentPix;
				}
			
			}
			
		
		}
		if (width.size() == 60)
			break;
	
	}
	return width;

}

int divide(int t, int l) {
	if (float(t) / l < 0.357) {
		return 2;
	}
	else if (float(t) / l < 0.500) {
		return 3;
	}
	else if (float(t) / l < 0.643) {
		return 4;
	}
	else {
		return 5;
	}
}

vector<int> cal_similar_edge(vector<int> data) {
	vector<int> similarEdge;
	/*
	float limit = 0;
	limit = float(data[1] + data[2] + data[3]) / 3 * 1.5;
	if (data[1] >= limit || data[2] >= limit || data[3] >= limit) {
		cout << "error!" << endl;
		return similarEdge;
	}
*/
	int index = 4;
	int T1 = 0, T2 = 0, L = 0;


	while (index < 54) {
		if (index == 28 || index == 29 || index == 30 || index == 31 || index == 32){
			index++;
			continue;
		}
		T1 = data[index] + data[index + 1];
		T2 = data[index + 1] + data[index + 2];
		L = data[index] + data[index + 1] + data[index + 2] + data[index + 3];
		similarEdge.push_back(divide(T1, L));
		similarEdge.push_back(divide(T2, L));
		index += 4;
	}
 

	return similarEdge;
}

void decode_sharp(vector<string>& barCode, vector<int> barSpaceWidth) {
	int i, index, c1, c2= 0;

	for (i = 0; i < 13; i++) {
		if (barCode[i] == "44") {
			index = INDEX_IN_WIDTH[i];
			c1 = barSpaceWidth[index + 2];
			c2 = barSpaceWidth[index + 3];
			if (c1 > c2) {
				barCode[i] = "1";
			}
			else {
				barCode[i] = "7";
			}
		}
		else if (barCode[i] == "33"){
			index = INDEX_IN_WIDTH[i];
			c1 = barSpaceWidth[index];
			c2 = barSpaceWidth[index + 1];
			if (c1 > c2) {
				barCode[i] = "2";
			}
			else {
				barCode[i] = "8";
			}
		}
		else if (barCode[i] == "34") {
			index = INDEX_IN_WIDTH[i];
			c1 = barSpaceWidth[index];
			c2 = barSpaceWidth[index + 1];
			if (c1 > c2) {
				barCode[i] = "7";
			}
			else {
				barCode[i] = "1";
			}
		}
		else if (barCode[i] == "43") {
			index = INDEX_IN_WIDTH[i];
			c1 = barSpaceWidth[index + 1];
			c2 = barSpaceWidth[index + 2];
			if (c1 > c2) {
				barCode[i] = "2";
			}
			else {
				barCode[i] = "8";
			}
		}
	}


}

bool check_bar_code(vector<int> barCode) {
	int evens, odds, sum = 0;
	evens = barCode[11] + barCode[9] + barCode[7] + barCode[5] + barCode[3] + barCode[1];
	odds = barCode[10] + barCode[8] + barCode[6] + barCode[4] + barCode[2] + barCode[0];
	sum = evens * 3 + odds;
	if (barCode[12] == (10 - sum % 10) % 10) {
		return true;
	}
	else
		return false;
}


vector<string> decode_similar_edge(vector<int> edge) {
	/*
	vector<string> barCode;
	barCode.push_back("6");
	int i = 0;
	for (i = 0; i < 24; i += 2) {
		barCode.push_back(EDGE_TABLE[edge[i]][edge[i+1]]);

	}
	*/
	vector<string> barCode;
	string str;
	for (int i = 0; i < 12; i += 2){
		str = str + FIRST_TABLE[edge[i]][edge[i + 1]];
	
	}
		
	if (FIRST_DETECT_TABLE.find(str) == FIRST_DETECT_TABLE.end()) {
		return barCode;
	}
	else {
		string s = to_string(FIRST_DETECT_TABLE[str]);
		barCode.push_back(s);
	}
		
	for (int i = 0; i < 24; i += 2) {
		barCode.push_back(EDGE_TABLE[edge[i]][edge[i + 1]]);

	}


	return barCode;
}


int main(int argc, char *argv[]) {
	Mat image, imageGray, imageBinary;
	int i = 0;
	list<int> width;
	vector<int> barSpaceWidth(60);

	//image = imread("C:\\Users\\xiaodong\\Desktop\\opencv\\jietu.jpg");
	image = imread("C:\\Users\\xiaodong\\Desktop\\12.png");
	imshow("1", image);
	cvtColor(image, imageGray, CV_RGB2GRAY);
	threshold(imageGray, imageBinary, 200, 255, CV_THRESH_BINARY);





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
	string bar;
	for (vector<string>::iterator iter = barCode_string.begin(); iter != barCode_string.end(); ++iter) {
		barCode.push_back(stoi(*iter));
		bar = bar + (*iter);
	}
	cout << "--------------" << endl;
	cout << "bar:+++" << bar << endl;
	//校验位
	cout << check_bar_code(barCode) << endl;
	cout << "--------------" << endl;


	//最终解码
	cout << "条形码解码：" << endl;
	for (vector<int>::iterator iter = barCode.begin(); iter != barCode.end(); ++iter) {
		cout << *iter ;
	}
	cout << endl;

	cout << "--------------" << endl;





	

	
	namedWindow("Source Image", CV_WINDOW_NORMAL);
	imshow("Source Image", imageBinary);

	waitKey(0);	
	
	destroyAllWindows();
	return 0;
}
