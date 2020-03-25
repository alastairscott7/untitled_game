#include "collision.h"
#include "collider_component.h"

bool Collision::AABB(const SDL_Rect& rect_A, const SDL_Rect& rect_B)
{
	if (rect_A.x + rect_A.w >= rect_B.x &&
		rect_B.x + rect_B.w >= rect_A.x &&
		rect_A.y + rect_A.h >= rect_B.y &&
		rect_B.y + rect_B.h >= rect_A.y) {
		return true;
	}
	return false;
}

bool Collision::AABB(const ColliderComponent& col_A, const ColliderComponent& col_B)
{
	if (AABB(col_A.collider, col_B.collider)) {
		std::cout << col_A.tag << " hit: " << col_B.tag << std::endl;
		return true;
	}
	return false;
}