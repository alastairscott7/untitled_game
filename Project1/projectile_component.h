#pragma once

#include "ECS.h"
#include "components.h"
#include "vector_2d.h"

class ProjectileComponent : public Component
{
public:
	ProjectileComponent(int range, int sp, Vector2D vel) : range (range), speed (sp), velocity(vel)
	{}
	~ProjectileComponent()
	{}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity;
	}

	/* need to decouple update loop from render loop to fix speed */
	void update() override
	{
		distance += speed;

		if (distance > range) {
			std::cout << "Projectile out of range" << std::endl;
			entity->destroy();
		}
		else if (transform->position.x > Game::camera.x + Game::camera.w ||
				transform->position.x < Game::camera.x ||
				transform->position.y > Game::camera.y + Game::camera.h ||
				transform->position.y < Game::camera.y)
		{
			std::cout << "Projectile out of bounds" << std::endl;
			entity->destroy();
		}
	}

private:

	TransformComponent* transform;

	int range = 0;
	int speed = 0;
	int distance = 0;
	Vector2D velocity;
};