#include "GenericImage.h"

GenericImage::GenericImage(void)
{
}

GenericImage::GenericImage(int w, int h, int ws,int channels, char *image_data) : width(w), height(h), width_step(ws), nchannels(channels)
{
	size = width_step * height;

	// Fill image vector buffer with OpenCv image data
	ImmBuffer = new char[size];
	for (int count = 0; count < size; count++)
	{
		//ImmBuffer.push_back(image_data[count]);
		ImmBuffer[count] = image_data[count];
	}
}

GenericImage::~GenericImage(void)
{
	//delete [] ImmBuffer; 
}

char * GenericImage::getImageBuffer()
{
	return ImmBuffer;
}

void GenericImage::setImage(cv::Mat image)
{	
	cv::Mat::MSize siz = image.size;
	width = image.cols;
	height = image.rows;
	nchannels = image.channels();
	width_step = image.step;
	size = width_step * height;

	// Fill image vector buffer with OpenCv image data	
	//ImmBuffer.clear();
	ImmBuffer = new char[size];
	for (int count = 0; count < size; count++)
	{
		ImmBuffer[count] = image.data[count];
		//ImmBuffer.push_back(image.data[count]);		
	}

}
