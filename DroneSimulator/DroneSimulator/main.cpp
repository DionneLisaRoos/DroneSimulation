//==============================================================
// Filename : main.cpp
// Authors : Dionne Ariëns and Marnick Los.
// Version : -
// License : -
// Description : This is the main function of the game. In this file a GUI 
//				 is created where we first get the settings of the game, set by the player.
//				 Then we start the game accourding to the given settings. Either a drone simulator with 
//				 or withour cargo is started and this can be done using file input or keyboard input. 
//				 When keyboard input is used, we will use a multithreaded program. When an input file is used, 
//				 the program excecutes single threaded. 
//==============================================================

#include "Graphics.h"
#include "SimStatic.h"
#include "SimDynamic.h"
#include "InputStatic.h"
#include "GUI.h"
#include <thread>
#include <chrono>
#include <mutex>

// mutex used when writing or reading memory that is accesible by multiple threads
std::mutex mtx;

// simulator thread that calculates the dyanmics using a certain frequency (100 Hz in this case)
void simOnThread(SimDynamic& simulator, bool& quit, int simFrequency)
{
	using namespace std::chrono_literals;
	std::chrono::duration<double, std::milli> sleepTime = 1000ms/simFrequency;

	// As long the program did not quit, the dynamics of the system are calculated with simFrequency Hz
	// uses chrono to subtract simulation time from sleeptime
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

// Graphics thread
void renderOnThread(SimDynamic& simulator, Graphics& graphics, bool& quit, bool cargo, int FPS)
{
	// store the current state on thread and define time target in 1000ms/FPS from now
	std::vector<double> xk;
	Uint32 timeoutRender = SDL_GetTicks() + 1000 / FPS;

	// As long the program did not quit, the graphics is updated with the calculated dynamics at (FPS) frequency 
	while (!quit)
	{
		while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeoutRender))
		{
			// fetch current state, and render updated graphics
			xk = simulator.getState();
			graphics.updateGraphics(xk);
		}
		// set next time target 1000ms/FPS further in time
		timeoutRender += 1000 / FPS;
	}
}


int main(int argc, char* args[])
{
	// Instantiate gui
	GUI gui;
	gui.startGUI();

	// Get the game settings from the gui
	bool cargo = gui.getCargoChoice();
	bool keyboard = gui.getKeyboardChoice();
	
	// When Gui was closed by the user, the whole program should quit. 
	if (!gui.isGuiFinished()) return 0;

	// Set frame rate and frequenty
	int FPS = 30;
	int SimPS = 100;
	double timeStep = 1.0 / SimPS;

	// Check what settings were made in the GUI settings page
	if (!keyboard) {
		if (cargo) {
			// Here the program will execute using a Drone and Cargo and using file input
			// Set initial state, set the second element to 1 so the drone is starting a bit higher than the cargo.
			std::vector<double> initialState = { 0, 1, 0, 0, 0,0,0,0,0 };

			// Get data from file and create a static simulation 
			InputStatic input("inputDroneAndCargo.csv", timeStep);
			SimStatic staticSimulation(initialState, input, timeStep);

			// Since the Runge Kutta method works best when using a Drone and Cargo, this algorithm is used here.
			staticSimulation.staticSimulateFullRungeKutta();

			// Write output to file
			staticSimulation.WriteCSV("outputRungeKuttaDroneAndCargo.csv");
		}
		else {
			// Here the program will execute using a Drone and using file input
			// Set initial state to zeros.
			std::vector<double> initialState = { 0, 0, 0, 0, 0 };

			// Get data form file and create a static simulation
			InputStatic input("inputDrone.csv", timeStep);
			SimStatic staticSimulation(initialState, input, timeStep);

			// When only a drone is used, the full euler can also be used to calculate the dynamics therefore this method is used here
			staticSimulation.staticSimulateFullEuler();

			// Write output to file
			staticSimulation.WriteCSV("outputEulerDrone.csv");
		}
	}
	else {
		// global exit flag
		bool quit = 0;

		// flag for tracking keypresses, input values, increase for more drone yeet
		bool KeyRight = 0, KeyLeft = 0, KeyUp = 0, KeyDown = 0;
		double maxThrust = 0;
		double tiltRate = 17.2;

		// initial position of the drone
		std::array<double, 2> initialInput = { 5 * 9.81, 0 };
		std::vector<double> initialState;
		std::string filename = "";

		if (cargo) {
			// When the dynamic simulator is created using a Drone and cargo and is using keyboard input
			// Set initial state, ouput file name and max thrust ( 2 * mass * gravity)
			initialState = { 0, 1, 0, 0, 0, 0, 0, 0, 0 };	
			filename = "DynamicSimDroneAndCargo.csv";
			maxThrust = 2 * 5 * 9.81;
		}
		else {
			// When the dynamic simulator is created using a Drone and is using keyboard input
			// Set initial state, ouput file name and max thrust ( 2 * mass * gravity)
			initialState = { 0, 0, 0, 0, 0 };	
			filename = "DynamicSimDrone.csv";
			maxThrust = 2 * 3 * 9.81;
		}

		// initialize graphics, simulator, timeout duration and event handler
		Graphics graphics;
		SimDynamic dynamicSimulator(initialState, initialInput, timeStep, FPS);

		// event obj
		SDL_Event e;

		// gameloop threaded
		std::thread simThread(simOnThread, std::ref(dynamicSimulator), std::ref(quit), SimPS);
		std::thread renderThread(renderOnThread, std::ref(dynamicSimulator), std::ref(graphics), std::ref(quit), cargo, FPS);

		while (!quit)
		{
			// SDL_PollEvent(&e) returns 0 is no event- and 1 if an event happened
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
				// Update the input thrust and input tilt in the dynamic simulator object
				mtx.lock();
				if (!KeyDown && KeyUp) dynamicSimulator.setInput(0, maxThrust);
				else if (KeyDown && !KeyUp) dynamicSimulator.setInput(0, -maxThrust);
				else dynamicSimulator.setInput(0, 0);

				if (!KeyLeft && KeyRight) dynamicSimulator.setInput(1, -tiltRate);
				else if (KeyLeft && !KeyRight) dynamicSimulator.setInput(1, tiltRate);
				else dynamicSimulator.setInput(1, 0);
				mtx.unlock();
			}// end processing event
		}// exiting main game loop

		// rejoin all threads
		simThread.join();
		renderThread.join();
		graphics.close();
		// write output of simulation to file
		dynamicSimulator.writeCSV(filename);
	}// end main()

	return 0;
}