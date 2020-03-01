#pragma once
#include "ECS.h"
#include "asset_manager.h"
#include "game.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <string.h>

/* Look into adding UI Containers */

class UILabel : public Component
{
public:
	UILabel(int x_pos, int y_pos, std::string text, std::string font, SDL_Color& color) :
		label_text(text), label_font(font), text_color(color)
	{
		position.x = x_pos;
		position.y = y_pos;

		set_label_text(label_text, label_font);
	}

	~UILabel()
	{}
	
	void set_label_text(std::string text, std::string font)
	{
		SDL_Surface* surf = TTF_RenderText_Blended(Game::assets->get_font(font), text.c_str(), text_color);
		label_texture = SDL_CreateTextureFromSurface(Game::renderer, surf);
		SDL_FreeSurface(surf);

		SDL_QueryTexture(label_texture, nullptr, nullptr, &position.w, &position.h);
	}

	void draw() override
	{
		SDL_RenderCopy(Game::renderer, label_texture, nullptr, &position);
	}

private:
	SDL_Rect position;
	std::string label_text;
	std::string label_font;
	SDL_Color text_color;
	SDL_Texture* label_texture;

};