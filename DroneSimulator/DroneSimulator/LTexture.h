//==============================================================
// Filename : LTexture.h
// Authors : Dionne Ariëns and Marnick Los.
// Version : -
// License : -
// Description : 
//==============================================================

#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>

class LTexture
{
public:
	LTexture() : mTexture(NULL), mWidth(0), mHeight(0), gRenderer(NULL) {}
	~LTexture() { free(); }

	void loadRenderer(SDL_Renderer& Renderer) {
		gRenderer = &Renderer;
	}

	bool loadFromFile(std::string path) {
		SDL_Texture* newTexture = NULL;

		SDL_Surface* loadedSurface = IMG_Load(path.c_str());
		if (loadedSurface == NULL) { printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError()); return false; }

		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL) { printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError()); return false; }

		mWidth = loadedSurface->w;
		mHeight = loadedSurface->h;

		SDL_FreeSurface(loadedSurface);

		mTexture = newTexture;
		return true;
	}

	void free() {
		if (mTexture != NULL)
		{
			SDL_DestroyTexture(mTexture);
			mTexture = NULL;
			mWidth = 0;
			mHeight = 0;
		}
	}

	int getWidth() { return mWidth; }

	int getHeight() { return mHeight; }

	SDL_Texture* mTexture;

private:
	SDL_Renderer* gRenderer;

	int mWidth;
	int mHeight;
};