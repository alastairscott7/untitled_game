#pragma once
#include "SDL.h"

/* Used instead of an include */
class ColliderComponent;
class TransformComponent;

class Collision
{
public:
	static bool AABB(const SDL_Rect& rect_A, const SDL_Rect& rect_B);
	static bool AABB(const ColliderComponent& col_A, const ColliderComponent& col_B);
};