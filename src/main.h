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
extern void scanDirForInputFiles(const string& input_dir, const vector<string>& ext_list, vector<string>& out_filename_list);
extern bool loadImg(const string imgFile, Mat& out_img);
extern bool loadLdmks(const string ldmkFile, vector<Point2f>& out_imgPts);
extern void writeLdmks(const string filename, const vector<Point2f>& pts);

// video.cpp
extern void extractImageFromVideo(string input_dir, string output_dir);

// mouseEvent.cpp
extern void adjustGivenLdmksByHand(string input_dir, string output_dir);

////////////////////////////////////////////////////////////////
#endif