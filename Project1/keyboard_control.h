#pragma once

#include "game.h"
#include "ECS.h"
#include "components.h"
#include "inventory_component.h"

class KeyboardControl : public Component
{
public:
	TransformComponent* transform;
	SpriteComponent* sprite;
	SpriteComponent* item_sprite;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
		const Uint8* keystates = SDL_GetKeyboardState(NULL);

		if (keystates[SDL_SCANCODE_W])
		{
			transform->velocity.y = -1;
			sprite->Play("Walk");
		}
		if (keystates[SDL_SCANCODE_A])
		{
			transform->velocity.x = -1;
			sprite->facing_left = true;
			sprite->Play("Walk");
		}
		if (keystates[SDL_SCANCODE_S])
		{
			transform->velocity.y = 1;
			sprite->Play("Walk");
		}
		if (keystates[SDL_SCANCODE_D])
		{
			transform->velocity.x = 1;
			sprite->facing_left = false;
			sprite->Play("Walk");
		}

		if (Game::event.type == SDL_KEYDOWN) {
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_DOWN:
				if (!entity->getComponent<InventoryComponent>().equipped.empty()) {
					sprite->Play("Attack");
				}
				break;
			default:
				break;
			}
		}

		if (Game::event.type == SDL_KEYUP) {
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				if (transform->velocity.y < 0) {
					transform->velocity.y = 0;
				}
				break;
			case SDLK_a:
				if (transform->velocity.x < 0) {
					transform->velocity.x = 0;
				}
				break;
			case SDLK_s:
				if (transform->velocity.y > 0) {
					transform->velocity.y = 0;
				}
				break;
			case SDLK_d:
				if (transform->velocity.x > 0) {
					transform->velocity.x = 0;
				}
				break;
			case SDLK_DOWN:
				sprite->Play("Idle");
				break;

			default:
				break;
			}

			if ((transform->velocity.x == 0) && (transform->velocity.y == 0)) {
				sprite->Play("Idle");
			}
		}
	}
};