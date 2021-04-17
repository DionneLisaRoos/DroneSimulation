//==============================================================
// Filename : SimStatic.h
// Authors : Dionne Ariëns and Marnick Los.
// Version : -
// License : -
// Description : simulator object used for static simulations with predefined inputs, uses InputStatic to read input from CSV
//				 numerically integrates system for the defined time duration using Euler forward or Runge Kutta 4
//==============================================================

#pragma once
#include "Simulator.h"
#include "InputStatic.h"
#include "Graphics.h"

class SimStatic :
	public Simulator
{
public:

	SimStatic(std::vector<double> X0, InputStatic input, double timestep) : x0(X0)
	{
		if (x0.size() == 9) {
			std::array<double, 2> y0;
			y0[0] = sqrt(pow(x0[0] - x0[5], 2) + pow(x0[1] - x0[6], 2));
			if (y0[0] == 0)	y0[1] = 0;
			else y0[1] = ((x0[0] - x0[5]) * (x0[3] - x0[7]) + (x0[1] - x0[6]) * (x0[4] - x0[8])) / y0[0];

			y.push_back(y0);
		}

		timeStep = timestep;

		u = input.getInput();
		t = input.getTime();
	}

	~SimStatic() 
	{
		graphics.close();
	}

	// simulate full time duraion using Euler forward
	void staticSimulateFullEuler()
	{
		// clears x to prevent multiple simulations to be written subsequently in same file
		x.clear();
		x.push_back(x0);

		// loops over full time, stores simulation results to x and visualises simulation
		for (size_t i = 1; i < t.size(); i++)
		{
			x.push_back(eulerForward(x.at(i - 1), u.at(i - 1)));
			graphics.updateGraphics(x.at(i));
		}
	}

	// simulate full time duraion using Runge Kutta 4
	void staticSimulateFullRungeKutta()
	{
		// clears x to prevent multiple simulations to be written subsequently in same file
		x.clear();
		x.push_back(x0);

		// loops over full time, stores simulation results to x and visualises simulation
		for (size_t i = 1; i < t.size(); i++)
		{
			x.push_back(rungeKutta(x.at(i - 1), u.at(i - 1)));
			graphics.updateGraphics(x.at(i));
		}
	};

	// write output to fileName.CSV
	void WriteCSV(std::string fileName) {
		WriteCSVHelper(fileName, u, t, x);
	}

private:
	// store x0 and graphics in class
	std::vector<double> x0;
	Graphics graphics;
};