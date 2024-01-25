#ifndef ACCEPTER_HPP
#define ACCEPTER_HPP

#include "queue.hpp"
#include "list.hpp"
#include"util.hpp"
#include <SFML/Network.hpp>

class Accepter {
private:
    Queue<message>& queue_;
    List<std::shared_ptr<sf::TcpSocket>>& socket_;

public:
    Accepter(Queue<message>& q, List<std::shared_ptr<sf::TcpSocket>>& s);
    void operator()();
};

#endif

