#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>
#include <array>
#include <algorithm>
#pragma once

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int METERS_TO_PIXELS = 10;
const int DRONE_CENTER_W = 103;
const int DRONE_CENTER_H = 53;
const int CARGO_CENTER_W = 41;
const int CARGO_CENTER_H = 32;
const int CARGO_CENTER_TO_DRONE_H = DRONE_CENTER_H + static_cast<int>(3 * DRONE_CENTER_H);
const int CARGO_SIDE_TO_DRONE_H = CARGO_CENTER_H - 24;

const int INIT_DRONE_W = (SCREEN_WIDTH / 2) - DRONE_CENTER_W;
const int INIT_DRONE_H = (SCREEN_HEIGHT / 2) + DRONE_CENTER_H;
const int INIT_CARGO_W = (SCREEN_WIDTH / 2) - CARGO_CENTER_W;
const int INIT_CARGO_H = (SCREEN_HEIGHT / 2) + CARGO_CENTER_TO_DRONE_H;

class Graphics
{
public:
	Graphics(bool Cargo) {
		cargo = Cargo;
		droneTexture = NULL;
		cargoTexture = NULL;
		droneWidth = 0;
		droneHeight = 0;
		cargoWidth = 0;
		cargoHeight = 0;
		droneX = 0;
		droneY = 0;

		gWindow = NULL;
		gRenderer = NULL;

		if (!init() || !loadImagesFromFile())
		{
			printf("Failed to initialize!\n");
		}
	}
	~Graphics() {
		free();
	}

	bool init() {
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
			return false;
		}
		else
		{
			if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) printf("Warning: Linear texture filtering not enabled!");

			gWindow = SDL_CreateWindow("Drone and Cargo game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

			if (gWindow == NULL)
			{
				printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
				return false;
			}
			else
			{
				gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

				if (gRenderer == NULL)
				{
					printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
					return false;
				}
				else
				{
					int imgFlags = IMG_INIT_PNG;
					if (!(IMG_Init(imgFlags) & imgFlags))
					{
						printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
						return false;
					}
				}
			}
		}

		return true;
	}

	bool loadImagesFromFile()
	{
		return loadFromFileHelper();
	}

	void render(int x, int y, bool withCargo, SDL_Rect* clip = NULL, double angle = 0.0, int cargox = 0, int cargoy = 0, double cargoAngle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE) {
		droneX = x + DRONE_CENTER_W;
		droneY = y + DRONE_CENTER_H;

		SDL_Rect renderQuad = { x, y, droneWidth, droneHeight };
		SDL_RenderCopyEx(gRenderer, droneTexture, clip, &renderQuad, angle, center, flip);
		std::cout << "drone xy: " << (x + DRONE_CENTER_W) << " " << (y + DRONE_CENTER_H) << std::endl;


		if (withCargo) {
			SDL_Rect renderQuad = { cargox, cargoy, cargoWidth, cargoHeight };
			SDL_RenderCopyEx(gRenderer, cargoTexture, clip, &renderQuad, cargoAngle, center, flip);
			std::cout << "cargo xy: " << (x + CARGO_CENTER_W) << " " << (y + CARGO_SIDE_TO_DRONE_H) << std::endl;

			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			SDL_RenderDrawLine(gRenderer, (x + DRONE_CENTER_W), (y + DRONE_CENTER_H), (cargox + CARGO_CENTER_W), (cargoy + CARGO_SIDE_TO_DRONE_H));
		}
	}

	void updateGraphics(double x, double y, double degrees, double cargox = 0, double cargoy = 0, double cargoDegrees = 0) {
		// TODO: meters to pixels
		int pixelX = static_cast<int>(x * METERS_TO_PIXELS);
		int pixelY = static_cast<int>(-y * METERS_TO_PIXELS);
		int px = static_cast<int>(cargox * METERS_TO_PIXELS);
		int py = static_cast<int>(-cargoy * METERS_TO_PIXELS);


		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);
		render(INIT_DRONE_W + pixelX, INIT_DRONE_H + pixelY, cargo, NULL, degrees, INIT_CARGO_W + px, INIT_CARGO_H + py, cargoDegrees);

		SDL_RenderPresent(gRenderer);
	}

	void close()
	{
		free();

		SDL_DestroyRenderer(gRenderer);
		SDL_DestroyWindow(gWindow);
		gWindow = NULL;
		gRenderer = NULL;

		IMG_Quit();
		SDL_Quit();
	}

	SDL_Window* gWindow;

	SDL_Renderer* gRenderer;

private:
	SDL_Texture* droneTexture;
	SDL_Texture* cargoTexture;

	int droneWidth;
	int droneHeight;
	int cargoWidth;
	int cargoHeight;

	int droneX;
	int droneY;

	bool cargo;

	bool loadFromFileHelper() {
		free();

		SDL_Texture* newDroneTexture = NULL;
		SDL_Texture* newCargoTexture = NULL;

		SDL_Surface* drone = IMG_Load("drone.png");
		newDroneTexture = SDL_CreateTextureFromSurface(gRenderer, drone);

		if (drone == NULL || newDroneTexture == NULL)
		{
			printf("Unable to load or create texture for image drone.png! SDL_image Error: %s\n", IMG_GetError());
			return false;
		}

		SDL_Surface* cargo = IMG_Load("cargo.png");
		newCargoTexture = SDL_CreateTextureFromSurface(gRenderer, cargo);

		if (cargo == NULL || newCargoTexture == NULL)
		{
			printf("Unable to load or create texture for image cargo.png! SDL_image Error: %s\n", IMG_GetError());
			return false;
		}

		droneWidth = drone->w;
		droneHeight = drone->h;
		cargoWidth = cargo->w;
		cargoHeight = cargo->h;

		SDL_FreeSurface(drone);
		SDL_FreeSurface(cargo);

		droneTexture = newDroneTexture;
		cargoTexture = newCargoTexture;

		return true;
	}

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