#pragma once

#include "ECS.h"
#include "transform_component.h"

class WeaponComponent : public Component
{
public:

	WeaponComponent(TransformComponent* owner_trns, SpriteComponent* owner_spr) : owner_trns(owner_trns), owner_spr(owner_spr)
	{}

	~WeaponComponent()
	{}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
		sprite->facing_left = owner_spr->facing_left;
		if (sprite->facing_left) {
			transform->position.x = owner_trns->position.x - 16;
			transform->position.y = owner_trns->position.y - 10;
		}
		else {
			transform->position.x = owner_trns->position.x + 16;
			transform->position.y = owner_trns->position.y - 10;
		}
	}

	void update() override
	{
		transform->velocity = owner_trns->velocity;
		sprite->facing_left = owner_spr->facing_left;
		if (owner_spr->facing_left) {
			transform->position.x = owner_trns->position.x - 16;
			transform->position.y = owner_trns->position.y - 10;
		}
		else {
			transform->position.x = owner_trns->position.x + 16;
			transform->position.y = owner_trns->position.y - 10;
		}
		if (owner_trns->attacking) {
			sprite->Play("Walk");
		}
		else {
			sprite->Play("Idle");
		}
	}

private:
	TransformComponent* transform;
	TransformComponent* owner_trns;
	SpriteComponent* sprite;
	SpriteComponent* owner_spr;
};