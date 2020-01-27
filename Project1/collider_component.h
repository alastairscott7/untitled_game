#pragma once
#include <string>
#include "SDL.h"
#include "components.h"

class ColliderComponent : public Component
{
public:
	SDL_Rect collider;
	std::string tag;

	TransformComponent* transform;

	ColliderComponent(std::string t)
	{
		tag = t;
	}

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>()) { //entity will need a transform component
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		Game::colliders.push_back(this); //why can i access this here?
	}

	void update() override
	{
		//top left of collision is origin of transform component
		collider.x = static_cast<int>(transform->position.x); 
		collider.y = static_cast<int>(transform->position.y);
		collider.w = transform->width * transform->scale;
		collider.h = transform->height * transform->scale;
	}
};