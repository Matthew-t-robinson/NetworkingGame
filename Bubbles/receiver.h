#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#include <SFML/Network.hpp>
#include "queue.hpp"
#include"util.hpp"

class Receiver {
public:
    Receiver(std::shared_ptr<sf::TcpSocket> s, Queue < message >& queue);
    void recv_loop();
private:
    std::shared_ptr<sf::TcpSocket> socket_;
    Queue<message>& queue_;
};
#endif

