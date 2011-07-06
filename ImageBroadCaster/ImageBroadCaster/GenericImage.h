#pragma once

#include <vector>

// include headers that implement a archive in simple text format
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

// OpenCv Headers
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class GenericImage
{
public:
	GenericImage(void);
	GenericImage(int w, int h, int ws,int channels, char *image_data);
	~GenericImage(void);
	char *getImageBuffer();
	int getSize() { return size; };
	int getWidth() { return width; };
	int getHeight() { return height; };
	int getStep() { return width_step; };
	int getChannels() { return nchannels; };
	void setImage(cv::Mat image);
private:
	friend class boost::serialization::access;
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & width;
        ar & height;
        ar & width_step;
		ar & size;
		ar & nchannels;
		//ar & ImmBuffer;
    }

	int width;
	int height;
	int width_step;
	int size;
	int nchannels;
	//std::vector<char> ImmBuffer;
	char *ImmBuffer;
};
