#pragma once

#include "components.h"
#include "SDL.h"
#include "texture_manager.h"
#include "animation.h"
#include <map>

class SpriteComponent : public Component
{
public:

	int anim_index = 0;

	std::map<const char*, Animation> animations_map;

	SDL_RendererFlip sprite_flip = SDL_FLIP_NONE;
	bool facing_left = true;

	SpriteComponent() = default;
	SpriteComponent(const char* path)
	{
		setTex(path);
	}

	SpriteComponent(const char* path, bool isAnimated)
	{
		animated = isAnimated;

		Animation idle = Animation(0, 2, 200);
		//Animation walk = Animation(1, 8, 100);

		animations_map.emplace("Idle", idle);
		//animations.emplace("Walk", walk);

		Play("Idle");
		setTex(path);
	}

	~SpriteComponent()
	{
		SDL_DestroyTexture(texture);
	}

	void setTex(const char* path)
	{
		texture = TextureManager::loadTexture(path);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	void update() override
	{
		/* Adjust srcRect.x to scroll through animations from left to right */
		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}
		/* Adjust srcRect.y depending anim_index to change animation cycle */
		srcRect.y = anim_index * transform->height;

		if (!facing_left) {
			sprite_flip = SDL_FLIP_HORIZONTAL;
		}
		else {
			sprite_flip = SDL_FLIP_NONE;
		}

		dstRect.x = static_cast<int>(transform->position.x);
		dstRect.y = static_cast<int>(transform->position.y);
		dstRect.w = transform->width * transform->scale;
		dstRect.h = transform->height * transform->scale;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, dstRect, sprite_flip);
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
	SDL_Rect srcRect, dstRect;

	bool animated = false;
	int frames = 0;
	int speed = 100;
};