#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <fstream>


class Simulator
{
public:
	Simulator() : timeStep(0) {};
	~Simulator() {};

protected:
	//system variables
	double timeStep;

	std::vector<double> eulerForward(std::vector<double> xk, std::array<double, 2> u)
	{
		std::vector<double> xk1;
		std::vector<double> xdot = dynamics(xk, u);

		std::vector<double>::iterator xkit = std::begin(xk);
		for (std::vector<double>::iterator it = std::begin(xdot); it != std::end(xdot); ++it)
		{
			xk1.push_back(*xkit + (*it) * timeStep);
			xkit++;
		}
		return xk1;
	}

	void WriteCSVHelper(std::string fileName, std::vector<std::array<double, 2>> u, std::vector<double> t, std::vector<std::vector<double>> x)
	{
		char delimiter = ';';

		std::ofstream output(fileName);
		//output.open(fileName + ".csv");

		output << "time" << delimiter << "input thrust" << delimiter << "input tilt" << delimiter << "x_drone" << delimiter << "y_drone"<< delimiter << "theta" << delimiter << "xdot_drone" << delimiter << "ydot_drone";
		if ((x.front()).size() == 9)
		{
			output << "x_cargo" << delimiter << "y_cargo" << delimiter << "xdot_cargo" << delimiter << "ydot_cargo";
		}
		output << '\n';

		for (int i = 0; i < t.size(); i++)
		{
			output << t[i] << delimiter << u[i][0] << delimiter << u[i][1];

			for (int j = 0; j < x[1].size(); j++)
			{
				output << delimiter << x[i][j];
			}
			output << '\n';
		}
		output.close();
	}

private:
	std::vector<double> dynamics(std::vector<double> x, std::array<double, 2> u)
	{
		std::vector<double>  xdot;

		//if (x.size() == 5)
		//{
			xdot.push_back(x[3]);
			xdot.push_back(x[4]);
			xdot.push_back(u[1]);
			xdot.push_back((-u[0] * cos(x[2]) - CdragDrone * sqrt(pow(x[3], 2) + pow(x[4], 2)) * x[3]) / massDrone);
			xdot.push_back((u[0] * cos(x[2]) - CdragDrone * sqrt(pow(x[3], 2) + pow(x[4], 2)) * x[4]) / massDrone - gravitation);
		//}
		if (x.size() == 9)
		{
			//xdot[1] = x[4];
			//xdot[2] = x[5];
			//xdot[3] = u[2];
			//xdot[4] = (-u[1] * cos(x[3]) - CdragDrone * sqrt(pow(x[4], 2) + pow(x[5], 2)) * x[4]) / massDrone;
			//xdot[5] = (u[1] * cos(x[3]) - CdragDrone * sqrt(pow(x[4], 2) + pow(x[5], 2)) * x[5]) / massDrone - gravitation;
			xdot.push_back(x[7]);
			xdot.push_back(x[8]);
			xdot.push_back((-CdragCargo * sqrt(pow(x[7], 2) + pow(x[8], 2)) * x[7]) / massCargo);
			xdot.push_back((-CdragCargo * sqrt(pow(x[7], 2) + pow(x[8], 2)) * x[8]) / massCargo - gravitation);
		}
		//else
		//{
		//	std::cout << "Error [Simulator.Dynamics()]: Input dimension defined for 5 and 9, dynamics for given dimension: " << x.size() << " are undefined\n";
		//}

		return xdot;
	}

	//const attributes of the system
	static const float massDrone;
	static const float massCargo;
	static const float CdragDrone;
	static const float CdragCargo;
	static const float lengthRope;
	static const float stiffnessRope;
	static const float dampingRope;
	static const float gravitation;
};//end class definition

const float Simulator::massDrone = 3;
const float Simulator::massCargo = 2;
const float Simulator::CdragDrone = 0.1;
const float Simulator::CdragCargo = 0.1;
const float Simulator::lengthRope = 1.5;
const float Simulator::stiffnessRope = 40000;
const float Simulator::dampingRope = 50;
const float Simulator::gravitation = 9.81;


