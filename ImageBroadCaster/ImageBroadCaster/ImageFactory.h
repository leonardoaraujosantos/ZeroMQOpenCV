#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include "GenericImage.h"

#include <boost/filesystem.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
/*
	Class used to get images from some directory or camera
*/
class ImageFactory
{
public:
	ImageFactory(int videoMode, std::string directory, std::string p_extension, int debugMode = 0, std::string roi = "", int equalizeHist = 0);
	~ImageFactory(void);
	void getImage(GenericImage &image);
private:
	void displayImage(cv::Mat image);
	int mVideoMode;
	std::string mImageDirectory;
	boost::filesystem::path ImPath;
	std::vector< std::string > mImageFilePath;
	int mVecIndex;
	int debugMode_;
	int equalizeHistogram_;
	std::string roi_;
};
