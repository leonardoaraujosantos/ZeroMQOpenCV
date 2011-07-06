#include "ImageFactory.h"

using namespace boost::filesystem;
ImageFactory::ImageFactory(int videoMode, std::string directory, std::string p_extension, 
						   int debugMode, std::string roi, int equalizeHist) 
						   : mVideoMode(videoMode), mImageDirectory(directory), ImPath(directory), 
						   debugMode_(debugMode), equalizeHistogram_(equalizeHist), roi_(roi)
{
	mVecIndex = 0;
	/*if (debugMode_)
	{
		cv::namedWindow("ImageDebug");
	}*/
	if (!mVideoMode)
	{
		// Verify if path exist and if it's some directory
		if ( (boost::filesystem::exists(ImPath)) && (boost::filesystem::is_directory(ImPath)) )
		{
			std::cout << directory << " is a valid directory" << std::endl;	
			
			typedef std::vector<boost::filesystem::path> vec;
			vec v;
			
			copy(directory_iterator(ImPath), directory_iterator(), back_inserter(v));
			// Sort...
			sort(v.begin(), v.end());
			
			for (vec::const_iterator it(v.begin()), it_end(v.end()); it != it_end; ++it)
			{				
				boost::filesystem::path p = *it;
				
				// Skip if it's not a file
				if (!boost::filesystem::is_regular_file(p)) continue;
				
				// Skip if extension is invalid
				if (boost::filesystem::extension(p) == p_extension)
				{
					std::cout << "Got image:  " << *it << std::endl;
					// Adciona na lista de imagens...
					mImageFilePath.push_back(p.string());
				}
			}
		}
		else
		{
			throw std::exception("Invalid directory");
		}
		if (mImageFilePath.size() == 0)
		{
			throw std::exception("No Images found");
		}

	}
}

ImageFactory::~ImageFactory(void)
{
}

void ImageFactory::getImage(GenericImage &image)
{
	if (!mVideoMode)
	{		
		// Get images from directory		
		cv::Mat imageCV = cv::imread(mImageFilePath[mVecIndex],1);	
		cv::Mat imageTresh = cv::Mat::zeros(imageCV.size(), imageCV.type());
		if (imageCV.empty())
		{
			throw std::exception("Error opening image ");
		}
		
		
		// Run any preprocessing before on this image...
		// Blur the image
		cv::medianBlur(imageCV,imageCV,3);
		if (equalizeHistogram_)
		{
			std::vector<cv::Mat> splitedImage;	
			cv::Mat equalizedImage;										
			
			// Equalize each channel
			if (imageCV.channels() == 3)
			{
				// Split channels
				cv::split(imageCV,splitedImage);		
				
				cv::equalizeHist(splitedImage[0],splitedImage[0]);
				cv::equalizeHist(splitedImage[1],splitedImage[1]);
				cv::equalizeHist(splitedImage[2],splitedImage[2]);
			}
			else
			{
				// Equalize... src, dest
				cv::equalizeHist(imageCV,equalizedImage);
			}
			
			// Merge channels
			cv::merge(splitedImage,equalizedImage);
			
			// Set image.
			image.setImage(equalizedImage);
		}
		else
		{
			image.setImage(imageCV);
		}
				
		// Iterate to next image in the directory
		if (mVecIndex < (mImageFilePath.size() - 1))
		{
			mVecIndex++;
		}
		else
		{
			mVecIndex = 0;
		}

		if (debugMode_)
		{			
			displayImage(imageCV);
		}
	}
	else
	{
		// Get images from camera
	}
}

void ImageFactory::displayImage(cv::Mat image)
{
	cv::namedWindow("Display Image");
	cv::imshow("Display Image",image);
	cv::waitKey(1); 
}
