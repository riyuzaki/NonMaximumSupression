#ifndef __DETECT_H__
#define __DETECT_H__

#include<iostream>
#include<stdio.h>

#include<vector>
#include<algorithm>
#include<string>
#include <sstream>

#include <fstream>
#include <iterator>

#include <experimental/filesystem> //C++17

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

float ComputeIOUScore(vector<float> a, vector<float> b);
void aggregate(string filename);


#endif // __DETECT_H__
