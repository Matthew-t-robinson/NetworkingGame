#include "Player.h"
//this sets the player bubble position
void Player::SetPosition(int x, int y)
{
	playerBubble.SetPosition(sf::Vector2f((float)x, (float)y));
	
}
//this sets the player bubble velocity
void Player::SetVelocity(sf::Vector2f newVelocity)
{
	playerBubble.SetVelocity(newVelocity);
}
//this adds the the players score
void Player::AddScore(int scoreAdd)
{
	score += scoreAdd;
}
//this checks if 2 bubbles have collided
bool Player::CheckCollision(float x1, float y1, float r1, float x2, float y2, float r2)
{

	if (((y1 - (20 + y2)) * (y1 - (20 + y2))) + ((x1 - (20 + x2)) * (x1 - (20 + x2))) <= 20 * 4 * 20)
	{
		return true;
		
	}
	
	return false;
}
//this checks to see which grid position is the closest. it checks the distane between the bubble and the grid position and if it is closer
//than the already stored closest position, then it will store this as the new closest position and will store the row and column of this positon
//at the end it will add the bubble to the grid at the position of the closest position
void Player::AddToGrid(bubble* otherBubble)
{
	float centerX = playerBubble.bubble.getPosition().x + grid.radius;
	float centerY = playerBubble.bubble.getPosition().y + grid.radius;
	float centerX2 = otherBubble->bubble.getPosition().x + grid.radius;
	float centerY2 = otherBubble->bubble.getPosition().y + grid.radius;
	sf::Vector2i gridPosition = sf::Vector2i(otherBubble->gridPosition.x, otherBubble->gridPosition.y);
	float gridPositionX = (float)gridPosition.x;
	float gridPositionY = (float)gridPosition.y;

	float distance = sqrtf(powf(playerBubble.GetPosition().x - otherBubble->GetPosition().x, 2) +
		powf(playerBubble.GetPosition().y - otherBubble->GetPosition().y, 2));
	playerBubble.bubble.getRadius();
	float closestDistance = 1000;
	float nextDistance;
	int closestRow = gridPosition.x, closestColumn = gridPosition.y;
	if (gridPosition.x < (grid.rows + 4) - 1 && !grid.g_tiles[(int64_t)gridPositionX + 1][(int64_t)gridPosition.y]->alive)
	{

		nextDistance = sqrtf(powf((playerBubble.GetPosition().x - grid.g_tiles[(int64_t)gridPositionX + 1][(int64_t)gridPositionY]->GetPosition().x), 2) + powf((playerBubble.bubble.getPosition().y - grid.g_tiles[(int64_t)gridPositionX + 1][(int64_t)gridPositionY]->bubble.getPosition().y), 2));
		if (closestDistance > nextDistance)
		{
			closestDistance = nextDistance;
			closestRow = gridPosition.x + 1;
			closestColumn = gridPosition.y;
		}
	}
	if (gridPosition.y > 0 && !grid.g_tiles[(int64_t)gridPositionX][(int64_t)gridPositionY - 1]->alive)
	{
		nextDistance = sqrtf(powf((playerBubble.bubble.getPosition().x - grid.g_tiles[(int64_t)gridPositionX][(int64_t)gridPositionY - 1]->bubble.getPosition().x), 2) + powf((playerBubble.bubble.getPosition().y - grid.g_tiles[(int64_t)gridPositionX][(int64_t)gridPositionY - 1]->bubble.getPosition().y), 2));
		if (closestDistance > nextDistance)
		{
			closestDistance = nextDistance;
			closestRow = gridPosition.x;
			closestColumn = gridPosition.y - 1;
		}
	}
	if (gridPosition.y < grid.columns - 1 && !grid.g_tiles[(int64_t)gridPositionX][(int64_t)gridPositionY + 1]->alive)
	{
		nextDistance = sqrtf(powf((playerBubble.bubble.getPosition().x - grid.g_tiles[(int64_t)gridPositionX][(int64_t)gridPositionY + 1]->GetPosition().x), 2) + powf((playerBubble.bubble.getPosition().y - grid.g_tiles[(int64_t)gridPositionX][(int64_t)gridPositionY + 1]->bubble.getPosition().y), 2));
		if (closestDistance > nextDistance)
		{
			closestDistance = nextDistance;
			closestRow = gridPosition.x;
			closestColumn = gridPosition.y + 1;
			
		}
	}
	playerBubble.SetPosition(grid.g_tiles[closestRow][closestColumn]->bubble.getPosition());
	grid.g_tiles[closestRow][closestColumn]->SetColour(playerBubble.bubble.getFillColor());
	grid.g_tiles[closestRow][closestColumn]->SetAlive(true);
	//it will now see if there is a cluster and then destroy it 
	if (grid.Cluster(closestRow, closestColumn))
	{
		
		grid.DestroyCluster(closestRow, closestColumn, playerBubble.GetFillColour());
		
		isCannonReady = true;
	}
	
	isCannonReady = true;
	
	
	
}
//this resests the plahyre ball'
void Player::ResetPlayerBall(sf::Color newColour, sf::Vector2f position)
{
	playerBubble.SetColour(newColour);
	playerBubble.SetPosition(position);
	playerBubble.SetVelocity(sf::Vector2f(0, 0));
}


