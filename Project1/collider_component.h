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
		/* entity will need a transform component if it doesn't have one */
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		tex = TextureManager::load_texture("assets/coltex.png");
		srcR = { 0, 0, 32, 32 };
		dstR = { collider.x, collider.y, collider.w, collider.h };
	}

	void update() override
	{
		/* only update if this collider component is not terrain */
		if (tag != "terrain") {
			/* top left of collision is origin of transform component */
			collider.x = static_cast<int>(transform->position.x);
			collider.y = static_cast<int>(transform->position.y);
			collider.w = transform->width * transform->scale;
			collider.h = transform->height * transform->scale;
			/* FIXME: dstR Width and height not getting initialized properly */
			dstR.w = collider.w;
			dstR.h = collider.h;
		}
			dstR.x = collider.x - Game::camera.x;
			dstR.y = collider.y - Game::camera.y;
	}

	void draw() override
	{
		TextureManager::draw(tex, srcR, dstR, SDL_FLIP_NONE);
	}
};