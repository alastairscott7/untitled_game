#include "texture_manager.h"

SDL_Texture* TextureManager::load_texture(const char* texture)
{
	SDL_Texture* tex = IMG_LoadTexture(Game::renderer, texture);
	return tex;
}

void TextureManager::draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dst, SDL_RendererFlip flip)
{
	/* Add texture to renderer. src = part of texture to be drawn, dst = where you want it */
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dst, NULL, NULL, flip);
}