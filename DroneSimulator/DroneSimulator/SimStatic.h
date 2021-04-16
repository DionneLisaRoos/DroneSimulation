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
		x.push_back(x0);
		timeStep = timestep;
		
		u = input.getInput();
		t = input.getTime();
	};
	~SimStatic() {};

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
			std::cout << t.at(i) << "\n";
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
			std::cout << t.at(i) << "\n";
		}
	};

	void WriteCSV(std::string fileName) {
		WriteCSVHelper(fileName, u, t, x);
	}

private:
	//storage variables
	std::vector<std::vector<double>> x;
	std::vector<double> t;
	std::vector<std::array<double, 2>> u;
	std::vector<double> x0;
	Graphics graphics;
};