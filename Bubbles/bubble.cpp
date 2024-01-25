#include "bubble.h"

//this sets the position of the bubble
void bubble::SetPosition(sf::Vector2f position)
{
	bubble.setPosition(position);
}
//this sets the colour of the bubble
void bubble::SetColour(sf::Color fillColour)
{
	bubble.setFillColor(fillColour);
}
//this sets the bubble isAlive  variable
void bubble::SetAlive(bool isAlive)
{
	alive = isAlive;
}
//this sets the bubble velocity
void bubble::SetVelocity(sf::Vector2f newVelocity)
{
	velocity = newVelocity;
}
//this sets the bubble radius
void bubble::SetRadius(float radius)
{
	bubble.setRadius(radius);
}
//this sets the bubble origin
void bubble::SetOrigin(sf::Vector2f origin)
{
	bubble.setOrigin(origin);
}
//this moves the bubble by the velocity
void bubble::Move()
{
	bubble.move(velocity);
}
//this gets the bubble position
sf::Vector2f bubble::GetPosition()
{
	sf::Vector2f bubblePosition = bubble.getPosition();
	sf::Vector2f AddRadius(20, 20);
	bubblePosition += AddRadius;
	return bubblePosition;
}
//this gets if the bubble is alive
bool bubble::IsAlive()
{
	return alive;
}
//this gets the bubble colour
sf::Color bubble::GetFillColour()
{
	return bubble.getFillColor();
}
