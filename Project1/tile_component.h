#pragma once

#include "ECS.h"
#include "transform_component.h"
#include "SDL.h"

class TileComponent : public Component
{
public:
	SDL_Texture* texture;

	SDL_Rect src_rect, dst_rect;
	int tile_id;
	const char* path;

	Vector2D real_pos;

	TileComponent() = default;

	~TileComponent()
	{
		SDL_DestroyTexture(texture);
	}

	TileComponent(int x, int y, int w, int h, int id)
	{
		real_pos.x = x;
		real_pos.y = y;

		src_rect = { 0, 0, 32, 32 };

		dst_rect.x = x;
		dst_rect.y = y;
		dst_rect.w = w;
		dst_rect.h = h;
		tile_id = id;

		switch (tile_id)
		{
		case 0:
			path = "assets/forest_basic.png";
			break;
		case 1:
			path = "assets/tree_mid.png";
			break;
		case 2:
			path = "assets/tree_top.png";
			break;
		default:
			break;
		}

		texture = TextureManager::load_texture(path);
	}
    
	void update() override
	{
		dst_rect.x = real_pos.x - Game::camera.x;
		dst_rect.y = real_pos.y - Game::camera.y;
	}
	
	void draw() override
	{
		TextureManager::draw(texture, src_rect, dst_rect, SDL_FLIP_NONE);
	}
};