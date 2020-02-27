#pragma once

#include <string>
#include "vector_2d.h"

class Map
{
public:

	Map();
	~Map();

	void load_map(std::string path, int size_x, int size_y);
	void add_tile(int id, int x, int y);
	void add_tile_col(std::string tag, int x, int y);

private:

};