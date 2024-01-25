#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>
#include "bubble.h"

class Grid
{
public:
	int columns = 14 ;
	int rows = 12;
	int radius = 20;
	std::vector<std::vector<bubble*>> g_tiles;
	bool Cluster(int row, int column);
	void DestroyCluster(int row, int column, sf::Color TargetColour);
	int destroyedBalls = 0;
	
};

