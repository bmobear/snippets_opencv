/* 
**** notes ****
	- to read video file, put opencv_ffmpeg<version>.dll in the bin directory
*/



#ifndef CV_MAIN_H
#define CV_MAIN_H
////////////////////////////////////////////////////////////////

// IO
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

// containers
#include <vector>
#include <map>

// Boost
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

// OPENCV
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

//==============================================================

//utils.cpp
extern void extractImageFromVideo(string input_dir, string output_dir);
extern void scanDirForInputFiles(const string& input_dir, const vector<string>& ext_list, vector<string>& out_filename_list);

////////////////////////////////////////////////////////////////
#endif