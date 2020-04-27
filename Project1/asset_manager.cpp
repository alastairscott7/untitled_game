#include "asset_manager.h"
#include "components.h"

AssetManager::AssetManager(Manager* man) : manager(man)
{}

AssetManager::~AssetManager()
{}

void AssetManager::create_projectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id, bool flip, bool anim)
{
	auto& projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 2);
	projectile.addComponent<SpriteComponent>(id, anim);
	projectile.addComponent<ProjectileComponent>(range, speed, vel);
	projectile.addComponent<ColliderComponent>("projectile");
	projectile.addGroup(Game::groupProjectiles);
	if (flip) {
		projectile.getComponent<SpriteComponent>().facing_left = false;
	}
}

void AssetManager::create_weapon(TransformComponent* owner_trns, SpriteComponent* owner_spr, std::string id, bool anim)
{
	std::cout << "Weapon owner position x: " << owner_trns->position.x << std::endl;
	std::cout << "Weapon owner position y: " << owner_trns->position.y << std::endl;
	auto& weapon(manager->addEntity());
	weapon.addComponent<TransformComponent>(owner_trns->position.x, owner_trns->position.y, 32, 32, 2);
	weapon.addComponent<SpriteComponent>(id, anim);
	weapon.addComponent<ColliderComponent>("projectile");
	weapon.addComponent<WeaponComponent>(owner_trns, owner_spr);
	weapon.addGroup(Game::groupWeapons);
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