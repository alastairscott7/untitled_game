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

private:
	bool is_attacking;
	float initial_x;
	float initial_y;
	TransformComponent* transform;
	SpriteComponent* sprite;

};