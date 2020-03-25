#pragma once

#include "ECS.h"
#include "transform_component.h"

class ItemComponent : public Component
{
public:

	ItemComponent()
	{}

	~ItemComponent()
	{}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
	}

private:
	TransformComponent* transform;
	SpriteComponent* sprite;

};