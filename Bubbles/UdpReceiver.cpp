#include "UdpReceiver.h"
#define BUFF_SIZE 1024

UdpReceiver::UdpReceiver(sf::UdpSocket& s) : socket(s)
{
}

void UdpReceiver::recv()
{
	sf::IpAddress recipient;
	unsigned short remote_port;
	char data[BUFF_SIZE];
	std::size_t received;
	std::memset(data, 0, BUFF_SIZE);
	//this receives the broadcast message from the client and then immediatley sends it back to the same port
	if (socket.receive(data, BUFF_SIZE, received, recipient, remote_port) != sf::Socket::Done)
	{
		std::cerr << "Problems receiving" << std::endl;
		exit(0);
	}
	std::string s = std::string(data);
	sf::Socket::Status status = socket.send(s.c_str(), s.length(), recipient, remote_port);
	if (status != sf::Socket::Done)
	{
		std::cerr << "Problems sending" << std::endl;
		exit(0);
	}
}
