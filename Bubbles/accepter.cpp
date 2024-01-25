#include "accepter.h"
#include "receiver.h"
#include <iostream>
#include <sstream>
#include <thread>

Accepter::Accepter(Queue<message>& q, List<std::shared_ptr<sf::TcpSocket>>& s) :
    queue_(q),
    socket_(s)
{}

void Accepter::operator()()
{
    int player = 1;
    //this sets the seed that the players will use
    unsigned int seed = static_cast<unsigned int>(time(NULL));
    sf::TcpListener listener;
    //this listens to the port that the server is at
    sf::Socket::Status listen = listener.listen(4305);
    if (listen != sf::Socket::Done)
    {
        std::cerr << "Error listening to port \n";
        return;
    }
    std::cout << "Bound to port\n";
    while (true)
    {
        std::shared_ptr<sf::TcpSocket> socket = std::make_shared<sf::TcpSocket>();
        //this accepts a connection from a socket trying to connect to the server port
        if (listener.accept(*socket) != sf::Socket::Done)
        {
            std::cout << "shut up\n";
            return;
        }
        socket_.push(socket);
        std::stringstream ss;
        ss << "Accepted a connection from: "
            << socket->getRemoteAddress()
            << ":"
            << socket->getRemotePort()
            << std::endl;
        std::cout << ss.str();
        //this stops the receiver thread that will receive the packets for the server
        std::shared_ptr<Receiver> receiver = std::make_shared<Receiver>(socket, queue_);
        //this sends the seed to the player that as just been accepted
        sf::Packet seedName;
        seedName << player << seed;
        sf::Packet seedSend = seedName;
        seedSend << player << seed;
        sf::Socket::Status statusSeed = socket->send(seedSend);
        if (statusSeed != sf::Socket::Done)
        {

        }
        player++;
        //this launches the receiver thread for the server
        std::thread(&Receiver::recv_loop, receiver).detach();
    }
}