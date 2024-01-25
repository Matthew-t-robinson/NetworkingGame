#include "Grid.h"

bool Grid::Cluster(int row, int column)
{
	//this checks every bubble around the ball that was just added to see if there are any balls of the same colour next to it
	//if there is one then it destroys the cluster
	sf::Color targetColour = g_tiles[row][column]->GetFillColour();
	
	if (row > 0 && (g_tiles[row - 1][column]->IsAlive()) && (g_tiles[row - 1][column]->GetFillColour() == targetColour)
		|| (row < (rows + 4) - 1 && g_tiles[row + 1][column]->IsAlive() && g_tiles[row + 1][column]->GetFillColour() == targetColour)
		|| (column > 0 && g_tiles[row][column - 1]->IsAlive() && g_tiles[row][column - 1]->GetFillColour() == targetColour)
		|| (column < columns - 1 && g_tiles[row][column + 1]->IsAlive() && g_tiles[row][column + 1]->GetFillColour() == targetColour)){
		if (row % 2 != 0 && g_tiles[row +  1][column + 1]->IsAlive() && g_tiles[row - 1][column + 1]->GetFillColour() == targetColour)
		{
			DestroyCluster(row, column, targetColour);
		}
		else if (row % 2 == 0 && g_tiles[row + 1][column - 1]->IsAlive() && g_tiles[row - 1][column - 1]->GetFillColour() == targetColour)
		{
			DestroyCluster(row, column, targetColour);
		}
		else
		{
			DestroyCluster(row, column, targetColour);
		}
		return true;
	}
	return false;
}

void Grid::DestroyCluster(int row, int column, sf::Color TargetColour)
{
	//this checks if the ball is the right colour, if the ball position is balid and if it is alive
	if (row < 0) return;
	if (row >= 4 + rows) return;
	if (column < 0) return;
	if (column >= columns) return;
	if (!g_tiles[row][column]->IsAlive()) return;
	if (g_tiles[row][column]->GetFillColour() != TargetColour) return;
	//it will then set the bubble to not be alive and then will call for the bubbles to be checked for the same thing

	g_tiles[row][column]->SetAlive(false);
	destroyedBalls++;
	DestroyCluster(row, column + 1, TargetColour);
	DestroyCluster(row, column - 1, TargetColour);
	DestroyCluster(row + 1, column, TargetColour);
	DestroyCluster(row - 1, column, TargetColour);
	if (row % 2 != 0)
	{
		DestroyCluster(row - 1, column + 1, TargetColour);
	}
	else
	{
		DestroyCluster(row - 1, column - 1, TargetColour);
	}
}
