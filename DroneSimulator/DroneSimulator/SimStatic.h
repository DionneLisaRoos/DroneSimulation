#pragma once
#include "Simulator.h"
#include "InputStatic.h"
#include "Graphics.h"

class SimStatic :
	public Simulator
{
public:

	SimStatic(std::vector<double> x0, InputStatic input, double timestep, bool cargo) : graphics(cargo)
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
		for (size_t i = 1; i < t.size(); i++)
		{
			x.push_back(eulerForward(x.at(i - 1), u.at(i - 1)));
			graphics.updateGraphics(x.at(i).at(0), x.at(i).at(1), x.at(i).at(2));
			std::cout << t.at(i) << "\n";
		}
	};

	void staticSimulateFullRungeKutta()
	{
		for (size_t i = 1; i < t.size(); i++)
		{
			x.push_back(rungeKutta(x.at(i - 1), u.at(i - 1)));
			graphics.updateGraphics(x.at(i).at(0), x.at(i).at(1), x.at(i).at(2));
			std::cout << t.at(i) << "\n";
		}
	};

	void WriteCSV(std::string fileName) {
		WriteCSVHelper(fileName, u, t, x);
	}

private:
	//storage variables
	std::vector<double> t;
	std::vector<std::vector<double>> x;//-------------------------------------
	std::vector<std::array<double, 2>> u;
	Graphics graphics;
};