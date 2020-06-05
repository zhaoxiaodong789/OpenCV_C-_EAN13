#pragma once

#include <opencv2/opencv.hpp>
#include "core/core.hpp"  
#include "highgui/highgui.hpp"  
#include "imgproc/imgproc.hpp"  
#include "zbar.h"

#include <iostream>
#include <map>
#include <string>
#include <list>

using namespace cv;
using namespace std;
using namespace zbar;  //Ìí¼ÓzbarÃû³Æ¿Õ¼ä  

list<int> get_bar_space_width(Mat img);
int divide(int t, int l);
vector<int> cal_similar_edge(vector<int> data);
void decode_sharp(vector<string>& barCode, vector<int> barSpaceWidth);
bool check_bar_code(vector<int> barCode);
vector<string> decode_similar_edge(vector<int> edge);
