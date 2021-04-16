//==============================================================
// Filename : LTexture.h
// Authors : Dionne Ariëns and Marnick Los.
// Version : -
// License : -
// Description : This function is used by the GUI and holds information about the texture of the loaded images.  
//==============================================================

#pragma once
#include <SDL.h>
#include <string>

class LTexture
{
public:
	LTexture() : mTexture(NULL), mWidth(0), mHeight(0), gRenderer(NULL) {}
	~LTexture() {}

	// Initialize renderer
	void loadRenderer(SDL_Renderer& Renderer) {
		gRenderer = &Renderer;
	}

	// Load image from file using the path as given by the input. 
	bool loadFromFile(std::string path) {
		SDL_Texture* newTexture = NULL;

		// Create surface for image and load the image from path
		SDL_Surface* loadedSurface = IMG_Load(path.c_str());
		if (loadedSurface == NULL) { printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError()); return false; }

		// Create texture for image
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL) { printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError()); return false; }

		// Set the width and height of the image
		mWidth = loadedSurface->w;
		mHeight = loadedSurface->h;

		// deallocate the loaded surface
		SDL_FreeSurface(loadedSurface);

		// Save the new texture
		mTexture = newTexture;

		// Return true when the file loading and creating the texture is done well
		return true;
	}

	// deallocate the memory by destroying the texture and setting the variables to zero
	void free() {
		if (mTexture != NULL)
		{
			SDL_DestroyTexture(mTexture);
			mTexture = NULL;
			mWidth = 0;
			mHeight = 0;
		}
	}

	// Returns widht of the image
	int getWidth() { return mWidth; }

	// returns height of the image
	int getHeight() { return mHeight; }

	SDL_Texture* mTexture;

private:
	// private object and variables
	SDL_Renderer* gRenderer;

	int mWidth;
	int mHeight;
};