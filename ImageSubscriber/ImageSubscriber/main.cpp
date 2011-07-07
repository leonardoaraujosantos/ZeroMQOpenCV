#include <zmq.hpp>
#include <string>
#include <iostream>
#include <sstream>

int main()
{
	zmq::context_t context (1);

	//  Socket to talk to server
    std::cout << "Preparing to receive images from video broadcast..." << std::endl;
    zmq::socket_t subscriber (context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:8666");

	subscriber.setsockopt(ZMQ_SUBSCRIBE, "00001 ", 6);

	while(true)
	{
		zmq::message_t update;
		subscriber.recv(&update);
		std::cout << static_cast<char*>(update.data()) << std::endl;
		/*std::istringstream iss(static_cast<char*>(update.data()));
        iss >> zipcode >> temperature >> relhumidity ;*/
	}
}