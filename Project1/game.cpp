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

	assets->add_texture("player", "assets/player.png");
	assets->add_texture("player_axe", "assets/player_axe.png");
	assets->add_texture("enemy", "assets/enemy_full.png");
	assets->add_texture("projectile", "assets/proj.png");
	assets->add_texture("axe", "assets/axe.png");
	assets->add_font("arial", "assets/arial.ttf", 16);

	map = new Map();
	map->load_map("assets/16x16map.map", 24, 16);

	player.addComponent<TransformComponent>(768, 224, 2);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<ColliderComponent>("player", player.getComponent<TransformComponent>().position.x,
		player.getComponent<TransformComponent>().position.y,
		player.getComponent<TransformComponent>().width * player.getComponent<TransformComponent>().scale,
		player.getComponent<TransformComponent>().height * player.getComponent<TransformComponent>().scale);
	player.addComponent<KeyboardControl>();
	player.addComponent<InventoryComponent>();
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
	TransformComponent transform_prev;
	SDL_Rect player_col;
	SDL_Rect player_future_col;
	SDL_Rect enemy_col;
	SDL_Rect item_col;
	std::stringstream ss;

	transform_prev = player.getComponent<TransformComponent>();
	player_col = player.getComponent<ColliderComponent>().collider;

	move_camera(transform_prev.position);

	ss << "Player Position: " << transform_prev.position;
	label.getComponent<UILabel>().set_label_text(ss.str(), "arial");

	for (auto e : enemies) {
		enemy_col = e->getComponent<ColliderComponent>().collider;

		e->getComponent<AIComponent>().aggro_check(transform_prev.position);
		if (Collision::AABB(enemy_col, player_col)) {
			e->getComponent<SpriteComponent>().Play("Attack");
		}
	}

	for (auto i : items) {
		item_col = i->getComponent<ColliderComponent>().collider;

		if (Collision::AABB(item_col, player_col)) {
			std::cout << "Player collected item: " << i->getComponent<SpriteComponent>().sprite_id << std::endl;
			player.getComponent<InventoryComponent>().pickup_item(i->getComponent<SpriteComponent>().sprite_id);
			i->destroy();
		}
	}

	/* Where would the player be next? */
	player_future_col = { static_cast<int>(transform_prev.position.x + transform_prev.velocity.x * transform_prev.speed),
		                  static_cast<int>(transform_prev.position.y + transform_prev.velocity.y * transform_prev.speed), 
		                  player_col.w, player_col.h };

	/* Stop impending tile collisions from occuring */
	for (auto c : colliders) {
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, player_future_col)) {
			if ((transform_prev.velocity.x > 0) && 
				(transform_prev.position.y < cCol.y + 64) && (transform_prev.position.y > cCol.y - 64)) {
				player.getComponent<TransformComponent>().position.x = cCol.x - 64;
				player.getComponent<TransformComponent>().velocity.x = 0;
			}
			else if ((transform_prev.velocity.x < 0) && 
				(transform_prev.position.y < cCol.y + 64) && (transform_prev.position.y > cCol.y - 64)) {
				player.getComponent<TransformComponent>().position.x = cCol.x + 64;
				player.getComponent<TransformComponent>().velocity.x = 0;
			}
			if ((transform_prev.velocity.y > 0) && 
				(transform_prev.position.x < cCol.x + 64) && (transform_prev.position.x > cCol.x - 64)) {
				player.getComponent<TransformComponent>().position.y = cCol.y - 64;
				player.getComponent<TransformComponent>().velocity.y = 0;
			}
			else if ((transform_prev.velocity.y < 0) && 
				(transform_prev.position.x < cCol.x + 64) && (transform_prev.position.x > cCol.x - 64)) {
				player.getComponent<TransformComponent>().position.y = cCol.y + 64;
				player.getComponent<TransformComponent>().velocity.y = 0;
			}
		}
	}

	manager.refresh();
	manager.update();

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