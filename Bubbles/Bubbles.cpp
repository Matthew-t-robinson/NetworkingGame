#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <iostream>
#include <sstream>
#include "Player.h"
#include "Grid.h"
#include "queue.hpp"
#include <vector>
#include <SFML/Network.hpp>
#include <thread>
#include "server.h"
#include <memory>
#include <cstring>

#include "receiver.h"

#define BUBBLE_SIZE 20
#define CANNON_H  60
#define CANNON_W  20
#define MAX_ANGLE 80
#define MIN_ANGLE 280
#define M_PI      3.141692654
#define VELOCITY  7
#define WINDOW_H  600
#define WINDOW_W  1200
#define BUFF_SIZE 1024
//this is a struc that stores the data that will be sent and received over the network
struct mesg2 {
    int colour;
    float velX;
    float velY;
    int score;
    float angle;
    bool isCannonReady;
};

int main(int argc, const char* argv[])
{
    bool gameRunning = true;
    sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "Bubble1");
    window.setFramerateLimit(60);
    //this is a udp broadcast. it will try and send a message to the port that the server will be at. if after a while it hasnt received an answer, it will start a new server
    Queue<message> queue;
    sf::IpAddress recipient = sf::IpAddress::Broadcast;
    server s;

    std::string msg = "broadcast message";
    sf::UdpSocket testServer;
    unsigned short remote_port = 4305;
    sf::Socket::Status status = testServer.send(msg.c_str(), msg.length(), recipient, remote_port);

    if (status != sf::Socket::Done)
    {
        std::cerr << "Problems sending" << std::endl;
        exit(0);
    }

    char data[BUFF_SIZE];
    std::size_t received;
    std::memset(data, 0, BUFF_SIZE);
    testServer.setBlocking(false);
    sf::sleep(sf::milliseconds(1500));
    if (testServer.receive(data, BUFF_SIZE, received, recipient, remote_port) != sf::Socket::Done)
    {
        std::cout << "starting server";
        std::thread(&server::gameServer, &s).detach();
        sf::sleep(sf::milliseconds(1000));
    }
    else
    {
        std::cout << data << std::endl;
    }

    testServer.setBlocking(true);
    //this connects the main socket to the server
    std::shared_ptr<sf::TcpSocket> tcpSocket = std::make_shared<sf::TcpSocket>();
    status = tcpSocket->connect(sf::IpAddress::getLocalAddress(), 4305);
    if (status != sf::Socket::Done)
    {
        std::cout << "Error connecting to server" << "\n";
        return 0;
    }
    std::cout << "Connected\n";
    //this is where the game receives the seed from the server for the main grid and all the future player balls. it also receives what number player it is
    sf::Packet seedPacket;
    status = tcpSocket->receive(seedPacket);

    if (status != sf::Socket::Done)
    {
        std::cout << "Error receiving seed" << "\n";
        return 0;
    }
    std::cout << "received seed";
    int playerN = 0;
    unsigned int seed = 0;
    seedPacket >> playerN >> seed;
    std::cout << "seed: " << seed << " player number: " << playerN << std::endl;
    srand(seed);
    //this is where the receiver thread is started for the player. all the receving of data is handled here
    std::shared_ptr<Receiver> receiver = std::make_shared<Receiver>(tcpSocket, queue);
    std::thread(&Receiver::recv_loop, receiver).detach();
    // this stores all the possible colours
    const sf::Color colours[5] = {
        sf::Color::Red,
        sf::Color(127, 192, 127),
        sf::Color::Blue,
        sf::Color(192, 192, 127),
        sf::Color(127, 192, 192),
    };
    Player* player1 = new Player;
    Player* player2 = new Player;
    //this creates the player 1 grid
    int initialRows = player1->grid.rows;
    int initialColumns = player1->grid.columns;
    for (size_t i = 0; i < (size_t)initialRows + 4; i++) {
        std::vector<bubble*> newVector;
        for (size_t j = 0; j < initialColumns; j++) {
            bubble* newBubble = new bubble;
            newBubble->SetRadius(BUBBLE_SIZE);
            newBubble->position = sf::Vector2f(BUBBLE_SIZE * 2 * (float)j + (i % 2) * BUBBLE_SIZE, (float)i * 33);

            newBubble->SetPosition(newBubble->position);
            newBubble->gridPosition = sf::Vector2i((int)i, (int)j);
            int points = rand() % 5;
            newBubble->SetColour(colours[points]);
            player1->colour = points;
            if (i >= player1->grid.rows)
            {
                newBubble->SetAlive(false);
            }
            newVector.push_back(newBubble);
        }
        player1->grid.g_tiles.push_back(newVector);
    }
    //this sets the size, origin and position of the player 1 cannon
    player1->cannon.setSize(sf::Vector2f(CANNON_W, CANNON_H));
    player1->cannon.setOrigin(CANNON_W / 2, CANNON_H);
    sf::Vector2f p1_pos;

    p1_pos = sf::Vector2f(WINDOW_W / 4, (WINDOW_H));
    player1->cannon.setPosition(p1_pos);

    //this creates the player 2 grid
    initialRows = player2->grid.rows;
    initialColumns = player2->grid.columns;
    for (size_t i = 0; i < (size_t)initialRows + 4; i++) {
        std::vector<bubble*> newVector;
        for (size_t j = 0; j < initialColumns; j++) {
            bubble* newBubble = new bubble;
            newBubble->SetRadius(BUBBLE_SIZE);
            newBubble->position = sf::Vector2f(BUBBLE_SIZE * 2 * (float)j + (i % 2) * BUBBLE_SIZE, (float)i * 33);
            newBubble->position.x += WINDOW_W / 2;
            newBubble->SetPosition(newBubble->position);
            newBubble->gridPosition = sf::Vector2i((int)i, (int)j);
            int points = rand() % 5;
            newBubble->SetColour(colours[points]);
            player2->colour = points;
            if (i >= player2->grid.rows)
            {
                newBubble->SetAlive(false);
            }
            newVector.push_back(newBubble);
        }
        player2->grid.g_tiles.push_back(newVector);
    }
    //this sets the size, origin and position of the player 2 grid
    player2->cannon.setSize(sf::Vector2f(CANNON_W, CANNON_H));
    player2->cannon.setOrigin(CANNON_W / 2, CANNON_H);
    sf::Vector2f p2_pos;

    p2_pos = sf::Vector2f(3 * WINDOW_W / 4, (WINDOW_H));


    player2->cannon.setPosition(p2_pos);

    //this creates the wall in the middle of the window

    sf::RectangleShape wall(sf::Vector2f(1, WINDOW_H));
    wall.setPosition(WINDOW_W / 2, 0);

    //this is where the player bubbles for both players are set up.
    player1->playerBubble.SetRadius(BUBBLE_SIZE);
    player1->playerBubble.SetOrigin(sf::Vector2f(BUBBLE_SIZE, BUBBLE_SIZE));
    player1->playerBubble.SetPosition(p1_pos);
    player1->defaultPosition = p1_pos;
    player1->playerBubble.SetColour(colours[rand() % 5]);


    player2->playerBubble.SetRadius(BUBBLE_SIZE);
    player2->playerBubble.SetOrigin(sf::Vector2f(BUBBLE_SIZE, BUBBLE_SIZE));
    player2->playerBubble.SetPosition(p2_pos);
    player2->defaultPosition = p2_pos;
    player2->playerBubble.SetColour(colours[rand() % 5]);

    //loads the font thats used for the scores and when you win or lose
    sf::Font font;
    if (!font.loadFromFile("PressStart2P.ttf"))
    {
        return 1;
    }
    //this uses your player number to work out if your player 1 or player 2 and sets you to be the grid on the left if your player 1 and the grid on the right if your player 2
    Player* currentPlayer = new Player;
    Player* otherPlayer = new Player;
    if (playerN > 1)
    {
        currentPlayer = player2;
        otherPlayer = player1;
    }
    else
    {
        currentPlayer = player1;
        otherPlayer = player2;
    }
    sf::Text score1;
    sf::Text score2;
    float otherRows = (float)otherPlayer->grid.rows;
    float currentRows = (float)currentPlayer->grid.rows;
    //this is the main game loop
    while (window.isOpen())
    {
        //this is the game loop for when the players are alive
        while (gameRunning)
        {
            //This checks to see how many of the current players ball are alive
            int aliveCurrent = 0;

            for (size_t i = 0; i < (size_t)currentRows + 4; i++)
            {
                for (size_t j = 0; j < currentPlayer->grid.columns; j++)
                {
                    if (currentPlayer->grid.g_tiles[i][j]->alive == true)
                    {
                        aliveCurrent++;
                    }
                }
            }

            if (playerN == 1)
            {
                score1.setString(std::to_string(currentPlayer->score));
                score1.setPosition(WINDOW_W / 2 - 70, 10);
                score1.setCharacterSize(30);
                score1.setFont(font);
                score2.setString(std::to_string(otherPlayer->score));
                score2.setPosition(WINDOW_W / 2 + 70, 10);
                score2.setCharacterSize(30);
                score2.setFont(font);
            }
            else
            {
                score1.setString(std::to_string(otherPlayer->score));
                score1.setPosition(WINDOW_W / 2 - 70, 10);
                score1.setCharacterSize(30);
                score1.setFont(font);
                score2.setString(std::to_string(currentPlayer->score));
                score2.setPosition(WINDOW_W / 2 + 70, 10);
                score2.setCharacterSize(30);
                score2.setFont(font);
            }
            sf::Event e;
            while (window.pollEvent(e))
            {
                if (e.type == sf::Event::Closed) {
                    window.close();
                }
            }
            //this handles the input for the local player. for testing reasons if the player is player 2 they must use the arrow keys
            currentPlayer->angle = currentPlayer->cannon.getRotation();
            float angle = currentPlayer->angle;
            if (playerN == 1)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && (currentPlayer->angle > MIN_ANGLE + 1 || currentPlayer->angle < MAX_ANGLE + 1))
                    currentPlayer->cannon.rotate(-1);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && (currentPlayer->angle > MIN_ANGLE - 1 || currentPlayer->angle < MAX_ANGLE - 1))
                    currentPlayer->cannon.rotate(1);

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && currentPlayer->isCannonReady)
                {
                    currentPlayer->angle = currentPlayer->cannon.getRotation();
                    currentPlayer->SetVelocity(sf::Vector2f((float)(-cos(((double)angle + 90) * M_PI / 180) * VELOCITY), (float)(-sin(((double)angle + 90) * M_PI / 180) * VELOCITY)));
                    currentPlayer->isCannonReady = false;
                }
            }
            else
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && (currentPlayer->angle > MIN_ANGLE + 1 || currentPlayer->angle < MAX_ANGLE + 1))
                    currentPlayer->cannon.rotate(-1);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && (currentPlayer->angle > MIN_ANGLE - 1 || currentPlayer->angle < MAX_ANGLE - 1))
                    currentPlayer->cannon.rotate(1);

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && currentPlayer->isCannonReady)
                {
                    currentPlayer->angle = currentPlayer->cannon.getRotation();
                    currentPlayer->SetVelocity(sf::Vector2f((float)(-cos(((double)angle + 90) * M_PI / 180) * VELOCITY), (float)(-sin(((double)angle + 90) * M_PI / 180) * VELOCITY)));
                    currentPlayer->isCannonReady = false;
                }
            }
            //this handles the movement for the local player. If it hits the wall at either side it will reverse the x velocity to make it bounce off
            if (currentPlayer->playerBubble.velocity.x != 0 && currentPlayer->playerBubble.velocity.y != 0)
            {
                currentPlayer->playerBubble.Move();
                sf::Vector2f pos = currentPlayer->playerBubble.bubble.getPosition();
                if (playerN > 1)
                {
                    if (pos.x < WINDOW_W / 2 + BUBBLE_SIZE || pos.x > WINDOW_W - BUBBLE_SIZE)
                    {
                        currentPlayer->playerBubble.velocity.x = -currentPlayer->playerBubble.velocity.x;
                    }
                }
                else
                {
                    if (pos.x < BUBBLE_SIZE || pos.x > WINDOW_W / 2 - BUBBLE_SIZE)
                    {
                        currentPlayer->playerBubble.velocity.x = -currentPlayer->playerBubble.velocity.x;
                    }
                }
                //this checks through each ball in the grid and checks to see if it is colliding with the player ball
                for (size_t i = 1; i < currentRows + 4; i++)
                {
                    for (size_t j = 0; j < currentPlayer->grid.columns; j++)
                    {
                        if (currentPlayer->grid.g_tiles[i][j]->alive)
                        {

                            float posX = currentPlayer->playerBubble.bubble.getPosition().x + BUBBLE_SIZE;
                            float posY = currentPlayer->playerBubble.bubble.getPosition().y + BUBBLE_SIZE;
                            float posX2 = currentPlayer->grid.g_tiles[i][j]->bubble.getPosition().x + BUBBLE_SIZE;
                            float posY2 = currentPlayer->grid.g_tiles[i][j]->bubble.getPosition().y + BUBBLE_SIZE;


                            if (currentPlayer->CheckCollision(posX, posY, BUBBLE_SIZE, posX2, posY2, BUBBLE_SIZE))
                            {
                                //this adds the player ball to the grid, gets rid of any clusters and resets the player ball
                                currentPlayer->AddToGrid(currentPlayer->grid.g_tiles[i][j]);
                                int points = rand() % 5;
                                currentPlayer->colour = points;
                                currentPlayer->score = currentPlayer->grid.destroyedBalls;
                                currentPlayer->ResetPlayerBall(colours[points], currentPlayer->defaultPosition);
                            }
                        }
                    }
                }

            }
            //this checks to see if the player grid has any balls low enough, if it does they lose.
            for (size_t i = 14; i < currentRows + 3; i++)
            {
                for (size_t j = 0; j < currentPlayer->grid.columns; j++)
                {
                    if (currentPlayer->grid.g_tiles[i][j]->IsAlive())
                    {
                        currentPlayer->gameOver = true;

                    }
                }
            }
            //this checks to see if the loop earlier found any alive balls in the local players grid
            if (aliveCurrent == 0)
            {
                otherPlayer->gameOver = true;
            }
            if (currentPlayer->score > 5)
            {
                otherPlayer->gameOver = true;
            }
            if (otherPlayer->score > 5)
            {
                currentPlayer->gameOver = true;
            }
            //this packs all the necessary data into a packet and sends it to the server
            mesg2 m;
            sf::Packet sendToOtherPlayer;
            sf::Vector2f playerPosition = currentPlayer->playerBubble.GetPosition();
            int colourNum = 0;
            m.angle = currentPlayer->angle;
            m.score = currentPlayer->score;
            m.velX = currentPlayer->playerBubble.velocity.x;
            m.velY = currentPlayer->playerBubble.velocity.y;
            m.colour = currentPlayer->colour;
            m.isCannonReady = currentPlayer->isCannonReady;
            std::cout << m.velX << " " << m.velY << std::endl;
            sendToOtherPlayer << m.colour << m.angle << m.score << m.velX << m.velY << m.isCannonReady;
            if (tcpSocket->send(sendToOtherPlayer) != sf::Socket::Done)
            {
                std::cout << "couldn't sent to other player" << std::endl;
            }



            //this pops the received packet out of the queue fpr the player to use
            auto q = queue.pop();
            sf::Packet receivePacket = std::get<1>(q);
            receivePacket >> m.colour >> m.angle >> m.score >> m.velX >> m.velY >> m.isCannonReady;
            //this sets the other cannon to be the right angle
            otherPlayer->angle = m.angle;
            otherPlayer->cannon.setRotation(otherPlayer->angle);
            //this sets the other velocity to be equal to the incoming velocity from the packet if the player isnt already moving. if it is it keeps the same velocity 
            sf::Vector2f oldVelocity(otherPlayer->playerBubble.velocity);
            sf::Vector2f otherVelocity(m.velX, m.velY);
            bool canon = m.isCannonReady;
            if (canon == false && otherPlayer->isCannonReady == true)
            {
                otherPlayer->isCannonReady = false;
            }
            if (canon == true && otherPlayer->isCannonReady == false)
            {
                otherVelocity = oldVelocity;
            }

            // this checks to see if there are any alive balls in the other grid
            otherPlayer->SetVelocity(otherVelocity);
            int aliveOther = 0;
            for (size_t i = 0; i < currentRows + 4; i++)
            {
                for (size_t j = 0; j < currentPlayer->grid.columns; j++)
                {
                    if (currentPlayer->grid.g_tiles[i][j]->alive == true)
                    {
                        aliveOther++;
                    }
                }
            }
            //this checks for the other players collision with the grid. it was easier to handle this locally instead of sending the values between the players.
            //it also moves the player ball and bounces it off the wall
            if (otherPlayer->playerBubble.velocity.x != 0 && otherPlayer->playerBubble.velocity.y != 0)
            {
                otherPlayer->playerBubble.Move();
                sf::Vector2f pos = otherPlayer->playerBubble.bubble.getPosition();
                if (playerN > 1)
                {
                    if (pos.x < WINDOW_W / 2 + BUBBLE_SIZE || pos.x > WINDOW_W - BUBBLE_SIZE)
                    {
                        otherPlayer->playerBubble.velocity.x = -otherPlayer->playerBubble.velocity.x;
                    }
                }
                else
                {
                    if (pos.x < BUBBLE_SIZE || pos.x > WINDOW_W / 2 - BUBBLE_SIZE)
                    {
                        otherPlayer->playerBubble.velocity.x = -otherPlayer->playerBubble.velocity.x;
                    }
                }

                for (size_t i = 1; i < otherRows + 4; i++)
                {
                    for (size_t j = 0; j < otherPlayer->grid.columns; j++)
                    {
                        if (otherPlayer->grid.g_tiles[i][j]->alive)
                        {
                            float posX = otherPlayer->playerBubble.bubble.getPosition().x + (BUBBLE_SIZE * 2);
                            float posY = otherPlayer->playerBubble.bubble.getPosition().y + (BUBBLE_SIZE * 2);
                            float posX2 = otherPlayer->grid.g_tiles[i][j]->bubble.getPosition().x + (BUBBLE_SIZE * 2);
                            float posY2 = otherPlayer->grid.g_tiles[i][j]->bubble.getPosition().y + (BUBBLE_SIZE * 2);


                            if (otherPlayer->CheckCollision(posX, posY, BUBBLE_SIZE, posX2, posY2, BUBBLE_SIZE))
                            {

                                otherPlayer->AddToGrid(otherPlayer->grid.g_tiles[i][j]);

                                otherPlayer->ResetPlayerBall(colours[m.colour], otherPlayer->defaultPosition);
                                colourNum = m.colour;
                            }
                        }
                    }
                }
            }
            //this checks to see if the other players grid has gotten too low, which will end the game if it has 
            for (size_t i = 14; i < otherRows + 3; i++)
            {
                for (size_t j = 0; j < otherPlayer->grid.columns; j++)
                {
                    if (otherPlayer->grid.g_tiles[i][j]->IsAlive())
                    {
                        otherPlayer->gameOver = true;

                    }
                }
            }
            if (aliveOther == 0)
            {
                currentPlayer->gameOver = true;
            }
            if (otherPlayer->gameOver || currentPlayer->gameOver)
            {
                gameRunning = false;
            }
            //this draws everything on to the screen, including the grids, walls cannons and player bubbles
            currentPlayer->score = currentPlayer->grid.destroyedBalls;
            otherPlayer->score = otherPlayer->grid.destroyedBalls;
            window.clear();
            window.draw(currentPlayer->cannon);
            window.draw(otherPlayer->cannon);
            window.draw(wall);
            window.draw(score1);
            window.draw(score2);
            window.draw(currentPlayer->playerBubble.bubble);
            window.draw(otherPlayer->playerBubble.bubble);
            for (size_t i = 1; i < currentRows + 4; i++) {
                for (size_t j = 0; j < currentPlayer->grid.columns; j++) {
                    if (currentPlayer->grid.g_tiles[i][j]->alive)
                    {
                        window.draw(currentPlayer->grid.g_tiles[i][j]->bubble);
                    }
                }
            }
            for (size_t i = 1; i < otherRows + 4; i++) {
                for (size_t j = 0; j < otherPlayer->grid.columns; j++) {
                    if (otherPlayer->grid.g_tiles[i][j]->alive)
                    {
                        window.draw(otherPlayer->grid.g_tiles[i][j]->bubble);
                    }
                }
            }
            window.display();
        }
        //this keeps sending data to the other player as if it doesnt the game will stop before it ends as its no longer receiving data
        mesg2 m;
        sf::Packet sendToOtherPlayer;
        sf::Vector2f playerPosition = currentPlayer->playerBubble.GetPosition();
        int colourNum = 0;
        m.angle = currentPlayer->angle;
        m.score = currentPlayer->score;
        m.velX = currentPlayer->playerBubble.velocity.x;
        m.velY = currentPlayer->playerBubble.velocity.y;
        m.colour = currentPlayer->colour;

        m.isCannonReady = currentPlayer->isCannonReady;
        std::cout << m.velX << " " << m.velY << std::endl;
        sendToOtherPlayer << m.colour << m.angle << m.score << m.velX << m.velY << m.isCannonReady;
        if (tcpSocket->send(sendToOtherPlayer) != sf::Socket::Done)
        {
            std::cout << "couldn't sent to other player" << std::endl;
        }
        //this displays whether the player one or lost
        window.clear();
        sf::Text winner;
        winner.setFont(font);
        winner.setPosition(sf::Vector2f(WINDOW_W / 2, WINDOW_H / 2));
        winner.setCharacterSize(30);
        if (currentPlayer->gameOver == true)
        {
            winner.setString("YOU LOSE");
        }
        else
        {
            winner.setString("YOU WIN");
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }
        window.draw(winner);
        window.display();
    }
    return 0;
}
