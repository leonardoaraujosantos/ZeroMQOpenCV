#include <zmq.hpp>
#include <string>
#include <iostream>

#include "ImageGather.h"

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <boost/format.hpp>

int main(int ac, char* av[])
{	
	int use_video;					// Use camera or directory
	std::string publisher_mode;		// (tcp://*:8666) 
	std::string image_dir;			// Image directory used if input is directory
	std::string image_extension;	// Image type (.jpg, .tif, .bmp, .png)
	int debugMode;					// Display image 
	int equalizeHist;				// Use the histogram equalization
	std::string ROI;				// ROI applied to the image sent

	try
	{
		// Parse program options
		// Parse program options...
		po::options_description desc("Allowed options");
		desc.add_options()
            ("help,h", "produce help message")
			("Publisher_mode,p",  po::value<std::string>(&publisher_mode)->default_value("tcp://*:8666"), "UDP server port")
			("video,v",  po::value<int>(&use_video)->default_value(0), "Use camera flag") 
			("debug,g",  po::value<int>(&debugMode)->default_value(0), "Debug mode") 
			("equalize,q",  po::value<int>(&equalizeHist)->default_value(0), "Equalize histogram")
			("ROI,r", po::value<std::string>(&ROI)->default_value(""),"Apply ROI on image sent.")
			("directory,d", po::value<std::string>(&image_dir)->default_value("."),"Image directory.")
			("extension,e", po::value<std::string>(&image_extension)->default_value(".jpg"),"Image extension.")
        ;
		po::variables_map vm;        
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);

		if (vm.count("help")) 
		{
			std::cout << desc << std::endl;
            return 1;
        }
		if (vm.count("video")) 
		{            
			std::cout << "Video mode set to " << vm["video"].as<int>() << std::endl;
        }
		if (vm.count("Publisher_mode")) 
		{            
			std::cout << "Publisher mode set to " << vm["Publisher_mode"].as<std::string>() << std::endl;
        }
		if (vm.count("directory"))
        {            
			std::cout << "Image directory: " << vm["directory"].as<std::string>() << std::endl;			
        }
		if (vm.count("extension"))
        {            
			std::cout << "Image extension: " << vm["extension"].as<std::string>() << std::endl;			
        }

		// Initialize ImageFactory
		ImageFactory imFactory(use_video,image_dir,image_extension,debugMode,ROI,equalizeHist);

		// Initialize ImageGather (prepare images to be sent)
		ImageGather  imgGather(imFactory);
		
		// Initialize ZMQ
		zmq::context_t context (1);

		// Get socket to publish 
		zmq::socket_t publisher (context, ZMQ_PUB);

		// Bind the socket to some port (TCP,or IPC)
		publisher.bind(publisher_mode.c_str());			
		
		while(1)
		{						
			// Get image to be sent
			GenericImage image;
			imgGather.prepareImage(image);						
			
			// Create message content
			boost::format fmter("%05d ");
			fmter % image.getHeader().length();
			std::string headerSize = fmter.str();
			std::string completeHeader = headerSize + image.getHeader();
			//std::string completeHeader = std::string("00001 ") + std::string("header!");

			// Create a message
			//zmq::message_t message(completeHeader.length() /*+ image.getSize()*/);
			zmq::message_t message(completeHeader.length() + image.getSize() + 1);
			//zmq::message_t message(image.getSize());

			// Copy image header
			memcpy((char*) message.data(), completeHeader.c_str(), completeHeader.length());	
			
			// Copy image content
			char *postHeader = (char*) message.data();
			postHeader += completeHeader.length() + 1;			
			memcpy((char*)postHeader,image.getImageBuffer(), image.getSize()); 
			
			// Broadcast this message
			publisher.send(message);

			Sleep(1000);
		}
	}
	catch(std::exception &e)
	{		
		std::cerr << e.what() << std::endl;
	}
	std::cout << "Image broadcast closing" << std::endl;

	return 0;
}