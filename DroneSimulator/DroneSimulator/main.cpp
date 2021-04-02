#include <iostream>
#include "Graphics.h"
#include "SimStatic.h"
#include "InputStatic.h"

int main(int argc, char* args[])
{
	double timeStep = 0.05;
	InputStatic input("input.csv", timeStep);
	std::vector<double> initialState = {0, 0, 0, 0, 0};

	SimStatic staticSimulation(initialState, input, timeStep, false);
	staticSimulation.staticSimulateFullEuler();
	staticSimulation.WriteCSV("outputEuler.csv");

	std::cout << "next\n";

	staticSimulation.staticSimulateFullRungeKutta();
	staticSimulation.WriteCSV("outputKutta.csv");

	return 0;
}
