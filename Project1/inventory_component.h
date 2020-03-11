#pragma once

#include "ECS.h"
#include "transform_component.h"

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

	void update() override
	{
		//if (!equipped.empty()) {
	}

	void pickup_item(std::string id)
	{
		items[item_index] = id;
		equipped = items[item_index];
		item_index++;
	}

private:
	TransformComponent* transform;
	SpriteComponent* sprite;
	std::array<std::string, max_items> items;
	int item_index;
};