#include <zmq.hpp>
#include <string>
#include <iostream>
#include <sstream>

// Header do objeto de imagem generica
#include "GenericImage.h"

#include  <boost/lexical_cast.hpp>

int main()
{
	zmq::context_t context (1);

	//  Socket to talk to server
    std::cout << "Preparing to receive images from video broadcast..." << std::endl;
    zmq::socket_t subscriber (context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:8666");

	// Subscribe to message type 000001
	//subscriber.setsockopt(ZMQ_SUBSCRIBE, "00001 ", 6);
	
	// Subscribe to all incomming messages
	subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);

	while(true)
	{
		zmq::message_t update;
		subscriber.recv(&update);
		std::cout << "Message received size=" << update.size() << std::endl;

		// Recover header serialized
		std::string imageHeaderStr;
		GenericImage imageHeader;

		// Get first 5 bytes from message (header size)		
		char *p_MessageHeaderSize = static_cast<char*>(update.data());
		std::string imageHeaderSizeStr = std::string(p_MessageHeaderSize,5);
		int imageHeaderSize = boost::lexical_cast<int>(imageHeaderSizeStr);

		// Create string from header
		char *p_MessageHeader = static_cast<char*>(update.data());
		p_MessageHeader += 6;
		imageHeaderStr = std::string(p_MessageHeader, imageHeaderSize);

		std::istringstream inStream(imageHeaderStr);
		boost::archive::text_iarchive inArchive(inStream);
		inArchive >> imageHeader;

		std::cout << "BEGIN--------------------------------------------------" << std::endl
			<< "image.width=" << imageHeader.getWidth() << std::endl
			<< "image.height=" << imageHeader.getHeight() << std::endl
			<< "image.channels=" << imageHeader.getChannels() << std::endl
			<< "image.size=" << imageHeader.getSize() << std::endl
			<< "END-----------------------------------------------------" << std::endl;
		

		//std::cout << static_cast<char*>(update.data()) << std::endl;
		/*std::istringstream iss(static_cast<char*>(update.data()));
        iss >> zipcode >> temperature >> relhumidity ;*/
	}
}