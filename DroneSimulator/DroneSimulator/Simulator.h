#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <fstream>

// to do
// optimization - pass variables by reference?


class Simulator
{
public:
	Simulator() : timeStep(0) {};
	~Simulator() {};

protected:
	// system variables
	double timeStep;

	// one step of runge kutta integration
	std::vector<double> eulerForward(std::vector<double> xk, std::array<double, 2> u)
	{
		// xk is x at k, 
		// xk1 is x at k+1
		// xdot is dx/dt at k
		std::vector<double> xk1;
		std::vector<double> xdot = dynamics(xk, u);

		// apply Euler-Forward
		// x(k+1) = x(k) + xdot(k)*h
		std::vector<double>::iterator xkit = std::begin(xk);
		for (std::vector<double>::iterator it = std::begin(xdot); it != std::end(xdot); ++it)
		{
			xk1.push_back(*xkit + (*it) * timeStep);
			xkit++;
		}
		return xk1;
	}

	// one step of runge kutta integration
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
			xk1.push_back((*it1) + (*it2) * 2 + (*it3) * 2 + (*it4));
			it2++; it3++; it4++;
		}
		return xk1;
	}

	// function to write [t, u1, u2, x1, x2, x3, x4, x5] to filename.csv in csv format
	void WriteCSVHelper(std::string fileName, std::vector<std::array<double, 2>> u, std::vector<double> t, std::vector<std::vector<double>> x)
	{
		char delimiter = ',';
		std::ofstream output(fileName);

		// write column headers
		output << "time" << delimiter << "thrust" << delimiter << "tilt" << delimiter << "x_drone" << delimiter << "y_drone"<< delimiter << "theta" << delimiter << "xdot_drone" << delimiter << "ydot_drone";
		if ((x.front()).size() == 9)
		{
			output << "x_cargo" << delimiter << "y_cargo" << delimiter << "xdot_cargo" << delimiter << "ydot_cargo";
		}
		output << '\n';

		for (size_t i = 0; i < t.size(); i++)
		{
			// write [t, u, ...
			output << t.at(i) << delimiter << u.at(i).at(0) << delimiter << u.at(i).at(1);

			// write ... x]
			for (std::vector<double>::iterator it = std::begin(x.at(i)); it < std::end(x.at(i)); it++)
			{
				output << *it << delimiter;
			}
			output << '\n';
		}
		output.close();
	}

private:
	// dynamics determines xdot from state{x} and input{u}
	std::vector<double> dynamics(std::vector<double> x, std::array<double, 2> u)
	{
		std::vector<double>  xdot;

		// dynamics for drone without cargo
		if (x.size() == 5)
		{
			xdot.push_back(x[3]);
			xdot.push_back(x[4]);
			xdot.push_back(u[1]);
			xdot.push_back((-u[0] * cos(x[2]) - CdragDrone * sqrt(pow(x[3], 2) + pow(x[4], 2)) * x[3]) / massDrone);
			xdot.push_back(( u[0] * cos(x[2]) - CdragDrone * sqrt(pow(x[3], 2) + pow(x[4], 2)) * x[4]) / massDrone - gravitation);
		}

		// dynamics for drone with cargo
		else if (x.size() == 9)
		{
			std::array<double, 2> y; // ropeLength and ropeLengthDot
			y[0] = sqrt(pow(x[0] - x[5], 2) + pow(x[1] - x[6], 2));
			y[1] = ((x[0] - x[5])*(x[3] - x[7]) + (x[1] - x[6])*(x[4] - x[8])) / y[0];	

			double Frope = stiffnessRope*(y[0]- lengthRope0) + dampingRope* y[1];
			double Fropex;
			double Fropey;

			// only apply force if there is tension in the rope
			if (Frope > 0)
			{
				Fropex = Frope * (x[0] - x[5]) / y[0];
				Fropey = Frope * (x[1] - x[6]) / y[0];
			}
			else
			{
				Fropex = 0;
				Fropey = 0;
			}

			// dynamics for drone
			xdot.push_back(x[3]);
			xdot.push_back(x[4]);
			xdot.push_back(u[1]);
			xdot.push_back((-u[0] * sin(x[2])	- CdragDrone * sqrt(pow(x[3], 2) + pow(x[4], 2)) * x[3] - Fropex) / massDrone);
			xdot.push_back(( u[0] * cos(x[2])	- CdragDrone * sqrt(pow(x[3], 2) + pow(x[4], 2)) * x[4] - Fropey) / massDrone - gravitation);
			// dynamics for cargo
			xdot.push_back(x[7]);
			xdot.push_back(x[8]);
			xdot.push_back((					- CdragCargo * sqrt(pow(x[7], 2) + pow(x[8], 2)) * x[7] + Fropex) / massCargo);
			xdot.push_back((					- CdragCargo * sqrt(pow(x[7], 2) + pow(x[8], 2)) * x[8] + Fropey) / massCargo - gravitation);
		}

		return xdot;
	}

	// helper function for product of scalar{p} and vector{x}
	std::vector<double> scalarVectorProd(double p, std::vector<double> x)
	{
		std::vector<double> y;
		for (std::vector<double>::iterator it = std::begin(x); it < std::end(x); it++)
		{
			y.push_back((*it) * p);
		}
		return y;
	}

	// helper function for sums of 2 vectors{x1},{x2}
	std::vector<double> vectorSum(std::vector<double> x1, std::vector<double> x2)
	{
		std::vector<double> y;
		std::vector<double>::iterator it2 = std::begin(x2);
		for (std::vector<double>::iterator it = std::begin(x1); it < std::end(x1); it++)
		{
			y.push_back((*it) + (*it2));
			it2++;
		}
		return y;
	}

	//const attributes of the system
	static const double massDrone;
	static const double massCargo;
	static const double CdragDrone;
	static const double CdragCargo;
	static const double lengthRope0;
	static const double stiffnessRope;
	static const double dampingRope;
	static const double gravitation;
};//end class definition

const double Simulator::massDrone = 3;
const double Simulator::massCargo = 2;
const double Simulator::CdragDrone = 0.1;
const double Simulator::CdragCargo = 0.1;
const double Simulator::lengthRope0 = 1.5;
const double Simulator::stiffnessRope = 40000;
const double Simulator::dampingRope = 50;
const double Simulator::gravitation = 9.81;


