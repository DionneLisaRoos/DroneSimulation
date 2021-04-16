#include <iostream>
#include "Graphics.h"
#include "SimStatic.h"
#include "InputStatic.h"
#include "GUI.h"

const int FPS = 20; // Frame rate in frames per second
Uint32 timeout_ms = SDL_GetTicks() + 1000 / FPS; // Initialize so
// that after drawing the first frame, it waits for 1000/FPS ms
SDL_Event e;

int main(int argc, char* args[])
{
	GUI gui; 
	gui.startGUI();

	bool cargo = gui.getCargoChoice();
	bool keyboard = gui.getKeyboardChoice();

	double timeStep = 0.01;

	if (cargo) {
		std::vector<double> initialState = { 0, 1, 0, 0, 0,0,0,0,0 };

		if (keyboard) {
			std::cout << "Using cargo: " << cargo << " and using keyboard: " << keyboard << std::endl;
		}
		else {
			InputStatic input("input.csv", timeStep);
			SimStatic staticSimulation(initialState, input, timeStep, cargo);
			staticSimulation.staticSimulateFullRungeKutta();
			staticSimulation.WriteCSV("outputEuler.csv");
		}
	}
	else {
		std::vector<double> initialState = { 0, 0, 0, 0, 0 };

		if (keyboard) {
			std::cout << "Using cargo: " << cargo << " and using keyboard: " << keyboard << std::endl;
		}
		else {
			InputStatic input("input.csv", timeStep);
			SimStatic staticSimulation(initialState, input, timeStep, cargo);
			staticSimulation.staticSimulateFullEuler();
			staticSimulation.WriteCSV("outputEuler.csv");
		}
	}
	return 0;
}