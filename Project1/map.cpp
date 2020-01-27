#include "map.h"
#include "game.h"
#include <fstream>

Map::Map()
{
}

Map::~Map()
{

}

void Map::LoadMap(std::string path, int sizeX, int sizeY)
{
	char tile;
	std::fstream map_file;
	map_file.open(path);

	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			map_file.get(tile);
			Game::AddTile(atoi(&tile), x * 64, y * 64);
			map_file.ignore();
		}
	}

	map_file.close();
}