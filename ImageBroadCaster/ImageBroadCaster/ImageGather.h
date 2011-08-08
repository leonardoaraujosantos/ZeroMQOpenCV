#pragma once

#include "ImageFactory.h"
#include "GenericImage.h"

class ImageGather
{
public:
	ImageGather(ImageFactory &p_imFactory);
	~ImageGather(void);
	void prepareImage(GenericImage &img);
private:
	// Class for get images from camera or directory
	ImageFactory &imFactory;
};
