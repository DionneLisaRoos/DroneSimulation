#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "LButton.h"
#include "LTexture.h"

const int SCREEN_WIDTH1 = 640;
const int SCREEN_HEIGHT1 = 480;
//
//const int BUTTON_WIDTH = 300;
//const int BUTTON_HEIGHT = 200;
//const int TOTAL_BUTTONS = 2;

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

class GUI
{
public:
	GUI() { 
		LTexture texture(gRenderer);
		gButtonSpriteSheetTexture = texture;
	};

	~GUI() {};

	void startSettingsPage() {
		//Start up SDL and create window
		if (!init())
		{
			printf("Failed to initialize!\n");
		}
		else
		{
			//Load media
			if (!loadMedia())
			{
				printf("Failed to load media!\n");
			}
			else
			{
				//Main loop flag
				bool quit = false;

				//Event handler
				SDL_Event e;

				//While application is running
				while (!quit)
				{
					//Handle events on queue
					while (SDL_PollEvent(&e) != 0)
					{
						//User requests quit
						if (e.type == SDL_QUIT)
						{
							quit = true;
						}

						//Handle button events
						for (int i = 0; i < TOTAL_BUTTONS; ++i)
						{
							gButtons[i].handleEvent(&e);
						}
					}

					//Clear screen
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderer);

					//Render buttons
					for (int i = 0; i < TOTAL_BUTTONS; ++i)
					{
						gButtons[i].render();
					}

					//Update screen
					SDL_RenderPresent(gRenderer);
				}
			}
		}

		//Free resources and close SDL
		close();

		//return 0;
	}

	bool init() {
		//Initialization flag
		bool success = true;

		//Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Set texture filtering to linear
			if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
			{
				printf("Warning: Linear texture filtering not enabled!");
			}

			//Create window
			gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH1, SCREEN_HEIGHT1, SDL_WINDOW_SHOWN);
			if (gWindow == NULL)
			{
				printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Create vsynced renderer for window
				gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
				if (gRenderer == NULL)
				{
					printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
					success = false;
				}
				else
				{
					//Initialize renderer color
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

					//Initialize PNG loading
					int imgFlags = IMG_INIT_PNG;
					if (!(IMG_Init(imgFlags) & imgFlags))
					{
						printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
						success = false;
					}
				}
			}
		}

		return success;
	}

	bool loadMedia() {
		//Loading success flag
		bool success = true;

		//Load sprites
		if (!gButtonSpriteSheetTexture.loadFromFile("drone.png"))
		{
			printf("Failed to load button sprite texture!\n");
			success = false;
		}
		else
		{
			//Set sprites
			for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
			{
				gSpriteClips[i].x = 0;
				gSpriteClips[i].y = i * 200;
				gSpriteClips[i].w = BUTTON_WIDTH;
				gSpriteClips[i].h = BUTTON_HEIGHT;
			}

			//Set buttons in corners
			gButtons[0].setPosition(0, 0);
			gButtons[1].setPosition(SCREEN_WIDTH1 - BUTTON_WIDTH, 0);
			gButtons[2].setPosition(0, SCREEN_HEIGHT1 - BUTTON_HEIGHT);
			gButtons[3].setPosition(SCREEN_WIDTH1 - BUTTON_WIDTH, SCREEN_HEIGHT1 - BUTTON_HEIGHT);
		}

		return success;
	}

	void close() {
		//Free loaded images
		gButtonSpriteSheetTexture.free();

		//Destroy window	
		SDL_DestroyRenderer(gRenderer);
		SDL_DestroyWindow(gWindow);
		gWindow = NULL;
		gRenderer = NULL;

		//Quit SDL subsystems
		IMG_Quit();
		SDL_Quit();
	}

private:
	SDL_Window* gWindow = NULL;

	//The window renderer
	SDL_Renderer* gRenderer = NULL;

	//Mouse button sprites
	SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];
	LTexture gButtonSpriteSheetTexture;

	//Buttons objects
	LButton gButtons[TOTAL_BUTTONS];
};

