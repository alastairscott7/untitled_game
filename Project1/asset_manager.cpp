#include "asset_manager.h"
#include "components.h"

AssetManager::AssetManager(Manager* man) : manager(man)
{}

AssetManager::~AssetManager()
{}

void AssetManager::create_projectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id)
{
	auto& projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
	projectile.addComponent<SpriteComponent>(id, false);
	projectile.addComponent<ProjectileComponent>(range, speed, vel);
	projectile.addComponent<ColliderComponent>("projectile");
	projectile.addGroup(Game::groupProjectiles);
}

void AssetManager::add_texture(std::string id, const char* path)
{
	/* Texture manager returns SDL pointer and gets placed into the map with the id */
	textures.emplace(id, TextureManager::load_texture(path));
}

SDL_Texture* AssetManager::get_texture(std::string id)
{
	return textures[id];
}

void AssetManager::add_font(std::string id, std::string path, int font_size)
{
	fonts.emplace(id, TTF_OpenFont(path.c_str(), font_size));
}

TTF_Font* AssetManager::get_font(std::string id)
{
	return fonts[id];
}