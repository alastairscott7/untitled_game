#pragma once

#include "ECS.h"
#include "transform_component.h"
#include "sprite_component.h"

class AIComponent : public Component
{
public:
	AIComponent() = default;

	~AIComponent()
	{}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		initial_x = transform->position.x;
		initial_y = transform->position.y;
		sprite = &entity->getComponent<SpriteComponent>();
		is_attacking = false;
		transform->speed = 1;
	}

	void update() override
	{
		if (transform->velocity.x == 0 && transform->velocity.y == 0) {
			transform->position.x = initial_x;
			transform->position.y = initial_y;
		}
	}

	void aggro_check(Vector2D player_pos)
	{
		if (transform->position.x > Game::camera.x && transform->position.x < Game::camera.x + Game::camera.w &&
			transform->position.y > Game::camera.y && transform->position.y < Game::camera.y + Game::camera.h) {
			if ((player_pos.x - transform->position.x) >= 32) {
				transform->velocity.x = 1;
				sprite->Play("Walk");
				sprite->facing_left = false;
			}
			else if ((player_pos.x - transform->position.x) <= -32) {
				transform->velocity.x = -1;
				sprite->Play("Walk");
				sprite->facing_left = true;
			}
			if ((player_pos.y - transform->position.y) >= 32) {
				transform->velocity.y = 1;
				sprite->Play("Walk");
			}
			else if ((player_pos.y - transform->position.y) <= -32) {
				transform->velocity.y = -1;
				sprite->Play("Walk");
			}
		}
		else {
			transform->velocity.x = 0;
			transform->velocity.y = 0;
			sprite->Play("Idle");
		}
	}

private:
	bool is_attacking;
	float initial_x;
	float initial_y;
	TransformComponent* transform;
	SpriteComponent* sprite;

};