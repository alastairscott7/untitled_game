#include "game.h"
#include "texture_manager.h"
#include "map.h"
#include "components.h"
#include "vector_2d.h"
#include "collision.h"

Map* map;
Manager manager;

/* Need to define static variables to use them */
SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0, 0, 512, 512 };

auto& player = manager.addEntity(); /* addEntity returns an Entity reference */

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

		isRunning = true;
	}
	else {
		isRunning = false;
	}

	map = new Map();
	map->LoadMap("assets/16x16map.map", 24, 16);

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

void Game::handleEvents()
{
	SDL_PollEvent(&event);
	switch (event.type) {
	    case SDL_QUIT:
			isRunning = false;
			break;

	    default:
		    break;
	}
}

void Game::update()
{
	SDL_Rect playercol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	int playerTilex;
	int playerTiley;
	playerTilex = playerPos.x / 512;
	playerTiley = playerPos.y / 512;
	camera.x = 0;

	manager.refresh();
	manager.update();

	for (auto c : colliders) {
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playercol)) {
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}

	Vector2D playerNewPos = player.getComponent<TransformComponent>().position;

	int playerNewx;
	int playerNewy;
	playerNewx = playerNewPos.x / 512;
	playerNewy = playerNewPos.y / 512;

	std::cout << playerNewPos.x << std::endl;
	std::cout << playerNewPos.y << std::endl;

	if ((playerNewx * 512) > camera.x) {
		camera.x = playerNewx * 512;

		/*
		if (camera.x < 0) {
			camera.x = 0;
		}
		if (camera.y < 0) {
			camera.y = 0;
		}
		*/
		if (camera.x > camera.w) {
			camera.x = camera.w;
		}
		/*
		if (camera.y > camera.h) {
			camera.y = camera.h;
		}
		*/

		for (auto t : tiles) {
			TileComponent tile_comp = t->getComponent<TileComponent>();
			tile_comp.transform->position.x -= Game::camera.x;
		}
		for (auto c : colliders) {
			c->getComponent<ColliderComponent>().collider.x -= Game::camera.x;
		}
		player.getComponent<TransformComponent>().position.x -= 512;
	}
	else if (playerNewPos.x < camera.x) {
		camera.x = -512;

		if (camera.x > camera.w) {
			camera.x = camera.w;
		}
		/*
		if (camera.y > camera.h) {
			camera.y = camera.h;
		}
		*/
		for (auto t : tiles) {
			TileComponent tile_comp = t->getComponent<TileComponent>();
			tile_comp.transform->position.x -= Game::camera.x;
		}
		for (auto c : colliders) {
			c->getComponent<ColliderComponent>().collider.x -= Game::camera.x;
		}
		player.getComponent<TransformComponent>().position.x += 512;
	}
	else if ((playerNewy * 512) > camera.y) {
		camera.y = playerNewy * 512;

		for (auto t : tiles) {
			TileComponent tile_comp = t->getComponent<TileComponent>();
			tile_comp.transform->position.y -= Game::camera.y;
		}
		for (auto c : colliders) {
			c->getComponent<ColliderComponent>().collider.y -= Game::camera.y;
		}
		player.getComponent<TransformComponent>().position.y -= 512;
	}
	else if (playerNewPos.y < camera.y) {
		camera.y = -512;

		for (auto t : tiles) {
			TileComponent tile_comp = t->getComponent<TileComponent>();
			tile_comp.transform->position.y -= Game::camera.y;
		}
		for (auto c : colliders) {
			c->getComponent<ColliderComponent>().collider.y -= Game::camera.y;
		}
		player.getComponent<TransformComponent>().position.y += 512;
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