#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "vector_2d.h"
#include "asset_manager.h"
#include <bitset>
#include <iostream>
#include <vector>

class ColliderComponent;
class AssetManager;

class Game {

public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void handle_events();
	void update();
	bool running() { return is_running; }
	void render();
	void clean();
	void move_camera(Vector2D player_pos_new);

	static SDL_Rect camera;
	static SDL_Renderer *renderer;
	static SDL_Event event;
	static AssetManager* assets;

	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupEnemies,
		groupColliders,
		groupProjectiles,
		groupItems,
		groupWeapons
	};

private:
	bool is_running; /* public or private? */
	SDL_Window* window;
};