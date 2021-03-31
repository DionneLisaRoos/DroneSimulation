#include "InputStatic.h"

int main(int argc, char* args[])
{
	double timestep = 0.1;
	InputStatic input("input.csv", timestep);
	input.getInput(13.3, 22.1);
	//std::array<double, 2> i1 = input.getInput(22);
	//std::array<double, 2> i2 = input.getInput(3);
	//std::array<double, 2> i3 = input.getInput(25);
	//std::array<double, 2> i4 = input.getInput(17);
	//std::array<double, 2> i5 = input.getInput(18);


	//std::cout << "\n\nt = 22; input u =" << i1[0] << ", " << i1[1] << std::endl;
	//std::cout << "t = 3; input u =" << i2[0] << ", " << i2[1] << std::endl;
	//std::cout << "t = 25; input u =" << i3[0] << ", " << i3[1] << std::endl;
	//std::cout << "t = 17; input u =" << i4[0] << ", " << i4[1] << std::endl;
	//std::cout << "t = 18; input u =" << i5[0] << ", " << i5[1] << std::endl;

	//i1 = input.getInput(1);
	//std::cout << "\n\nt = 1; input u =" << i1[0] << ", " << i1[1] << std::endl;

	//i1 = input.getInput(8);
	//std::cout << "\n\nt = 8; input u =" << i1[0] << ", " << i1[1] << std::endl;

	//i1 = input.getInput(23);
	//std::cout << "\n\nt = 23; input u =" << i1[0] << ", " << i1[1] << std::endl;

	//i1 = input.getInput(0);
	//std::cout << "\n\nt = 0; input u =" << i1[0] << ", " << i1[1] << std::endl;

	//i1 = input.getInput(10);
	//std::cout << "\n\nt = 10; input u =" << i1[0] << ", " << i1[1] << std::endl;

	//i1 = input.getInput(28);
	//std::cout << "\n\nt = 28; input u =" << i1[0] << ", " << i1[1] << std::endl;

	return 0;
}




//
//#include <SDL.h>
//#include <SDL_image.h>
//#include <stdio.h>
//#include <string>
//#include <cmath>
//#include <iostream>
//#include <array>
//#include <algorithm>
//#pragma once
//
//const int SCREEN_WIDTH = 1040;
//const int SCREEN_HEIGHT = 680;
//const int DRONE_CENTER_W = 103;
//const int DRONE_CENTER_H = 53;
//const int CARGO_CENTER_W = 41; 
//const int CARGO_CENTER_H = 32; 
//const int CARGO_CENTER_TO_DRONE_H = DRONE_CENTER_H + static_cast<int>(3 * DRONE_CENTER_H);
//const int CARGO_SIDE_TO_DRONE_H = CARGO_CENTER_H - 24;
//
//
//class Graphics
//{
//public:
//	Graphics() {
//		droneTexture = NULL;
//		cargoTexture = NULL;
//		droneWidth = 0;
//		droneHeight = 0;
//		cargoWidth = 0;
//		cargoHeight = 0;
//		droneX = 0;
//		droneY = 0;
//
//		gWindow = NULL;
//		gRenderer = NULL;
//
//		if (!init() || !loadImagesFromFile())
//		{
//			printf("Failed to initialize!\n");
//		}
//	}
//	~Graphics() {
//		free();
//	}
//
//	bool init() {
//		if (SDL_Init(SDL_INIT_VIDEO) < 0)
//		{
//			printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
//			return false;
//		}
//		else
//		{
//			if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) printf("Warning: Linear texture filtering not enabled!");
//
//			gWindow = SDL_CreateWindow("Drone and Cargo game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
//
//			if (gWindow == NULL)
//			{
//				printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
//				return false;
//			}
//			else
//			{
//				gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//
//				if (gRenderer == NULL)
//				{
//					printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
//					return false;
//				}
//				else
//				{
//					int imgFlags = IMG_INIT_PNG;
//					if (!(IMG_Init(imgFlags) & imgFlags))
//					{
//						printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
//						return false;
//					}
//				}
//			}
//		}
//
//		return true;
//	}
//
//	bool loadImagesFromFile()
//	{
//		return loadFromFileHelper();
//	}
//
//	void render(int x, int y, bool drone, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE) {
//		if (drone) {
//			droneX = x + DRONE_CENTER_W;
//			droneY = y + DRONE_CENTER_H;
//
//			SDL_Rect renderQuad = { x, y, droneWidth, droneHeight };
//			SDL_RenderCopyEx(gRenderer, droneTexture, clip, &renderQuad, angle, center, flip);
//		}
//		else {
//			SDL_Rect renderQuad = { x, y, cargoWidth, cargoHeight };
//			SDL_RenderCopyEx(gRenderer, cargoTexture, clip, &renderQuad, angle, center, flip);
//
//			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
//			SDL_RenderDrawLine(gRenderer, droneX, droneY, (x + CARGO_CENTER_W), (y + CARGO_SIDE_TO_DRONE_H));
//		}
//		
//	}
//
//	void close()
//	{
//		free();
//	
//		SDL_DestroyRenderer(gRenderer);
//		SDL_DestroyWindow(gWindow);
//		gWindow = NULL;
//		gRenderer = NULL;
//
//		IMG_Quit();
//		SDL_Quit();
//	}
//
//	SDL_Window* gWindow; 
//
//	SDL_Renderer* gRenderer; 
//
//private:
//	SDL_Texture* droneTexture;
//	SDL_Texture* cargoTexture;
//
//	int droneWidth;
//	int droneHeight;
//	int cargoWidth;
//	int cargoHeight;
//
//	int droneX;
//	int droneY;
//
//	bool loadFromFileHelper() {
//		free();
//
//		SDL_Texture* newDroneTexture = NULL;
//		SDL_Texture* newCargoTexture = NULL;
//
//		SDL_Surface* drone = IMG_Load("drone.png");
//		newDroneTexture = SDL_CreateTextureFromSurface(gRenderer, drone);
//
//		if (drone == NULL || newDroneTexture == NULL)
//		{
//			printf("Unable to load or create texture for image drone.png! SDL_image Error: %s\n", IMG_GetError());
//			return false;
//		}
//
//		SDL_Surface* cargo = IMG_Load("cargo.png");
//		newCargoTexture = SDL_CreateTextureFromSurface(gRenderer, cargo);
//
//		if (cargo == NULL || newCargoTexture == NULL)
//		{
//			printf("Unable to load or create texture for image cargo.png! SDL_image Error: %s\n", IMG_GetError());
//			return false;
//		}
//
//		droneWidth = drone->w;
//		droneHeight = drone->h;
//		cargoWidth = cargo->w;
//	    cargoHeight = cargo->h;
//
//		//std::cout << "drone w: " << droneWidth << "\th: " << droneHeight << "\ncargo w: " << cargoWidth << "\th: " << cargoHeight << "\n";
//
//		SDL_FreeSurface(drone);
//		SDL_FreeSurface(cargo);
//
//		droneTexture = newDroneTexture;
//		cargoTexture = newCargoTexture;
//
//		return true;
//	}
//
//	void free() {
//		if (droneTexture != NULL)
//		{
//			SDL_DestroyTexture(droneTexture);
//			droneTexture = NULL;
//			droneWidth = 0;
//			droneHeight = 0;
//		}
//		if (cargoTexture != NULL)
//		{
//			SDL_DestroyTexture(cargoTexture);
//			cargoTexture = NULL;
//			cargoWidth = 0;
//			cargoHeight = 0;
//		}
//	}
//};
//
//int main(int argc, char* args[])
//{
//	Graphics gArrowTexture;
//
//	//if (!gArrowTexture.init())
//	//{
//	//	printf("Failed to initialize!\n");
//	//}
//	//else
//	//{
//		//if (!gArrowTexture.loadImagesFromFile())
//		//{
//		//	printf("Failed to load arrow texture!\n");
//		//}
//		//else
//		//{
//			//Main loop flag
//			bool quit = false;
//
//			//Event handler
//			SDL_Event e;
//
//			//Angle of rotation
//			double degrees = 0;
//
//			//Flip type
//			SDL_RendererFlip flipType = SDL_FLIP_NONE;
//
//			//While application is running
//			while (!quit)
//			{
//				//Handle events on queue
//				while (SDL_PollEvent(&e) != 0)
//				{
//					//User requests quit
//					if (e.type == SDL_QUIT)
//					{
//						quit = true;
//					}
//					else if (e.type == SDL_KEYDOWN)
//					{
//						switch (e.key.keysym.sym)
//						{
//						case SDLK_a:
//							degrees -= 60;
//							break;
//
//						case SDLK_d:
//							degrees += 60;
//							break;
//
//						case SDLK_q:
//							flipType = SDL_FLIP_HORIZONTAL;
//							break;
//
//						case SDLK_w:
//							flipType = SDL_FLIP_NONE;
//							break;
//
//						case SDLK_e:
//							flipType = SDL_FLIP_VERTICAL;
//							break;
//						}
//					}
//				}
//
//				//Clear screen
//				SDL_SetRenderDrawColor(gArrowTexture.gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
//				SDL_RenderClear(gArrowTexture.gRenderer);
//				//Render arrow
//				gArrowTexture.render((SCREEN_WIDTH/2) - DRONE_CENTER_W, (SCREEN_HEIGHT/2) + DRONE_CENTER_H, true, NULL, degrees, NULL, flipType);
//				gArrowTexture.render((SCREEN_WIDTH / 2) - CARGO_CENTER_W, (SCREEN_HEIGHT / 2) + CARGO_CENTER_TO_DRONE_H, false, NULL, degrees, NULL, flipType);
//
//
//				//Update screen
//				SDL_RenderPresent(gArrowTexture.gRenderer);
//			}
//		//}
//	//}
//
//	gArrowTexture.close();
//
//	return 0;
//}

