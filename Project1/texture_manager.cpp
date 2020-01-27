#include "texture_manager.h"

SDL_Texture* TextureManager::loadTexture(const char* texture)
{
	SDL_Texture* tex = IMG_LoadTexture(Game::renderer, texture);
	return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dst, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dst, NULL, NULL, flip); //add texture to renderer. src = part of texture to be drawn, dst = where you want it
}