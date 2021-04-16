#include <iostream>
#include "SDL.h"
#include "Graphics.h"
#include "SimStatic.h"
#include "SimDynamic.h"
#include "InputStatic.h"
#include <thread>
#include <mutex>


std::mutex mtx;

void simOnThread(SimDynamic& simulator, bool& quit, int simFrequency)
{
	std::cout << "entered sim thread" << '\n';
	Uint32 timeoutSim = SDL_GetTicks() + 1000 / simFrequency;
	while (!quit)//access global quit?
	{
		while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeoutSim)) 
		{
			//std::cout << "sim" << '\n';
			mtx.lock();
			simulator.updateStates();
			mtx.unlock();
		}
		timeoutSim += 1000 / simFrequency;
	}
}
void renderOnThread(SimDynamic& simulator, Graphics& graphics, bool& quit, int FPS)
{
	std::cout << "entering render thread" << '\n';
	std::vector<double> xk;
	Uint32 timeoutRender = SDL_GetTicks() + 1000 / FPS;
	while (!quit)//access global quit?
	{
		while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeoutRender))
		{
			//std::cout << "render" << '\n';
			xk = simulator.getState();
			graphics.updateGraphics(xk.at(0), xk.at(1), xk.at(2), xk.at(5), xk.at(6));
		}
		timeoutRender += 1000 / FPS;
	}

}
void handleInputOnTread(SimDynamic& simulator, bool& quit, int InputFrequency)
{ 
	SDL_Event e;
	Uint32 timeoutInput = SDL_GetTicks() + 1000 / InputFrequency;
	std::cout << "entering input handler" << '\n';
	double maxThrust = 4 * 5 * 9.81;
	double tiltRate = 30;
	while (!quit)
	{
		while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeoutInput))
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
						mtx.lock();
						simulator.setInput(0, maxThrust);
						mtx.unlock();
						break;
					case SDLK_LEFT:
						mtx.lock();
						simulator.setInput(1, tiltRate);
						mtx.unlock();
						break;
					case SDLK_RIGHT:
						mtx.lock();
						simulator.setInput(1, -tiltRate);
						mtx.unlock();
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
						mtx.lock();
						simulator.setInput(0, 0);
						mtx.unlock();
						break;
					case SDLK_LEFT:
						mtx.lock();
						simulator.setInput(1, 0);
						mtx.unlock();
						break;
					case SDLK_RIGHT:
						mtx.lock();
						simulator.setInput(1, 0);
						mtx.unlock();
						break;
					default:
						break;
					}
					break;
				default:
					break;
				}
			std::cout << "processed an event\n";
			}// end processing event
		}
		std::cout << "InputHandler Loop complete\n";
		timeoutInput += 1000 / InputFrequency;
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

	int FPS = 30;
	int SimPS = 100;
	int InputPS = 100;
	double timeStep = 1.0/SimPS;
	//double timeStep = 0.005;
	bool quit = 0 ;
	double maxThrust = 4 * 5 * 9.81;
	double tiltRate = 30;
	std::vector<double> initialState = { 0, 1, 0, 0, 0, 0, 1, 0, 0 };
	std::array<double, 2> initialInput = {5*9.81, 0};
	
	// flags to track key input
	bool KeyRight=0, KeyLeft=0, KeyUp=0;

	// initialize graphics, simulator, timeout duration and event handler
	Graphics graphics(1);
	SimDynamic dynamicSimulator(initialState, initialInput, timeStep, FPS);

	// pass initial values to working variables
	std::vector<double> xk = initialState;
	std::array<double,2> uk = initialInput;
	
	bool thread=1;

	// gameloop threaded
	if (thread)
	{
		std::cout << "entering thread-if" << '\n';
		std::thread sim(simOnThread, std::ref(dynamicSimulator), std::ref(quit), SimPS);
		std::thread render(renderOnThread, std::ref(dynamicSimulator), std::ref(graphics), std::ref(quit), FPS);
		std::thread inputHandler(handleInputOnTread, std::ref(dynamicSimulator), std::ref(quit), InputPS);
		std::cout << "created threads?" << '\n';

		sim.join();
		render.join();
		inputHandler.join();
		std::cout << "Joined all threads!" << '\n';
		dynamicSimulator.writeCSV();
	}

	// unthreaded
	else
	{
		Uint32 timeout = SDL_GetTicks() + 1000 / FPS;
		SDL_Event e;
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

