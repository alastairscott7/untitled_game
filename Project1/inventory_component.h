#pragma once

#include "ECS.h"
#include "transform_component.h"
#include "sprite_component.h"

constexpr std::size_t max_items = 16;

class InventoryComponent : public Component
{
public:
	std::string equipped;

	InventoryComponent()
	{
		item_index = 0;
	}

	~InventoryComponent()
	{}

	void pickup_item(std::string id)
	{
		items[item_index] = id;
		equipped = items[item_index];
		entity->getComponent<SpriteComponent>().setTex("player_" + id);
		item_index++;
	}

private:
	TransformComponent* transform;
	std::array<std::string, max_items> items;
	int item_index;
};