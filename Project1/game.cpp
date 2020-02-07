#include "game.h"
#include "texture_manager.h"
#include "map.h"
#include "components.h"
#include "vector_2d.h"
#include "collision.h"

Map* map;
Manager manager;

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;

//std::vector<ColliderComponent*> Game::colliders; //re-watch the static variables video to see why we need these
//bool Game::isRunning = false;

auto& player = manager.addEntity(); //addEntity returns an Entity reference

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
	//ECS implementation

	map->LoadMap("assets/16x16map.map", 8, 8);

	player.addComponent<TransformComponent>(224, 224, 2);
	player.addComponent<SpriteComponent>("assets/hoodie.png", true);
	player.addComponent<KeyboardControl>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);

	/*
	Example way to add a wall with collision:
	wall.addComponent<TransformComponent>(300.0f, 300.0f, 300, 20, 1);
	wall.addComponent<SpriteComponent>("assets/wall_basic.png");
	wall.addComponent<ColliderComponent>("wall");
	wall.addGroup(groupMap);
	*/
}

//rendering groups in order by creating the lists to go through when rendering
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

	manager.refresh();
	manager.update();

	for (auto c : colliders) {
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playercol)) {
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}
}

void Game::render()
{
	SDL_RenderClear(renderer);
	//this used to draw objects in order they were created: manager.draw();

	//go through groups and render one after the other:
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