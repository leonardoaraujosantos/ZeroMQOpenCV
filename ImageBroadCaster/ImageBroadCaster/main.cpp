#include <zmq.hpp>
#include <string>
#include <iostream>

int main()
{
	
	// Initialize ZMQ
	zmq::context_t context (1);

	// Get socket to publish 
	zmq::socket_t publisher (context, ZMQ_PUB);

	// Bind the socket to some port (TCP,or IPC)
	publisher.bind("tcp://*:8666");
	
	while(1)
	{
		Sleep(1000);
		std::cout << "Send message" << std::endl;
		// Send message
		std::string msgText = "Video";
		zmq::message_t message(20);
		sprintf ((char *) message.data() ,"%05d %s", 1, msgText.c_str());	
		std::cout << static_cast<char*>(message.data()) << std::endl;
		publisher.send(message);
	}

	return 0;
}