#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <cmath>
#include <string>

class Simulator
{
public:
	Simulator()
	{};
	~Simulator()
	{};

protected:
	std::vector<double> eulerForward(std::vector<double> xk, std::array<double, 2> u)
	{
		std::vector<double> xk1;
		std::vector<double> xdot = dynamics(xk, u);

		std::vector<double>::iterator xkit = std::begin(xk);
		for (std::vector<double>::iterator it = std::begin(xdot); it != std::end(xdot); ++it)
		{
			xk1.push_back(*xkit + (*it)* timeStep);
			xkit++;
		}
		return xk1;
	}

	void csvWrite(std::string fileName, std::vector<std::array<double, 2>> u, std::vector<double> t, std::vector<std::vector<double>> x)
	{
		std::ofstream output;
		output.open(fileName + ".csv");

		output << "time,input thrust,input tilt,x_drone,y_drone,theta,xdot_drone,ydot_drone";
		if ((x.front()).size() == 9)
		{
			output << "x_cargo,y_cargo,xdot_cargo,ydot_cargo";
		}
		output << '\n';

		for (int i = 0; i < t.size(); i++)
		{
			output << t[i] << ',';
			output << u[i][0] << ',';
			output << u[i][1];
			for (int j=0; j<x[1].size(); j++)
			{
				output << ',' << x[i][j];
			}
			output << '\n';
		}
		output.close();
	}


	//system variables
	float timeStep{ 0.01 };

private:
	std::vector<double> dynamics(std::vector<double> x, std::array<double, 2> u)
	{
		std::vector<double>  xdot;

		if (x.size() == 5)
		{
			xdot[1] = x[4];
			xdot[2] = x[5];
			xdot[3] = u[2];
			xdot[4] = (-u[1] * cos(x[3]) - CdragDrone * sqrt(pow(x[4], 2) + pow(x[5], 2)) * x[4]) / massDrone;
			xdot[5] = (u[1] * cos(x[3]) - CdragDrone * sqrt(pow(x[4], 2) + pow(x[5], 2)) * x[5]) / massDrone - gravitation;
		}
		else if (x.size() == 9)
		{
			xdot[1] = x[4];
			xdot[2] = x[5];
			xdot[3] = u[2];
			xdot[4] = (-u[1] * cos(x[3]) - CdragDrone * sqrt(pow(x[4], 2) + pow(x[5], 2)) * x[4]) / massDrone;
			xdot[5] = (u[1] * cos(x[3]) - CdragDrone * sqrt(pow(x[4], 2) + pow(x[5], 2)) * x[5]) / massDrone - gravitation;
			xdot[6] = x[8];
			xdot[7] = x[9];
			xdot[8] = (-CdragCargo * sqrt(pow(x[8], 2) + pow(x[9], 2)) * x[8]) / massCargo;
			xdot[9] = (-CdragCargo * sqrt(pow(x[8], 2) + pow(x[9], 2)) * x[9]) / massCargo - gravitation;
		}
		else
		{
			std::cout << "Error [Simulator.Dynamics()]: Input dimension defined for 5 and 9, dynamics for given dimension: " << x.size() << " are undefined\n";
		}

		return xdot;
	}

	//const attributes of the system
	float const massDrone{ 3 };
	float const massCargo{ 2 };
	float const CdragDrone{ 0.1 };
	float const CdragCargo{ 0.1 };
	float const lengthRope{ 1.5 };
	float const stiffnessRope{ 40000 };
	float const dampingRope{ 50 };
	float const gravitation{ 9.81 };
};//end class definition