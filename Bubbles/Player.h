#pragma once
#include "Grid.h"
#include "bubble.h"
class Player
{
public:
	float angle = 0;
	Grid grid;
	bubble playerBubble;
	sf::RectangleShape cannon;
	bool isCannonReady = true;
	int score = 0;
	int colour = 0;
	sf::Vector2f defaultPosition;
	void SetPosition(int x, int y);
	void SetVelocity(sf::Vector2f newVelocity);
	void AddScore(int scoreAdd);
	bool CheckCollision(float x1, float y1, float r1, float x2, float y2, float r2);
	void AddToGrid(bubble* otherBubble);
	void ResetPlayerBall(sf::Color newColour, sf::Vector2f position);
	bool Cluster(int row, int column);
	bool gameOver = false;
};

