#pragma once

#include "components.h"
#include "SDL.h"
#include "texture_manager.h"
#include "animation.h"
#include "asset_manager.h"
#include <map>

class SpriteComponent : public Component
{
public:

	int anim_index = 0;
	std::string sprite_id;

	std::map<const char*, Animation> animations_map;

	SDL_RendererFlip sprite_flip = SDL_FLIP_NONE;
	bool facing_left = true;

	SpriteComponent() = default;
	SpriteComponent(std::string id)
	{
		setTex(id);
	}

	SpriteComponent(std::string id, bool isAnimated)
	{
		animated = isAnimated;
		sprite_id = id;

		Animation idle = Animation(0, 2, 200);
		Animation walk = Animation(1, 2, 200);
		Animation attack = Animation(2, 2, 200);

		animations_map.emplace("Idle", idle);
		animations_map.emplace("Walk", walk);
		animations_map.emplace("Attack", attack);

		Play("Idle");
		setTex(sprite_id);
	}

	~SpriteComponent()
	{
	}

	void setTex(std::string id)
	{
		texture = Game::assets->get_texture(id);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		src_rect.x = src_rect.y = 0;
		src_rect.w = transform->width;
		src_rect.h = transform->height;
	}

	void update() override
	{
		/* Adjust src_rect.x to scroll through animations from left to right */
		if (animated)
		{
			src_rect.x = src_rect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}
		/* Adjust src_rect.y depending anim_index to change animation cycle */
		src_rect.y = anim_index * transform->height;

		if (!facing_left) {
			sprite_flip = SDL_FLIP_HORIZONTAL;
		}
		else {
			sprite_flip = SDL_FLIP_NONE;
		}

		dst_rect.x = static_cast<int>(transform->position.x) - Game::camera.x;
		dst_rect.y = static_cast<int>(transform->position.y) - Game::camera.y;
		dst_rect.w = transform->width * transform->scale;
		dst_rect.h = transform->height * transform->scale;
	}

	void draw() override
	{
		TextureManager::draw(texture, src_rect, dst_rect, sprite_flip);
	}

	void Play(const char* animName)
	{
		frames = animations_map[animName].frames;
		anim_index = animations_map[animName].index;
		speed = animations_map[animName].speed;
	}

private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect src_rect, dst_rect;

	bool animated = false;
	int frames = 0;
	int speed = 100;
};