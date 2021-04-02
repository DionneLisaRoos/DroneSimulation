#pragma once
#include "Simulator.h"

class SimStatic : 
	public Simulator
{
public:
	SimStatic(std::vector<double> x0)
	{
		x.push_back(x0);

		inputStatic input;
		input.setTimeStep(timeStep);			// zoiets? of is het handiger om timeStep in de constructor van inputStatic mee te geven?
		input.interpolate(inputdatafile.csv);	// hier de filenaam/format nog aanpassen

		std::vector<std::array<double, 2>> u = input.getInput();
		std::vector<double> t = input.getTime();
	};
	~SimStatic()
	{};

	// simulate full time range of input
	void staticSimulateFullEuler()
	{
		for (int i = 1; i < t.size(); i++)
		{
			x[i] = eulerForward(x.at(i - 1), u.at(i - 1));
		}
	};

private:
	//storage variables
	std::vector<double> t;
	std::vector<std::vector<double>> x;//-------------------------------------
	std::vector<std::array<double, 2>> u;

};

