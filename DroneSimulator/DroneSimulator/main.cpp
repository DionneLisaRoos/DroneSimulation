#include <iostream>
#include "SDL.h"
#include "Graphics.h"
#include "SimStatic.h"
#include "SimDynamic.h"
#include "InputStatic.h"
#include <thread>
#include <chrono>
#include <mutex>


std::mutex mtx;

void simOnThread(SimDynamic& simulator, bool& quit, int simFrequency)
{
	using namespace std::chrono_literals;
	std::cout << "entered sim thread" << '\n';
	std::chrono::duration<double, std::milli> sleepTime = 1000ms/simFrequency;
	while (!quit)
	{
		auto preSim = std::chrono::high_resolution_clock::now();
		mtx.lock();
		simulator.updateStates();
		mtx.unlock();
		auto postSim = std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(sleepTime - (postSim - preSim));
	}
}
void renderOnThread(SimDynamic& simulator, Graphics& graphics, bool& quit, int FPS)
{
	std::cout << "entering rendering thread" << '\n';
	std::vector<double> xk;
	Uint32 timeoutRender = SDL_GetTicks() + 1000 / FPS;
	while (!quit)
	{
		while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeoutRender))
		{
			// fetch current state, and render updated graphics
			xk = simulator.getState();
			graphics.updateGraphics(xk.at(0), xk.at(1), xk.at(2), xk.at(5), xk.at(6));
		}
		timeoutRender += 1000 / FPS;
	}
}


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

	//frames per second, simulation steps per second
	int FPS = 60;
	int SimPS = 100;
	double timeStep = 1.0/SimPS;

	// global exit flag
	bool quit = 0 ;

	// flag for tracking keypresses, input values, increase for more drone yeet
	bool KeyRight = 0, KeyLeft = 0, KeyUp = 0, KeyDown = 0;
	double maxThrust = 4 * 5 * 9.81;
	double tiltRate = 60;

	// initial position of the drone
	std::vector<double> initialState = { 0, 1, 0, 0, 0, 0, 1, 0, 0 };
	std::array<double, 2> initialInput = {5*9.81, 0};
	
	// initialize graphics, simulator, timeout duration and event handler
	Graphics graphics(1);
	SimDynamic dynamicSimulator(initialState, initialInput, timeStep, FPS);

	// to thread or not to thread, that is the question
	bool thread=1;

	// event obj
	SDL_Event e;

	// gameloop threaded
	if (thread)
	{
		std::cout << "entering threaded simulation" << '\n';
		std::thread simThread(simOnThread, std::ref(dynamicSimulator), std::ref(quit), SimPS);
		std::thread renderThread(renderOnThread, std::ref(dynamicSimulator), std::ref(graphics), std::ref(quit), FPS);
		std::cout << "created threads." << '\n';

		while (!quit)
		{
			while (SDL_PollEvent(&e))
			{
				switch (e.type)
				{
				// handle keypresses
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym)
					{
					case SDLK_UP:
						KeyUp = 1;
						break;
					case SDLK_DOWN:
						KeyDown = 1;
						break;
					case SDLK_LEFT:
						KeyLeft = 1;
						break;
					case SDLK_RIGHT:
						KeyRight = 1;
						break;
					case SDLK_q:
						mtx.lock();
						quit = 1;
						mtx.unlock();
						break;
					case SDLK_ESCAPE:
						mtx.lock();
						quit = 1;
						mtx.unlock();
						break;
					default:
						break;
					}
					break;
				// handle keyreleases
				case SDL_KEYUP:
					switch (e.key.keysym.sym)
					{
					case SDLK_UP:
						KeyUp = 0;
						break;
					case SDLK_DOWN:
						KeyDown = 0;
						break;
					case SDLK_LEFT:
						KeyLeft = 0;
						break;
					case SDLK_RIGHT:
						KeyRight = 0;
						break;
					default:
						break;
					}
					break;
				default:
					break;
				}
			mtx.lock();
			if (!KeyDown && KeyUp) dynamicSimulator.setInput(0, maxThrust);
			else if (KeyDown && !KeyUp) dynamicSimulator.setInput(0, -maxThrust);
			else dynamicSimulator.setInput(0, 0);

			if (!KeyLeft && KeyRight) dynamicSimulator.setInput(1, -tiltRate);
			else if (KeyLeft && !KeyRight) dynamicSimulator.setInput(1, tiltRate);
			else dynamicSimulator.setInput(1, 0);
			mtx.unlock();
			std::cout << "processed an event\n";
			}// end processing event

		}
		std::cout << "quit." << '\n';

		simThread.join();
		renderThread.join();
		std::cout << "Joined all threads!" << '\n';
		dynamicSimulator.writeCSV();
	}

	// unthreaded
	else
	{
		// flags to track key input
		bool KeyRight = 0, KeyLeft = 0, KeyUp = 0;
		Uint32 timeout = SDL_GetTicks() + 1000 / FPS;

		// pass initial values to working variables
		std::vector<double> xk = initialState;
		std::array<double, 2> uk = initialInput;

		//main gameloop
		while (!quit)
		{
			graphics.updateGraphics(xk.at(0), xk.at(1), xk.at(2), xk.at(5), xk.at(6));
			xk = dynamicSimulator.simToNextFrame(uk);

			// wait for 1/FPS seconds to pass and meanwhile handle key events
			while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeout)){
				// Handle all input events
				while (SDL_PollEvent(&e))
				{
					switch (e.type)
					{
					// handle registered keypresses
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
						default:
							break;
						}
					break;
					//handle keyreleases
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
			else uk[0] = 0;
			if (KeyLeft && !KeyRight) uk[1] = tiltRate;
			else if (!KeyLeft && KeyRight) uk[1] = -tiltRate;
			else uk[1] = 0;
			}//exit time-to-next-render loop 

			timeout += 1000 / FPS;
		}
	dynamicSimulator.writeCSV();
	}

	return 0;
}

