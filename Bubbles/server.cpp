#include "server.h"
#include "accepter.h"
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include "queue.hpp"
#include "list.hpp"
#include"receiver.h"
#include"util.hpp"
#include"UdpReceiver.h"

#define BUFF_SIZE 1024
struct mesg3 {
	int colour;
	float posX;
	float posY;
	float velX;
	float velY;
	int score;
	int angle;
	bool isCannonReady;
	bool gameOver;
};
void server::gameServer()
{
	Queue<message> queue;
	List<std::shared_ptr<sf::TcpSocket>> sockets;
	sf::UdpSocket socket;
	unsigned int local_port = PORT;
	//this binds the socket to the port of the server
	if (socket.bind(local_port) != sf::Socket::Done)
	{
		std::cerr << "could not bind to port";
		return;
	}
	//this starts the accepter thread
	std::thread(Accepter(queue, sockets)).detach();
	//this starts the receiver thread specifically for the udp echo at the start of the game
	std::shared_ptr<UdpReceiver> receiver = std::make_shared<UdpReceiver>(socket);
	std::thread(&UdpReceiver::recv, receiver).detach();
	sf::IpAddress recipient;
	
	
	while (1)
	{
		//this pops the pair of the socket and packet and then unpacks it
		auto p = queue.pop();
		sf::Packet packet = std::get<1>(p);
		sf::TcpSocket* otherSocket = std::get<0>(p);
		auto sendToOne = [&packet, &otherSocket](std::shared_ptr<sf::TcpSocket> socket) {
			sf::TcpSocket* currentSocket = socket.get();
			 
			//this checks to see if the socket is the same on it received it from before it sends it
			if (currentSocket != otherSocket)
			{
				sf::Socket::Status status = socket->send(packet);
				if (status != sf::Socket::Done)
				{
					/* code */
				}
			}
			else
			{
				return;
			}
			
		};
		sockets.for_each(sendToOne);
		
		
		
	}
}
