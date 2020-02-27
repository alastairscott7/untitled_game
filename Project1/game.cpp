#include "game.h"
#include "texture_manager.h"
#include "map.h"
#include "components.h"
#include "collision.h"

Map* map;
Manager manager;

SDL_Rect Game::camera = { 0, 0, 512, 512 };
SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;

auto& player = manager.addEntity();

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsystems Initialised!..." << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) {
			std::cout << "Window created!" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
			std::cout << "Renderer created!" << std::endl;
		}

		is_running = true;
	}
	else {
		is_running = false;
	}

	map = new Map();
	map->load_map("assets/16x16map.map", 24, 16);

	player.addComponent<TransformComponent>(768, 224, 2);
	player.addComponent<SpriteComponent>("assets/hoodie.png", true);
	player.addComponent<KeyboardControl>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);
}

/* Creating the lists to go through when rendering */
auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));

void Game::handle_events()
{
	SDL_PollEvent(&event);
	switch (event.type) {
	    case SDL_QUIT:
			is_running = false;
			break;

	    default:
		    break;
	}
}

void Game::update()
{
	SDL_Rect player_col = player.getComponent<ColliderComponent>().collider;
	Vector2D player_pos = player.getComponent<TransformComponent>().position;
	Vector2D player_pos_new;

	manager.refresh();
	manager.update();

	for (auto c : colliders) {
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, player_col)) {
			player.getComponent<TransformComponent>().position = player_pos;
		}
	}

	player_pos_new = player.getComponent<TransformComponent>().position;
	move_camera(player_pos_new);
}

void Game::move_camera(Vector2D player_pos_new)
{
	if (player_pos_new.x > (camera.x + camera.w - 32)) {
		camera.x += 512;
	}
	else if (player_pos_new.x < (camera.x - 32)) {
		camera.x -= 512;
	}
	else if (player_pos_new.y > (camera.y + camera.h - 32)) {
		camera.y += 512;
	}
	else if (player_pos_new.y < (camera.y - 32)) {
		camera.y -= 512;
	}
}

void Game::render()
{
	SDL_RenderClear(renderer);

	/* Render groups in order */
	for (auto& t : tiles)
	{
		t->draw();
	}
	for (auto& c : colliders)
	{
		c->draw();
	}
	for (auto& p : players)
	{
		p->draw();
	}
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned" << std::endl;
}