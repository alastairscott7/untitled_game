#pragma once

#include "ECS.h"
#include "transform_component.h"
#include "SDL.h"

class TileComponent : public Component
{
public:
	TransformComponent* transform;
	SpriteComponent* sprite;

	SDL_Rect tile_rect;
	int tile_id;
	const char* path;

	TileComponent() = default;

	TileComponent(int x, int y, int w, int h, int id)
	{
		tile_rect.x = x;
		tile_rect.y = y;
		tile_rect.w = w;
		tile_rect.h = h;
		tile_id = id;

		switch (tile_id)
		{
		case 0:
			path = "assets/water.png";
			break;
		case 1:
			path = "assets/wall.png";
			break;
		default:
			break;
		}
	}

	void init() override
	{
		entity->addComponent<TransformComponent>((float)tile_rect.x, (float)tile_rect.y, tile_rect.w, tile_rect.h, 1);
		transform = &entity->getComponent<TransformComponent>();

		entity->addComponent<SpriteComponent>(path);
		sprite = &entity->getComponent<SpriteComponent>();
	}
};