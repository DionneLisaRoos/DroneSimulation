//==============================================================
// Filename : Graphics.h
// Authors : Dionne Ariëns and Marnick Los.
// Version : -
// License : -
// Description : This class creates a SDL window and shows the Cargo and Drone on 
//               screen as result of the simulation.
//==============================================================

#pragma once
#include <SDL_image.h>
#include <string>
#include <cmath>
#include <iostream>
#include <array>
#include <vector>

// Constants for image position in window since (0,0) is at the upper left pixel
const int SCREEN_WIDTH = 1500;
const int SCREEN_HEIGHT = 800;
const int METERS_TO_PIXELS = 50;
const int DRONE_CENTER_W = 51;
const int DRONE_CENTER_H = 26;
const int CARGO_CENTER_W = 20;
const int CARGO_CENTER_H = 16;

const int DRONE_OFFSET_W = (SCREEN_WIDTH / 2) - DRONE_CENTER_W;
const int CARGO_AND_DRONE_OFFSET_H = (SCREEN_HEIGHT / 2) + DRONE_CENTER_H;
const int CARGO_OFFSET_W = (DRONE_CENTER_W - CARGO_CENTER_W) + DRONE_OFFSET_W;

class Graphics
{
public:
	// Constructor: initialization of the private variables
	Graphics() {
		droneTexture = NULL;
		cargoTexture = NULL;
		droneWidth = 0;
		droneHeight = 0;
		cargoWidth = 0;
		cargoHeight = 0;

		gWindow = NULL;
		gRenderer = NULL;

		// Initialization of SDL graphics and loading of Drone and Cargo images
		// When either the initialization or loading of images fails, this command 
		// is printed to the screen.
		if (!init() || !loadImagesFromFile()) { std::cout << "Failed to initialize!\n"; }
	}

	// Deconstructor
	~Graphics() {
		free();
	}

	bool init() {
		// Initialize of video frame
		if (SDL_Init(SDL_INIT_VIDEO) < 0){ printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError()); return false;}
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) printf("Warning: Linear texture filtering not enabled!");

		// Create SDL window for showing images 
		gWindow = SDL_CreateWindow("Drone and Cargo game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) { printf("Window could not be created! SDL Error: %s\n", SDL_GetError()); return false; }
		
		// Create SDL render for the created window
		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (gRenderer == NULL){ printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError()); return false; }
		
		// Loads dynamic libraries and prepares them for use
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags)) { printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError()); return false; }
		
		// Return true when initialization went well
		return true;
	}

	// Loading images from file
	bool loadImagesFromFile()
	{
		return loadFromFileHelper();
	}

	// Add objects to renderer that need to be shown on the window
	void render(int xDrone, int yDrone, bool cargo, double angle = 0.0, int xCargo = 0, int yCargo = 0, double cargoAngle = 0.0) {
		// Create rectangle for the drone image with (x,y) position and the size of the image
		SDL_Rect rect = { xDrone, yDrone, droneWidth/2, droneHeight/2 };
		SDL_RenderCopyEx(gRenderer, droneTexture, NULL, &rect, -angle, NULL, SDL_FLIP_NONE);

		// If the system used the cargo, a rectangle for the cargo is created as well (for the (x,y) position and the size of the image)
		// In addition the rope between the drone and cargo is drawn
		if (cargo) {
			SDL_Rect rect = { xCargo, yCargo, cargoWidth/2, cargoHeight/2 };
			SDL_RenderCopyEx(gRenderer, cargoTexture, NULL, &rect, -cargoAngle, NULL, SDL_FLIP_NONE);

			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			SDL_RenderDrawLine(gRenderer, (xDrone + DRONE_CENTER_W), (yDrone + DRONE_CENTER_H), (xCargo + CARGO_CENTER_W), (yCargo + CARGO_CENTER_H));
		}
	}

	// Update the graphics window
	void updateGraphics(std::vector<double> xk) {
		// Get angle of drone and the position in pixels using conversion from meters to pixels
		double droneAngle = xk.at(2);
		int pxDrone = static_cast<int>(xk.at(0) * METERS_TO_PIXELS);
		int pyDrone = static_cast<int>(-xk.at(1) * METERS_TO_PIXELS);

		double cargoAngle = 0.0;
		int pxCargo = 0;
		int pyCargo = 0;

		bool cargo = false;

		// If the system uses the cargo and drone, get the angle of the cargo and the position in pixels using conversion from meters to pixels
		if (xk.size() == 9) {
			cargoAngle = 180 * std::atan2(xk[5] - xk[0], xk[1] - xk[6]) / M_PI;
			pxCargo = static_cast<int>(xk.at(5) * METERS_TO_PIXELS);
			pyCargo = static_cast<int>(-xk.at(6) * METERS_TO_PIXELS);

			cargo = true;
		}
		
		// Set renderer color, clear the old renderer and update the renderer with the new positions of the images.
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);
		render(DRONE_OFFSET_W + pxDrone, CARGO_AND_DRONE_OFFSET_H + pyDrone, cargo, droneAngle, CARGO_OFFSET_W + pxCargo, CARGO_AND_DRONE_OFFSET_H + pyCargo, cargoAngle);
		
		// Show rendered objects on window
		SDL_RenderPresent(gRenderer);
	}

	void close()
	{
		// Free memory
		free();

		// Destroy renderer and window
		SDL_DestroyRenderer(gRenderer);
		SDL_DestroyWindow(gWindow);
		gWindow = NULL;
		gRenderer = NULL;

		// Quit IMG and SDL
		IMG_Quit();
		SDL_Quit();
	}

	// Window object
	SDL_Window* gWindow;

	// Renderer object
	SDL_Renderer* gRenderer;

private:
	// Private variables
	SDL_Texture* droneTexture;
	SDL_Texture* cargoTexture;

	int droneWidth;
	int droneHeight;
	int cargoWidth;
	int cargoHeight;

	// Helper function for loading images from file
	bool loadFromFileHelper() {
		// Create two empty textures for the drone and cargo
		SDL_Texture* newDroneTexture = NULL;
		SDL_Texture* newCargoTexture = NULL;

		// Load the drone.png image and create texture for this image. Return false when this goes wrong.
		SDL_Surface* drone = IMG_Load("drone.png");
		newDroneTexture = SDL_CreateTextureFromSurface(gRenderer, drone);

		if (drone == NULL || newDroneTexture == NULL)
		{
			printf("Unable to load or create texture for image drone.png! SDL_image Error: %s\n", IMG_GetError());
			return false;
		}

		// Load the cargo.png image and create texture for this image. Return false when this goes wrong.
		SDL_Surface* cargo = IMG_Load("cargo.png");
		newCargoTexture = SDL_CreateTextureFromSurface(gRenderer, cargo);

		if (cargo == NULL || newCargoTexture == NULL)
		{
			printf("Unable to load or create texture for image cargo.png! SDL_image Error: %s\n", IMG_GetError());
			return false;
		}

		// Get the width and height of the images and save these in variables in this class
		droneWidth = drone->w;
		droneHeight = drone->h;
		cargoWidth = cargo->w;
		cargoHeight = cargo->h;

		// Free the surface used for loading the images
		SDL_FreeSurface(drone);
		SDL_FreeSurface(cargo);

		// Save the created textures for the drone and cargo
		droneTexture = newDroneTexture;
		cargoTexture = newCargoTexture;

		// Return true when loading of the images went well
		return true;
	}

	// Deallocate memory of the created textures of the drone and cargo. 
	// Also reset the width and heigth variables.
	void free() {
		if (droneTexture != NULL)
		{
			SDL_DestroyTexture(droneTexture);
			droneTexture = NULL;
			droneWidth = 0;
			droneHeight = 0;
		}
		if (cargoTexture != NULL)
		{
			SDL_DestroyTexture(cargoTexture);
			cargoTexture = NULL;
			cargoWidth = 0;
			cargoHeight = 0;
		}
	}
};