#pragma once

#include "game.h"
#include "ECS.h"
#include "components.h"

class KeyboardControl : public Component
{
public:
	TransformComponent* transform;
	SpriteComponent* sprite;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
		if (Game::event.type == SDL_KEYDOWN) {
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				transform->velocity.y = -1;
				//sprite->Play("Walk");
				break;
			case SDLK_a:
				transform->velocity.x = -1;
				sprite->facing_left = true;
				//sprite->Play("Walk");
				break;
			case SDLK_s:
				transform->velocity.y = 1;
				//sprite->Play("Walk");
				break;
			case SDLK_d:
				transform->velocity.x = 1;
				sprite->facing_left = false;
				//sprite->Play("Walk");
				break;

			default:
				break;
			}
		}

		if (Game::event.type == SDL_KEYUP) {
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				transform->velocity.y = 0;
				//sprite->Play("Idle");
				break;
			case SDLK_a:
				transform->velocity.x = 0;
				//sprite->Play("Idle");
				//sprite->spriteFlip = SDL_FLIP_NONE;
				break;
			case SDLK_s:
				transform->velocity.y = 0;
				//sprite->Play("Idle");
				break;
			case SDLK_d:
				transform->velocity.x = 0;
				//sprite->Play("Idle");
				break;

			default:
				break;
			}
		}
	}
};