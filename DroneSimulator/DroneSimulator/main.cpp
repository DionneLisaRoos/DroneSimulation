#include <iostream>
#include "SDL.h"
#include "Graphics.h"
#include "SimStatic.h"
#include "SimDynamic.h"
#include "InputStatic.h"
#include "GUI.h"
#include <thread>
#include <chrono>
#include <mutex>


std::mutex mtx;

void simOnThread(SimDynamic& simulator, bool& quit, int simFrequency)
{
	using namespace std::chrono_literals;
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
void renderOnThread(SimDynamic& simulator, Graphics& graphics, bool& quit, bool cargo, int FPS)
{
	std::vector<double> xk;
	Uint32 timeoutRender = SDL_GetTicks() + 1000 / FPS;
	while (!quit)
	{
		while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeoutRender))
		{
			// fetch current state, and render updated graphics
			xk = simulator.getState();
			graphics.updateGraphics(xk);
		}
		timeoutRender += 1000 / FPS;
	}
}

int main(int argc, char* args[])
{
	GUI gui;
	gui.startGUI();

	bool cargo = gui.getCargoChoice();
	bool keyboard = gui.getKeyboardChoice();
	
	if (!gui.isGuiFinished()) return 0;

	int FPS = 60;
	int SimPS = 100;
	double timeStep = 1.0 / SimPS;

	if (!keyboard) {
		if (cargo) {
			std::vector<double> initialState = { 0, 1, 0, 0, 0,0,0,0,0 };

			InputStatic input("inputDroneAndCargo.csv", timeStep);
			SimStatic staticSimulation(initialState, input, timeStep);
			staticSimulation.staticSimulateFullRungeKutta();
			staticSimulation.WriteCSV("outputEulerDroneAndCargo.csv");
		}
		else {
			std::vector<double> initialState = { 0, 0, 0, 0, 0 };

			InputStatic input("inputDrone.csv", timeStep);
			SimStatic staticSimulation(initialState, input, timeStep);
			staticSimulation.staticSimulateFullEuler();
			staticSimulation.WriteCSV("outputEulerDrone.csv");
		}
	}
	else {
		// global exit flag
		bool quit = 0;

		// flag for tracking keypresses, input values, increase for more drone yeet
		bool KeyRight = 0, KeyLeft = 0, KeyUp = 0, KeyDown = 0;
		double maxThrust = 4 * 5 * 9.81;
		double tiltRate = 60;

		// initial position of the drone
		std::array<double, 2> initialInput = { 5 * 9.81, 0 };
		std::vector<double> initialState;
		std::string filename = "";

		if (cargo) {
			initialState = { 0, 1, 0, 0, 0, 0, 0, 0, 0 };	
			filename = "DynamicSimDroneAndCargo.csv";
		}
		else {
			initialState = { 0, 0, 0, 0, 0 };	
			filename = "DynamicSimDrone.csv";
		}

		// initialize graphics, simulator, timeout duration and event handler
		Graphics graphics;
		SimDynamic dynamicSimulator(initialState, initialInput, timeStep, FPS);

		// to thread or not to thread, that is the question
		bool thread = 1;

		// event obj
		SDL_Event e;

		// gameloop threaded
		std::thread simThread(simOnThread, std::ref(dynamicSimulator), std::ref(quit), SimPS);
		std::thread renderThread(renderOnThread, std::ref(dynamicSimulator), std::ref(graphics), std::ref(quit), cargo, FPS);

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
			}// end processing event

		}

		simThread.join();
		renderThread.join();
		dynamicSimulator.writeCSV(filename);
	}

	return 0;
}