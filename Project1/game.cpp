#include "game.h"
#include "texture_manager.h"
#include "map.h"
#include "components.h"
#include "collision.h"
#include "asset_manager.h"
#include <sstream>

Map* map;
Manager manager;

SDL_Rect Game::camera = { 0, 0, 512, 512 };
SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;

AssetManager* Game::assets = new AssetManager(&manager);

auto& player = manager.addEntity();
auto& enemy = manager.addEntity();
auto& label = manager.addEntity();
auto& axe = manager.addEntity();

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
	
	if (TTF_Init() == -1)
	{
		std::cout << "Error: SDL_TTF" << std::endl;
	}


	assets->add_texture("player", "assets/hoodie.png");
	assets->add_texture("enemy", "assets/enemy_full.png");
	assets->add_texture("projectile", "assets/proj.png");
	assets->add_texture("axe", "assets/axe.png");
	assets->add_font("arial", "assets/arial.ttf", 16);

	map = new Map();
	map->load_map("assets/16x16map.map", 24, 16);

	player.addComponent<TransformComponent>(768, 224, 2);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardControl>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);

	enemy.addComponent<TransformComponent>(100, 224, 2);
	enemy.addComponent<SpriteComponent>("enemy", true);
	enemy.addComponent<ColliderComponent>("enemy");
	enemy.addComponent<AIComponent>();
	enemy.addGroup(groupEnemies);

	axe.addComponent<TransformComponent>(1300, 224, 2);
	axe.addComponent<SpriteComponent>("axe", true);
	axe.addComponent<ColliderComponent>("item");
	axe.addComponent<ItemComponent>();
	axe.addGroup(groupItems);

	SDL_Color white = { 255, 255, 255, 255 };
	label.addComponent<UILabel>(10, 10, "Test String", "arial", white);

	//assets->create_projectile(Vector2D(525, 100), Vector2D(2, 0), 500, 2, "projectile");
}

/* Creating the lists to go through when rendering */
auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& items(manager.getGroup(Game::groupItems));

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
	Vector2D enemy_pos;
	SDL_Rect enemy_col;
	std::stringstream ss;

	ss << "Player Position: " << player_pos;
	label.getComponent<UILabel>().set_label_text(ss.str(), "arial");

	move_camera(player_pos);

	for (auto e : enemies) {
		enemy_col = e->getComponent<ColliderComponent>().collider;

		e->getComponent<AIComponent>().aggro_check(player_pos);
		if (Collision::AABB(enemy_col, player_col)) {
			e->getComponent<SpriteComponent>().Play("Attack");
		}
	}

	manager.refresh();
	manager.update();

	for (auto c : colliders) {
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, player_col)) {
			/* FIXME: Player's previous position still collides */
			std::cout << "collider position x:" << cCol.x << std::endl;
			std::cout << "collider position y:" << cCol.y << std::endl;
			std::cout << "player col position x:" << player_col.x << std::endl;
			std::cout << "player col position y:" << player_col.y << std::endl;
			player.getComponent<TransformComponent>().position = player_pos;
		}
	}

	for (auto& p : projectiles)
	{
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider)) {
			/* get type of projectile, do different kinds of damage, etc */
			std::cout << "Projectile hit player" << std::endl;
			p->destroy();
		}
	}

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
	for (auto& p : projectiles)
	{
		p->draw();
	}
	for (auto& i : items)
	{
		i->draw();
	}
	for (auto& p : players)
	{
		p->draw();
	}
	for (auto& e : enemies)
	{
		e->draw();
	}
	label.draw();

	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned" << std::endl;
}