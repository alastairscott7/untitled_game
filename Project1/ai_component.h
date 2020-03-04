#pragma once

#include "ECS.h"

class AIComponent : public Component
{
public:
	AIComponent()
	{}

	~AIComponent()
	{}

	void init() override
	{}

	void update() override
	{
	}

private:
	bool is_attacking = false;

};