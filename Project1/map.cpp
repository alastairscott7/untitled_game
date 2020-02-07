#include "map.h"
#include "game.h"
#include <fstream>
#include "ECS.h"
#include "components.h"

extern Manager manager;

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
			AddTile(atoi(&tile), x * 64, y * 64);
			map_file.ignore();
		}
	}

	map_file.ignore();

	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			map_file.get(tile);
			if (tile == '3') {
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("terrain", x * 64, y * 64, 64, 64);
				tcol.addGroup(Game::groupColliders);
			}
			map_file.ignore();
		}
	}

	map_file.close();
}

void Map::AddTile(int id, int x, int y)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(x, y, 64, 64, id);
	tile.addGroup(Game::groupMap);
}