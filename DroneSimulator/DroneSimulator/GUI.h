#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "LTexture.h"
#include "LButton.h"

enum class Images
{
	IMAGE_DRONE = 0,
	IMAGE_DRONE_AND_CARGO = 1,
	IMAGE_TITLE = 2,
	IMAGE_TEXT_DRONE = 3,
	IMAGE_TEXT_DRONE_AND_CARGO = 4,
	IMAGE_KEYBOARD = 5,
	IMAGE_CSV_FILE = 6,
	IMAGE_TEXT_KEYBOARD = 7,
	IMAGE_TEXT_CSV_FILE = 8,
	IMAGE_SUBTEXT = 9,
	TOTAL_IMAGES = 10
};

class GUI
{
public:
	GUI() : keyboard(false), cargo(false) {
		if (!init()) { printf("Failed to initialize!\n"); return; }
		if (!loadMedia()) { printf("Failed to load media!\n"); return; }
	}

	~GUI() {}

	void startGUI() {
		bool choice = false;
		bool fileFlag = false;
		bool quit = false;

		SDL_Event e;

		while (!quit)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}

				for (int i = 0; i < 2; ++i)
				{
					gButtons[i].handleEvent(&e);
				}
			}

			if (gButtons[0].isPushed() && choice) {
				keyboard = false;
				gButtons[0].clearPushedFlag();
				quit = true;
			}
			else if (gButtons[1].isPushed() && choice) {
				keyboard = true;
				quit = true;
			}
			else if (gButtons[0].isPushed()) {
				cargo = false;
				gButtons[0].clearPushedFlag();
				choice = true;
			}
			else if (gButtons[1].isPushed()) {
				cargo = true;
				gButtons[1].clearPushedFlag();
				choice = true;
			}

			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			render(choice);
			SDL_RenderPresent(gRenderer);
		}
		close();
	}

	bool getCargoChoice() { return cargo; }

	bool getKeyboardChoice() { return keyboard; }

private:
	SDL_Window* gWindow = NULL;
	SDL_Renderer* gRenderer = NULL;

	SDL_Rect gImages[static_cast<int>(Images::TOTAL_IMAGES)];
	LTexture gDrone;
	LTexture gDroneWithCargo;
	LTexture gTitle;
	LTexture gtextDrone;
	LTexture gtextDroneWithCargo;
	LTexture gKeyboard;
	LTexture gCSVfile;
	LTexture gtextKeyboard;
	LTexture gtextCSVfile;
	LTexture gSubText;

	LButton gButtons[2];

	bool cargo;
	bool keyboard;

	bool init()
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0) { printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError()); return false; }
		
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) { printf("Warning: Linear texture filtering not enabled!"); }

		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 400, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) { printf("Window could not be created! SDL Error: %s\n", SDL_GetError()); return false; }

		//Create vsynced renderer for window
		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (gRenderer == NULL) { printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError()); return false; }
		
		gDrone.loadRenderer(*gRenderer);
		gDroneWithCargo.loadRenderer(*gRenderer);;
		gTitle.loadRenderer(*gRenderer);
		gtextDrone.loadRenderer(*gRenderer);
		gtextDroneWithCargo.loadRenderer(*gRenderer);
		gKeyboard.loadRenderer(*gRenderer);
		gCSVfile.loadRenderer(*gRenderer);
		gtextKeyboard.loadRenderer(*gRenderer);
		gtextCSVfile.loadRenderer(*gRenderer);
		gSubText.loadRenderer(*gRenderer);
		
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

		//Initialize PNG loading
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags)) { printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError()); return false; }
		
		return true;
	}

	bool loadMedia()
	{
		if (!gDrone.loadFromFile("drone.png")) { printf("Failed to load drone.pgn!\n"); return false; }
		if (!gDroneWithCargo.loadFromFile("DroneAndCargo.png")) { printf("Failed to load DroneAndCargo.png!\n"); return false; }
		if (!gKeyboard.loadFromFile("Keyboard.png")) { printf("Failed to load Keyboard.png!\n"); return false; }
		if (!gCSVfile.loadFromFile("CSV.png")) { printf("Failed to load CSV.png!\n"); return false; }
		if (!gtextDrone.loadFromFile("DroneChoice.png")) { printf("Failed to load DroneChoice.png!\n"); return false; }
		if (!gtextDroneWithCargo.loadFromFile("DroneWithCargoChoice.png")) { printf("Failed to load DroneWithCargoChoice!\n"); return false; }
		if (!gTitle.loadFromFile("Title.png")) { printf("Failed to load Title.png!\n"); return false; }
		if (!gtextKeyboard.loadFromFile("KeyboardInputChoice.png")) { printf("Failed to load KeyboardInputChoice.png!\n"); return false; }
		if (!gtextCSVfile.loadFromFile("InputFileChoice.png")) { printf("Failed to load InputFileChoice.png!\n"); return false; }
		if (!gSubText.loadFromFile("SubText.png")) { printf("Failed to load SubText.png!\n"); return false; }

		fillImagePosition(static_cast<int>(Images::IMAGE_DRONE), 60, 180, gDrone);
		fillImagePosition(static_cast<int>(Images::IMAGE_DRONE_AND_CARGO), 380, 180, gDroneWithCargo);
		fillImagePosition(static_cast<int>(Images::IMAGE_TITLE), 200, 10, gTitle);
		fillImagePosition(static_cast<int>(Images::IMAGE_TEXT_DRONE), 120, 120, gtextDrone);
		fillImagePosition(static_cast<int>(Images::IMAGE_TEXT_DRONE_AND_CARGO), 400, 120, gtextDroneWithCargo);
		fillImagePosition(static_cast<int>(Images::IMAGE_CSV_FILE), 85, 180, gCSVfile);
		fillImagePosition(static_cast<int>(Images::IMAGE_KEYBOARD), 380, 180, gKeyboard);
		fillImagePosition(static_cast<int>(Images::IMAGE_TEXT_CSV_FILE), 60, 120, gtextCSVfile);
		fillImagePosition(static_cast<int>(Images::IMAGE_TEXT_KEYBOARD), 410, 120, gtextKeyboard);
		fillImagePosition(static_cast<int>(Images::IMAGE_SUBTEXT), 205, 50, gtextKeyboard);

		gButtons[0].setPosition(60, 180, gKeyboard.getWidth(), gDroneWithCargo.getHeight());
		gButtons[1].setPosition(380, 180, gKeyboard.getWidth(), gDroneWithCargo.getHeight());

		return true;
	}

	void fillImagePosition(int i, int x, int y, LTexture& texture) {
		gImages[i].x = x;
		gImages[i].y = y;
		gImages[i].w = texture.getWidth();
		gImages[i].h = texture.getHeight();
	}

	void render(bool choice)
	{
		SDL_Rect rect = gButtons[0].getPosition();
		SDL_Rect rect1 = gButtons[1].getPosition();
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(gRenderer, &rect);
		SDL_RenderDrawRect(gRenderer, &rect1);

		if (!choice) {
			SDL_RenderCopyEx(gRenderer, gDrone.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_DRONE)], 0.0, NULL, SDL_FLIP_NONE);
			SDL_RenderCopyEx(gRenderer, gDroneWithCargo.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_DRONE_AND_CARGO)], 0.0, NULL, SDL_FLIP_NONE);
			SDL_RenderCopyEx(gRenderer, gtextDrone.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_TEXT_DRONE)], 0.0, NULL, SDL_FLIP_NONE);
			SDL_RenderCopyEx(gRenderer, gtextDroneWithCargo.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_TEXT_DRONE_AND_CARGO)], 0.0, NULL, SDL_FLIP_NONE);
		}
		else {
			SDL_RenderCopyEx(gRenderer, gCSVfile.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_CSV_FILE)], 0.0, NULL, SDL_FLIP_NONE);
			SDL_RenderCopyEx(gRenderer, gKeyboard.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_KEYBOARD)], 0.0, NULL, SDL_FLIP_NONE);
			SDL_RenderCopyEx(gRenderer, gtextCSVfile.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_TEXT_CSV_FILE)], 0.0, NULL, SDL_FLIP_NONE);
			SDL_RenderCopyEx(gRenderer, gtextKeyboard.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_TEXT_KEYBOARD)], 0.0, NULL, SDL_FLIP_NONE);
		}

		SDL_RenderCopyEx(gRenderer, gTitle.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_TITLE)], 0.0, NULL, SDL_FLIP_NONE);
		SDL_RenderCopyEx(gRenderer, gSubText.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_SUBTEXT)], 0.0, NULL, SDL_FLIP_NONE);
	}

	void close()
	{
		gDrone.free();
		gDroneWithCargo.free();
		gtextCSVfile.free();
		gtextDrone.free();
		gtextDroneWithCargo.free();
		gtextKeyboard.free();
		gKeyboard.free();
		gCSVfile.free();
		gTitle.free();
		gSubText.free();
	
		SDL_DestroyRenderer(gRenderer);
		SDL_DestroyWindow(gWindow);
		gWindow = NULL;
		gRenderer = NULL;

		IMG_Quit();
		SDL_Quit();
	}
};