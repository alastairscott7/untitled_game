#pragma once

#include "game.h"

class TextureManager {

public:
	static SDL_Texture* loadTexture(const char* fileName);
	static void Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dst, SDL_RendererFlip flip);
};