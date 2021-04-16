//==============================================================
// Filename : SimStatic.h
// Authors : Dionne Ariëns and Marnick Los.
// Version : -
// License : -
// Description : 
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
		std::array<double, 2> y0;

		x.push_back(x0);
		timeStep = timestep;
		
		u = input.getInput();
		t = input.getTime();

		y0[0] = sqrt(pow(x0[0] - x0[5], 2) + pow(x0[1] - x0[6], 2));
		if (y0[0] == 0)	y0[1] = 0;
		else y0[1] = ((x0[0] - x0[5]) * (x0[3] - x0[7]) + (x0[1] - x0[6]) * (x0[4] - x0[8])) / y0[0];

		y.push_back(y0);
	}

	~SimStatic() {}

	// simulate full time range of input
	void staticSimulateFullEuler()
	{
		x.clear();
		x.push_back(x0);
		// 2 options, make if else for cargo to update graphics
		// or pass full xk to graphics and discriminate between cargo or no in the grapgics class
		for (size_t i = 1; i < t.size(); i++)
		{
			x.push_back(eulerForward(x.at(i - 1), u.at(i - 1)));
			graphics.updateGraphics(x.at(i));
		}
	};

	void staticSimulateFullRungeKutta()
	{
		x.clear();
		x.push_back(x0);
		for (size_t i = 1; i < t.size(); i++)
		{
			x.push_back(rungeKutta(x.at(i - 1), u.at(i - 1)));
			graphics.updateGraphics(x.at(i));
		}
	};

	void WriteCSV(std::string fileName) {
		WriteCSVHelper(fileName, u, t, x);
	}

private:
	//storage variables
	std::vector<double> x0;
	Graphics graphics;
};