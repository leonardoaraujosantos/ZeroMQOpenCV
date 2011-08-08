#include "ImageGather.h"

ImageGather::ImageGather(ImageFactory &p_imFactory) : imFactory(p_imFactory)
{
}

ImageGather::~ImageGather(void)
{
}

void ImageGather::prepareImage(GenericImage &img)
{
	imFactory.getImage(img);

	// Create an image header serialing the GenericImage content
	std::ostringstream outStream;
	boost::archive::text_oarchive outArchive(outStream);	
	outArchive << img;
	img.setHeader(outStream.str());

	std::cout << "Image header:" << outStream.str() << std::endl;
	std::cout << "Image has " << img.getSize() << " bytes..." << std::endl;

}
