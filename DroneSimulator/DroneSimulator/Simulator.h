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

		xk1 = vectorSum(xk, scalarVectorProd(timeStep, xdot));

		return xk1;
	}

	std::vector<double> rungeKutta(std::vector<double> xk, std::array<double, 2> u)
	{
		std::vector<double> xk1;

		std::vector<double> K1 = dynamics(xk, u);
		std::vector<double> K2 = dynamics(vectorSum(xk, scalarVectorProd(0.5 * timeStep, K1)), u);
		std::vector<double> K3 = dynamics(vectorSum(xk, scalarVectorProd(0.5 * timeStep, K2)), u);
		std::vector<double> K4 = dynamics(vectorSum(xk, scalarVectorProd(timeStep, K3)), u);

		std::vector<double>::iterator it2 = std::begin(K2);
		std::vector<double>::iterator it3 = std::begin(K3);
		std::vector<double>::iterator it4 = std::begin(K4);

		for (std::vector<double>::iterator it1 = std::begin(K1); it1 != std::end(K1); ++it1)
		{
			xk1.push_back( *it1 + *it2*2 + *it3*2 + *it4 );
			it2++; it3++; it4++;
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

	std::vector<double> scalarVectorProd(double p, std::vector<double> x)
	{
		std::vector<double> y;
		for (std::vector<double>::iterator it = std::begin(x); it < std::end(x); it++)
		{
			y.push_back(*it * p);
		}
		return y;
	}

	std::vector<double> vectorSum(std::vector<double> x1, std::vector<double> x2)
	{
		std::vector<double> y;
		std::vector<double>::iterator it2 = std::begin(x2);
		for (std::vector<double>::iterator it = std::begin(x1); it < std::end(x1); it++)
		{
			y.push_back(*it + (*it2));
			it2++;
		}
		return y;
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