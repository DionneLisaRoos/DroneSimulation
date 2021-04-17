//==============================================================
// Filename : SimDynamic.h
// Authors : Dionne Ariëns and Marnick Los.
// Version : -
// License : -
// Description : simulator object used for real-time simulations with user inputs, 
//				 has functionality to work single- and multithreaded, currently only multithreaded is implemented.
//==============================================================

#pragma once
#include "Simulator.h"

class SimDynamic : public Simulator
{
public:
	SimDynamic(std::vector<double> x0, std::array<double, 2> u0, double timestep, int FPS)
	{
		if (x0.size() == 9) {
			std::array<double, 2> y0;
			y0[0] = sqrt(pow(x0[0] - x0[5], 2) + pow(x0[1] - x0[6], 2));
			if (y0[0] == 0)	y0[1] = 0;
			else y0[1] = ((x0[0] - x0[5]) * (x0[3] - x0[7]) + (x0[1] - x0[6]) * (x0[4] - x0[8])) / y0[0];

			y.push_back(y0);
		}

		// push_back all initial values
		t.push_back(0);
		u.push_back(u0);
		x.push_back(x0);

		// store initial values in working variables
		xk = x0;
		uk = u0;

		// simualtion timing parameters
		timeStep = timestep;
		draw_time = 1.0/FPS;
	}

	~SimDynamic()
	{}

	// funcion to simulate system until next frame in non-multithreaded configuration
	std::vector<double> simToNextFrame(std::array<double,2> uk)
	{
		// xk1 is the state of the next drawn frame
		std::vector<double> xk1;

		// timeTarget is the time of the next render
		double timeTarget = t.back() + draw_time;

		// simulate the drone until next frame can be drawn and store intermediate variables
		while (t.back() < timeTarget)
		{
			xk1 = rungeKutta(x.back(), uk);
			x.push_back(xk1);
			t.push_back(t.back() + timeStep);
			u.push_back(uk);
		}

		return xk1;
	}

	// updates state using current input uk and pushes to storage variable
	void updateStates(void)
	{
		xk = rungeKutta(x.back(), uk);
		x.push_back(xk);
		t.push_back(t.back() + timeStep);
		u.push_back(uk);
	}

	// set the current input of thrust and tilt rate
	void setInput(int i, double uin)
	{
		this->uk[i] = uin;
	}

	// get the current state
	std::vector<double> getState(void)
	{
		return xk;
	}


	// write output to fileName.CSV
	void writeCSV(std::string fileName)
	{
		WriteCSVHelper(fileName, u, t, x);
	}


private:
	// working variables
	std::vector<double> xk;
	std::array<double,2> uk;

	// simulator parameters
	double draw_time;
};