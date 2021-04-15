#pragma once
#include "Simulator.h"
#include "InputKeyboard.h"
#include "Graphics.h"

class SimDynamic : public Simulator
{
public:
	SimDynamic(std::vector<double> x0, std::array<double, 2> u0, double timestep, int FPS)
	{
		x.push_back(x0);
		t.push_back(0);
		u.push_back(u0);
		timeStep = timestep;
		draw_time = static_cast<double>(1/FPS);
	};

	~SimDynamic()
	{};

	std::vector<double> simToNextFrame(std::array<double,2> uk)
	{
		// xk1 is the states of the next drawn frame
		std::vector<double> xk1;

		// timeTarget is the time of the next render
		double timeTarget = t.back() + draw_time;

		// simulate the drone until next frame can be drawn
		while (t.back() < timeTarget)
		{
			xk1 = rungeKutta(x.back(), uk);
			x.push_back(xk1);
			t.push_back(t.back() + timeStep);
			u.push_back(uk);
		}

		return xk1;
	}




private:
	// storage variables
	std::vector<double> t;
	std::vector<std::vector<double>> x;
	std::vector<std::array<double, 2>> u;


	// simulator parameters
	double timeStep;
	double draw_time;
};