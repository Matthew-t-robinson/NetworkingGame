#pragma once
#include <cstring>
#include <iostream>
#include <iostream>
#include <sstream>
#include <thread>
#include<SFML/Network.hpp>
class UdpReceiver
{
public:
	UdpReceiver(sf::UdpSocket& s);
	void recv();
private:
	sf::UdpSocket& socket;
};

