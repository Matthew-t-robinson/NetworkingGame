#include "receiver.h"
#include "util.hpp"
#include <chrono>
#include <cstring>
#include <iostream>
#include <iostream>
#include <sstream>
#include <thread>

Receiver::Receiver(std::shared_ptr<sf::TcpSocket> s,
    Queue<message>& q) :
    socket_(s),
    queue_(q)
{
}

void Receiver::recv_loop()
{
    //this receives the packet and adds it to the queue along with the socket that sent it
    sf::Packet buffer;
    while (1)
    {
        sf::Socket::Status receive = socket_->receive(buffer);
        if ( receive != sf::Socket::Done)
        {
            std::cout << "Error receiving message\n";
            return;
        }
        sf::TcpSocket* t = socket_.get();
        queue_.push(std::make_pair(socket_.get(),buffer));
    }
}

