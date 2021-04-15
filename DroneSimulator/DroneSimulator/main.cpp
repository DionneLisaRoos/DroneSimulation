#include <iostream>
#include "Graphics.h"
#include "SimStatic.h"
#include "InputStatic.h"

int main(int argc, char* args[])
{
	double timeStep = 0.01;
	InputStatic input("input.csv", timeStep);
	std::vector<double> initialState = {0, 1, 0, 0, 0,0,0,0,0};
	//std::vector<double> initialState = { 0, 0, 0, 0, 0 };

	SimStatic staticSimulation(initialState, input, timeStep, 1);
	//staticSimulation.staticSimulateFullEuler();
	//staticSimulation.WriteCSV("outputEuler.csv");

	std::cout << "next\n";

	staticSimulation.staticSimulateFullRungeKutta();
	staticSimulation.WriteCSV("outputKutta.csv");

	return 0;
}


/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, standard IO, and strings
//#include <SDL.h>
//#include <stdio.h>
//#include <string>
//#include <iostream>
//
//
//int main(int argc, char* args[])
//{
//
//	//While the user hasn't quit
//	while (1)
//	{
//		////While there's events to handle
//		//while (SDL_PollEvent(&event))
//		//{
//		//	//If the user has Xed out the window
//		//	if (event.type == SDL_QUIT)
//		//	{
//		//		//Quit the program
//		//		quit = true;
//		//	}
//		//}
//
//		//Get the keystates
//		//Uint8* keystates = SDL_GetKeyState(NULL);
//
//		//If up is pressed
//		/*if (keystates[SDLK_UP])
//		{
//			std::cout << "UP";
//		}
//		else if (keystates[SDLK_DOWN])
//		{
//			std::cout << "DOWN";
//		}
//		else if (keystates[SDLK_LEFT])
//		{
//			std::cout << "LEFT";
//		}else if (keystates[SDLK_RIGHT])
//		{
//			std::cout << "RIGHT";
//		}
//		else {
//			std::cout << "----------------";
//		}*/
//
//
//		//Update the screen
//		/*if (SDL_Flip(screen) == -1)
//		{
//			return 1;
//		}*/
//	}
//
//	/*bool up = 0;
//	while (1) {
//		const Uint8* state = SDL_GetKeyboardState(NULL);
//		if (state[SDL_SCANCODE_RETURN]) {
//			printf("<RETURN> is pressed.\n");
//		}
//		if (state[SDL_SCANCODE_RIGHT] && state[SDL_SCANCODE_UP]) {
//			printf("Right and Up Keys Pressed.\n");
//		}
//	}
//	*/
//
//	////Start up SDL and create window
//	//if (!init())
//	//{
//	//	printf("Failed to initialize!\n");
//	//}
//	//else
//	//{
//	//	//Load media
//	//	if (!loadMedia())
//	//	{
//	//		printf("Failed to load media!\n");
//	//	}
//	//	else
//	//	{
//	//		//Main loop flag
//	//		bool quit = false;
//
//	//		//Event handler
//	//		SDL_Event e;
//	//		
//
//	//		//Set default current surface
//	//		gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
//
//	//		//While application is running
//	//		while (!quit)
//	//		{
//	//			//up = 0;
//	//			//const Uint8 *state = SDL_GetKeyboardState(NULL);
//	//			//Handle events on queue
//	//			//while (SDL_PollEvent(&e) != 0)
//	//			//{
//	//			//	//User requests quit
//	//			//	if (e.type == SDL_QUIT)
//	//			//	{
//	//			//		quit = true;
//	//			//	}
//	//			//	//User presses a key
//	//			//	else if (e.type == SDL_KEYDOWN)
//	//			//	{
//	//			//		//Select surfaces based on key press
//	//			//		
//	//			//		switch (e.key.keysym.sym)
//	//			//		{
//	//			//		case SDLK_UP:
//	//			//			gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
//	//			//			//std::cout << "pushed\n";
//	//			//			up = 1;
//	//			//			break;
//
//	//			//		case SDLK_DOWN:
//	//			//			gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
//	//			//			break;
//
//	//			//		case SDLK_LEFT:
//	//			//			gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
//	//			//			break;
//
//	//			//		case SDLK_RIGHT:
//	//			//			gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
//	//			//			break;
//
//	//			//		default:
//	//			//			gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
//	//			//			//std::cout << "default\n";
//	//			//			break;
//	//			//		}
//	//			//	}
//	//			//}
//	//			/*const Uint8* state = SDL_GetKeyboardState(NULL);
//
//	//			if (state[SDL_SCANCODE_UP]) up = 1;
//	//			else up = 0;
//
//	//			if (!up) std::cout << "not pushed\n";
//	//			else std::cout << "--------------------------\n";
//
//	//			Apply the current image
//	//			SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
//
//	//			Update the surface
//	//			SDL_UpdateWindowSurface(gWindow);*/
//	//		}
//	//	}
//	//}
//
//	//Free resources and close SDL
//	//close();
//
//	return 0;
//}