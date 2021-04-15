#include <iostream>
#include "SDL.h"
#include "Graphics.h"
#include "SimStatic.h"
#include "SimDynamic.h"
#include "InputStatic.h"

int main(int argc, char* args[])
{

	/* static */

	/*
	double timeStep = 0.01;
	InputStatic input("input.csv", timeStep);
	std::vector<double> initialState = {0, 1, 0, 0, 0,0,0,0,0};
	std::vector<double> initialState = { 0, 0, 0, 0, 0 };

	SimStatic staticSimulation(initialState, input, timeStep, 1);
	staticSimulation.staticSimulateFullEuler();
	staticSimulation.WriteCSV("outputEuler.csv");

	std::cout << "next\n";

	staticSimulation.staticSimulateFullRungeKutta();
	staticSimulation.WriteCSV("outputKutta.csv");
	*/


	/* dynamic */
	int debugCnt1 = 0;
	int debugCnt2 = 0;
	std::cout << "entering declarations section\n";

	int FPS = 20;
	double timeStep = 0.005;
	bool quit = 0 ;
	double maxThrust = 2 * 5 * 9.81;
	double tiltRate = 5;
	std::vector<double> initialState = { 0, 1, 0, 0, 0, 0, 0, 0, 0 };
	std::array<double, 2> initialInput = {1.5*9.81, 0};
	
	// flags to track key input
	bool KeyRight=0, KeyLeft=0, KeyUp=0;

	std::cout << "entering initialization section\n";
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
		//std::cout << "entering game loop "<<debugCnt1<<"\n";
		//debugCnt1++;
		graphics.updateGraphics(xk.at(0), xk.at(1), xk.at(2), xk.at(5), xk.at(6));
		xk = dynamicSimulator.simToNextFrame(uk);

		uk = { 0,0 };

		// wait for 1/FPS seconds to pass and meanwhile handle key events
		while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeout)){
			//std::cout << "entering frame loop "<<debugCnt2<<" \n";
			//debugCnt2++;;
			// Handle all input events
			while (SDL_PollEvent(&e))
			{
				switch (e.type)
				{
				// handle registered keypresses
				case SDL_KEYDOWN:
					std::cout << "toets in\n";
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
					default:
						break;
					}
				break;
				//handle keyreleases
				case SDL_KEYUP:
					std::cout << "toets op\n";
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
					default:
						break;
					}
				break;
				default:
					break;
				}
			}//end processing event

		// generate input values according to keypress flags
		if (KeyUp)uk[0] = maxThrust;
		if (KeyLeft && !KeyRight) uk[1] = tiltRate;
		else if (!KeyLeft && KeyRight) uk[1] = -tiltRate;
		else uk[1] = 0;
		}//exit time-to-next-render loop 

		timeout += 1000 / FPS;
	}
	dynamicSimulator.writeCSV();


	return 0;
}

