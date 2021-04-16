//==============================================================
// Filename : GUI.h
// Authors : Dionne Ariëns and Marnick Los.
// Version : -
// License : -
// Description : This class generates a GUI that enables the user 
//				 of the game to set some intial settings before the game starts.
//==============================================================

#pragma once
#include "LTexture.h"
#include "LButton.h"

// Enum class to track each image (used in the settings GUI) induvidually
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
	// Set initial values to these booleans and initialize the SLD window and load the images
	GUI() : keyboard(false), cargo(false), gWindow(NULL), gRenderer(NULL){
		// The boolean done is used to see whether the whole settings GUI is being processed before quiting this page. 
		// Because when the GUI is closed early, the simulation should not start. This boolean makes sure the program 
		// does not start when the settings are not propperly set.
		done = false;
		if (!init()) { printf("Failed to initialize!\n"); return; }
		if (!loadMedia()) { printf("Failed to load media!\n"); return; }
	}

	~GUI() {}

	// This function starts the GUI that askes on the initial settings of the game
	void startGUI() {
		bool choice = false;
		bool fileFlag = false;
		bool quit = false;

		// Create SDL event to be able to detect Key inputs
		SDL_Event e;

		while (!quit)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				// Quit program when QUIT button is pushed
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}

				// For all created buttons (in total 2) we check whether the button is pushed or not
				for (int i = 0; i < 2; ++i)
				{
					gButtons[i].handleEvent(&e);
				}
			}

			// If the button for using static input is pushed, set the keyboard booelan to true and quit the program
			if (gButtons[0].isPushed() && choice) {
				keyboard = false;
				done = true;
				quit = true;
			}
			// If the button for using dynamic input is pushed, set the keyboard boolean to true and quit the program
			else if (gButtons[1].isPushed() && choice) {
				keyboard = true;
				done = true;
				quit = true;
			}
			// If the button Drone is pushed set to choice is true so this button will not be showed again. 
			// Also clear the flag of the button so it is able to be pushed again in the next window.
			else if (gButtons[0].isPushed()) {
				cargo = false;
				gButtons[0].clearPushedFlag();
				choice = true;
			}
			// If the button Drone with Cargo is pushed set to choice is true so this button will not be showed again. 
			// Also clear the flag of the button so it is able to be pushed again in the next window.
			else if (gButtons[1].isPushed()) {
				cargo = true;
				gButtons[1].clearPushedFlag();
				choice = true;
			}

			// Set render color and clear the old renderer
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			// Render the new objects
			render(choice);

			// Show the renderer on the window
			SDL_RenderPresent(gRenderer);
		}

		// Close the window and release all memory of the GUI.
		close();
	}

	// Returns whether the cargo is added to the system or not.
	bool getCargoChoice() { return cargo; }

	// Returns whether keyboard input is used or not.
	bool getKeyboardChoice() { return keyboard; }

	// Implies wheter the GUI is closed early (which means that the program should quit) or not.
	bool isGuiFinished() { return done; }

private:
	// Private objects
	SDL_Window* gWindow;
	SDL_Renderer* gRenderer;

	// Create an SDL_rect array that holds all positions and sizes of all images
	SDL_Rect gImages[static_cast<int>(Images::TOTAL_IMAGES)];

	// Create textures for all images in the GUI
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

	// Create an array of buttons for the 2 buttons that are showed in the setting page. 
	LButton gButtons[2];

	// Private variables
	bool cargo;
	bool keyboard;
	bool done;

	// Initialize SDL window and renderer
	bool init()
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0) { printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError()); return false; }
		
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) { printf("Warning: Linear texture filtering not enabled!"); }
		
		// Create SDL window
		gWindow = SDL_CreateWindow("Settings page", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 400, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) { printf("Window could not be created! SDL Error: %s\n", SDL_GetError()); return false; }

		//Create SDL renderer for window
		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (gRenderer == NULL) { printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError()); return false; }
		
		// Update all images with the created renderer and pass the renderer by reference. 
		// We could choose to give the renderer as input in the constructor but to pass an object by 
		// reference, it can not be NULL. When the renderer is created, it is set to NULL. Therefore the 
		// renderer is passed by reference here, just after the renderer got an useful value.
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
		
		// Set color for renderer
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

		//Initialize PNG loading
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags)) { printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError()); return false; }
		
		// Return true when initialization of window and renderer went well.
		return true;
	}

	// This function loads all media and set all positions and sizes of the loaded images. 
	bool loadMedia()
	{
		// Load images from file
		if (!gDrone.loadFromFile("DroneButton.png")) { printf("Failed to load DroneButton.pgn!\n"); return false; }
		if (!gDroneWithCargo.loadFromFile("DroneAndCargo.png")) { printf("Failed to load DroneAndCargo.png!\n"); return false; }
		if (!gKeyboard.loadFromFile("Keyboard.png")) { printf("Failed to load Keyboard.png!\n"); return false; }
		if (!gCSVfile.loadFromFile("CSV.png")) { printf("Failed to load CSV.png!\n"); return false; }
		if (!gtextDrone.loadFromFile("DroneChoice.png")) { printf("Failed to load DroneChoice.png!\n"); return false; }
		if (!gtextDroneWithCargo.loadFromFile("DroneWithCargoChoice.png")) { printf("Failed to load DroneWithCargoChoice!\n"); return false; }
		if (!gTitle.loadFromFile("Title.png")) { printf("Failed to load Title.png!\n"); return false; }
		if (!gtextKeyboard.loadFromFile("KeyboardInputChoice.png")) { printf("Failed to load KeyboardInputChoice.png!\n"); return false; }
		if (!gtextCSVfile.loadFromFile("InputFileChoice.png")) { printf("Failed to load InputFileChoice.png!\n"); return false; }
		if (!gSubText.loadFromFile("SubText.png")) { printf("Failed to load SubText.png!\n"); return false; }

		// fill the images SDL_Rect with the xy positions and size of the loaded images
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

		// Create the two buttons with xy position and size
		gButtons[0].setPosition(60, 180, gKeyboard.getWidth(), gDroneWithCargo.getHeight());
		gButtons[1].setPosition(380, 180, gKeyboard.getWidth(), gDroneWithCargo.getHeight());

		// Return true when images loading went well
		return true;
	}

	// Function that fills the SDL_Rect object for given index with xy position and size
	void fillImagePosition(int i, int x, int y, LTexture& texture) {
		gImages[i].x = x;
		gImages[i].y = y;
		gImages[i].w = texture.getWidth();
		gImages[i].h = texture.getHeight();
	}

	// This function renders the images that should be shown on the window into the renderer.
	void render(bool choice)
	{
		// Get position of the buttons and draw these on the screen
		SDL_Rect rect = gButtons[0].getPosition();
		SDL_Rect rect1 = gButtons[1].getPosition();
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(gRenderer, &rect);
		SDL_RenderDrawRect(gRenderer, &rect1);

		// When the first choice of the settings data (with or without cargo) is not made yet, add the drone and drone and cargo images into the renderer.
		if (!choice) {
			SDL_RenderCopyEx(gRenderer, gDrone.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_DRONE)], 0.0, NULL, SDL_FLIP_NONE);
			SDL_RenderCopyEx(gRenderer, gDroneWithCargo.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_DRONE_AND_CARGO)], 0.0, NULL, SDL_FLIP_NONE);
			SDL_RenderCopyEx(gRenderer, gtextDrone.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_TEXT_DRONE)], 0.0, NULL, SDL_FLIP_NONE);
			SDL_RenderCopyEx(gRenderer, gtextDroneWithCargo.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_TEXT_DRONE_AND_CARGO)], 0.0, NULL, SDL_FLIP_NONE);
		}
		// When the first choice of the settings data (with or without cargo) is made, add the CSV file input and keyboard input images into the renderer.
		else {
			SDL_RenderCopyEx(gRenderer, gCSVfile.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_CSV_FILE)], 0.0, NULL, SDL_FLIP_NONE);
			SDL_RenderCopyEx(gRenderer, gKeyboard.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_KEYBOARD)], 0.0, NULL, SDL_FLIP_NONE);
			SDL_RenderCopyEx(gRenderer, gtextCSVfile.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_TEXT_CSV_FILE)], 0.0, NULL, SDL_FLIP_NONE);
			SDL_RenderCopyEx(gRenderer, gtextKeyboard.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_TEXT_KEYBOARD)], 0.0, NULL, SDL_FLIP_NONE);
		}

		// Add the title and subtile of the settings page into renderer
		SDL_RenderCopyEx(gRenderer, gTitle.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_TITLE)], 0.0, NULL, SDL_FLIP_NONE);
		SDL_RenderCopyEx(gRenderer, gSubText.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_SUBTEXT)], 0.0, NULL, SDL_FLIP_NONE);
	}

	// Deallocate all textures of all images and destroy the renderer and window
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

		// Quit IMG and SDL
		IMG_Quit();
		SDL_Quit();

	}
};