#include <iostream>
#include "SDL.h"
#include "Graphics.h"
#include "SimStatic.h"
#include "SimDynamic.h"
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

	/* dynamic */
	int FPS = 20;
	bool quit = 0 ;
	double maxThrust = 2 * 5 * 9.81;
	double tiltRate = 5;
	std::vector<double> initialState = { 0, 1, 0, 0, 0, 0, 0, 0, 0 };
	std::array<double, 2> initialInput = {5*9.81, 0};
	
	// flags to track key input
	bool KeyRight=0, KeyLeft=0, KeyUp=0;

	// initialize graphics, simulator, timeout duration and event handler
	Graphics graphics(1);
	SimDynamic dynamicSimulator(initialState, initialInput, timeStep, FPS);
	Uint32 timeout = SDL_GetTicks() + 1000 / FPS;
	SDL_Event e;

	// pass initial values to working variables
	std::vector<double> xk = initialState;
	std::array<double,2> uk = initialInput;
	

	while (!quit)
	{
		graphics.updateGraphics(xk.at(0), xk.at(1), xk.at(2), xk.at(5), xk.at(6));
		xk = dynamicSimulator.simToNextFrame(uk);

		uk = { 0,0 };

		// wait for 1/FPS seconds to pass and meanwhile handle key events
		while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeout)){
			// Handle all waiting input events

			while (SDL_PollEvent(&e))
			{
				switch (e.type)
				{
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym)
					{
					case SDLK_LEFT:
						KeyLeft = 1;
						break;
					case SDLK_RIGHT:
						KeyRight = 1;
						break;
					case SDLK_UP:
						KeyUp = 1;
						break;
					case SDLK_q:
						quit = 1;
						break;
					case SDLK_ESCAPE:
						quit = 1;
						break;
					}
				case SDL_KEYUP:
					switch (e.key.keysym.sym)
					{
					case SDLK_LEFT:
						KeyLeft = 0;
						break;
					case SDLK_RIGHT:
						KeyRight = 0;
						break;
					case SDLK_UP:
						KeyUp = 0;
						break;
					}
				}
			}//end processing event
		}//exit time-to-next-render loop 
		if (KeyUp) uk[0] = maxThrust;
		if (KeyLeft) uk[1] -= tiltRate;
		if (KeyRight) uk[1] += tiltRate;
		timeout += 1000 / FPS;
	}



	return 0;
}

//while (SDL_PollEvent(&e)) {}

/*
			Uint8 *keystates = SDL_GetKeyState(NULL);
			if (keystates[SDLK_UP]) uk[0] = maxThrust;
			if (keystates[SDLK_LEFT]) uk[1] = -tiltRate;
			if (keystates[SDLK_RIGHT]) uk[1] = tiltRate;
*/

//Get the keystates
// //Uint8* keystates = SDL_GetKeyState(NULL);
//
// //If up is pressed
// /*if (keystates[SDLK_UP])
// {
// std::cout << "UP";
// }
// else if (keystates[SDLK_DOWN])
// {
// std::cout << "DOWN";
// }
// else if (keystates[SDLK_LEFT])
// {
// std::cout << "LEFT";
// }else if (keystates[SDLK_RIGHT])
// {
// std::cout << "RIGHT";
// }
// else {
// std::cout << "----------------";
// }*/
//


//Event handler
// // SDL_Event e;
// //
//
// // //Set default current surface
// // gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
//
// // //While application is running
// // while (!quit)
// // {
// // //up = 0;
// // //const Uint8 *state = SDL_GetKeyboardState(NULL);
// // //Handle events on queue
// // //while (SDL_PollEvent(&e) != 0)
// // //{
// // // //User requests quit
// // // if (e.type == SDL_QUIT)
// // // {
// // // quit = true;
// // // }
// // // //User presses a key
// // // else if (e.type == SDL_KEYDOWN)
// // // {
// // // //Select surfaces based on key press
// // //
// // // switch (e.key.keysym.sym)
// // // {
// // // case SDLK_UP:
// // // gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
// // // //std::cout << "pushed\n";
// // // up = 1;
// // // break;
//
// // // case SDLK_DOWN:
// // // gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
// // // break;
//
// // // case SDLK_LEFT:
// // // gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
// // // break;
//
// // // case SDLK_RIGHT:
// // // gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
// // // break;
//
// // // default:
// // // gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
// // // //std::cout << "default\n";
// // // break;
// // // }
// // // }
// // //}