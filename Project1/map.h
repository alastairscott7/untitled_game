#pragma once

#include <string>
#include "vector_2d.h"

class Map
{
public:

	Map();
	~Map();

	void LoadMap(std::string path, int sizeX, int sizeY);
	void CheckBounds(Vector2D player_pos);
	void AddTile(int id, int x, int y);
	void AddTileCol(std::string tag, int x, int y);

private:

};