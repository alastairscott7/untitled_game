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

void Map::load_map(std::string path, int size_x, int size_y)
{
	char tile;
	std::fstream map_file;
	map_file.open(path);

	for (int y = 0; y < size_y; y++) {
		for (int x = 0; x < size_x; x++) {
			map_file.get(tile);
			add_tile(atoi(&tile), x * 64, y * 64);
			map_file.ignore();
		}
	}

	map_file.ignore();

	for (int y = 0; y < size_y; y++) {
		for (int x = 0; x < size_x; x++) {
			map_file.get(tile);
			if (tile == '3') {
				add_tile_col("terrain", x * 64, y * 64);
			}
			map_file.ignore();
		}
	}

	map_file.close();
}

void Map::add_tile(int id, int x, int y)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(x, y, 64, 64, id);
	tile.addGroup(Game::groupMap);
}

void Map::add_tile_col(std::string tag, int x, int y)
{
	auto& tcol(manager.addEntity());
	tcol.addComponent<ColliderComponent>("terrain", x, y, 64, 64);
	tcol.addGroup(Game::groupColliders);
}