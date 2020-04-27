#pragma once

#include <map>
#include <string>
#include "texture_manager.h"
#include "vector_2d.h"
#include "ECS.h"
#include "SDL_ttf.h"

class TransformComponent;
class SpriteComponent;

class AssetManager
{
public:
	AssetManager(Manager* man);
	~AssetManager();

	/* Game Objects */

	void create_projectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id, bool flip, bool anim);
	void create_weapon(TransformComponent* owner_trns, SpriteComponent* owner_spr, std::string id, bool anim);

	/* Texture Management */
	void add_texture(std::string id, const char* path);
	SDL_Texture* get_texture(std::string id);

	/* Fonts */
	void add_font(std::string id, std::string path, int font_size);
	TTF_Font* get_font(std::string id);

private:

	Manager* manager;
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
};