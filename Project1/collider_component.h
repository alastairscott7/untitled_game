#pragma once
#include <string>
#include "SDL.h"
#include "components.h"
#include "texture_manager.h"

class ColliderComponent : public Component
{
public:
	SDL_Rect collider;
	std::string tag;

	SDL_Texture* tex;
	SDL_Rect srcR, dstR;

	TransformComponent* transform;

	ColliderComponent(std::string t)
	{
		tag = t;
	}

	ColliderComponent(std::string t, int xpos, int ypos, int w, int h)
	{
		tag = t;
		collider.x = xpos;
		collider.y = ypos;
		collider.h = h;
		collider.w = w;
	}

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>()) { //entity will need a transform component
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		tex = TextureManager::loadTexture("assets/coltex.png");
		srcR = { 0, 0, 32, 32 };
		dstR = { collider.x, collider.y, collider.w, collider.h };

		//Use grouping instead of this?
		//Game::colliders.push_back(this); //why can i access this here?
	}

	void update() override
	{
		if (tag != "terrain") { //only update if this collider component is not terrain
			//top left of collision is origin of transform component
			collider.x = static_cast<int>(transform->position.x);
			collider.y = static_cast<int>(transform->position.y);
			collider.w = transform->width * transform->scale;
			collider.h = transform->height * transform->scale;
		}
	}

	void draw() override
	{
		TextureManager::Draw(tex, srcR, dstR, SDL_FLIP_NONE);
	}
};