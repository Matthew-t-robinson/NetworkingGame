#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>

class bubble
{
public:
	sf::Vector2f velocity;
	sf::Vector2f position;
	bool alive = true;
	sf::CircleShape bubble;
	sf::Vector2i gridPosition;
	void SetPosition(sf::Vector2f position);
	void SetColour(sf::Color fillColour);
	void SetAlive(bool alive);
	void SetVelocity(sf::Vector2f newVelocity);
	void SetRadius(float radius);
	void SetOrigin(sf::Vector2f origin);
	void Move();
	sf::Vector2f GetPosition();
	bool IsAlive();
	sf::Color GetFillColour();
};