//==============================================================
// Filename : Simulator.h
// Authors : Dionne Ari?ns and Marnick Los.
// Version : -
// License : -
// Description :	Main simulator class, base class for SimStatic and SimDynamic. Contains system parameters, system dynamics, numerical solvers,
//					storage variables and a function that writes time,input,state and additional outputs to a CSV.
//==============================================================

#pragma once
#include <vector>
#include <array>
#include <cmath>
#include <fstream>

class Simulator
{
public:
	Simulator() {}

	~Simulator() {}

protected:
	// system time and storage variables
	double timeStep;
	std::vector<std::vector<double>> x;
	std::vector<double> t;
	std::vector<std::array<double, 2>> u;
	std::vector<std::array<double, 2>> y;

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
		// xk is x at k, 
		// xk1 is x at k+1
		// xdot is dx/dt at k
		std::vector<double> xk1;
		std::vector<double> xdot;

		// intermediate Xdot estimations
		std::vector<double> K1 = dynamics(xk, u);
		std::vector<double> K2 = dynamics(vectorSum(xk, scalarVectorProd(0.5 * timeStep, K1)), u);
		std::vector<double> K3 = dynamics(vectorSum(xk, scalarVectorProd(0.5 * timeStep, K2)), u);
		std::vector<double> K4 = dynamics(vectorSum(xk, scalarVectorProd(timeStep, K3)), u);

		// iterators to loop over all Xdot estimations
		std::vector<double>::iterator it2 = std::begin(K2);
		std::vector<double>::iterator it3 = std::begin(K3);
		std::vector<double>::iterator it4 = std::begin(K4);

		for (std::vector<double>::iterator it1 = std::begin(K1); it1 != std::end(K1); ++it1)
		{
			// determine Xdot by weight averaging the intermediate estimations
			// and multiply each element by the timestep h individually
			xdot.push_back(((*it1) + (*it2) * 2 + (*it3) * 2 + (*it4)) * timeStep/6);
			it2++; it3++; it4++;
		}
		xk1 = vectorSum(xk, xdot);
		return xk1;
	}

	// function to write [t, u, x, y] to filename.csv in csv format
	void WriteCSVHelper(std::string fileName, std::vector<std::array<double, 2>> u, std::vector<double> t, std::vector<std::vector<double>> x)
	{
		char delimiter = ',';
		std::ofstream output(fileName);

		// write column headers
		output << "time" << delimiter << "thrust" << delimiter << "tilt" << delimiter << "x_drone" << delimiter << "y_drone" << delimiter << "theta" << delimiter << "xdot_drone" << delimiter << "ydot_drone";
		if ((x.front()).size() == 9)
		{
			output << delimiter << "x_cargo" << delimiter << "y_cargo" << delimiter << "xdot_cargo" << delimiter << "ydot_cargo" << delimiter << "l_rope" << delimiter << "ldot_rope";
		}
		output << '\n';

		// loop over all time instances
		for (size_t i = 0; i < t.size(); i++)
		{
			// write [t, u, ...
			output << t.at(i) << delimiter << u.at(i).at(0) << delimiter << u.at(i).at(1);

			// write ... xi, xi+1 ...
			for (std::vector<double>::iterator it = std::begin(x.at(i)); it < std::end(x.at(i)); it++)
			{
				output << delimiter << *it ;
			}
			//write ... y]
			if(x.at(0).size() == 9)	output << delimiter << y.at(i).at(0) << delimiter << y.at(i).at(1);

			output << '\n';
		}
		output.close();
	}

private:
	// dynamics determines xdot at k from state xk and input uk
	std::vector<double> dynamics(std::vector<double> x, std::array<double, 2> u)
	{
		std::vector<double>  xdot;

		// dynamics for drone without cargo
		if (x.size() == 5)
		{
			xdot.push_back(x[3]);
			xdot.push_back(x[4]);
			xdot.push_back(u[1]);
			xdot.push_back((-u[0] * sin(x[2]*M_PI/180) - CdragDrone * sqrt(pow(x[3], 2) + pow(x[4], 2)) * x[3]) / massDrone);
			xdot.push_back((u[0] * cos(x[2]*M_PI/180) - CdragDrone * sqrt(pow(x[3], 2) + pow(x[4], 2)) * x[4]) / massDrone - gravitation);
		}

		// dynamics for drone with cargo
		else if (x.size() == 9)
		{
			// ropeLength and ropeLengthDot
			std::array<double, 2> yk; 
		//  y1    = sqrt( (   x1   - x6)^2    + (   x2   - x7)^2 )
			yk[0] = sqrt( pow(x[0] - x[5], 2) + pow(x[1] - x[6], 2));
			if (yk[0] == 0)	yk[1] = 0;
			else yk[1] = ((x[0] - x[5]) * (x[3] - x[7]) + (x[1] - x[6]) * (x[4] - x[8])) / yk[0];
		//       y2   =  ((x1   - x6)   * (x4   - x8)  + ( x2   - x7)   * (x5   - x9)  ) / y1

			y.push_back(yk);
			
			double Frope = stiffnessRope * (yk[0] - lengthRope0) + dampingRope * yk[1];
			double Fropex;
			double Fropey;

			// only apply force if there is tension in the rope
			if (Frope > 0)
			{
				Fropex = Frope * (x[0] - x[5]) / yk[0];
				Fropey = Frope * (x[1] - x[6]) / yk[0];
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
			xdot.push_back((-u[0] * sin(x[2]*M_PI/180)	- CdragDrone * sqrt(pow(x[3], 2) + pow(x[4], 2)) * x[3] - Fropex) / massDrone);
			xdot.push_back(( u[0] * cos(x[2]*M_PI/180)	- CdragDrone * sqrt(pow(x[3], 2) + pow(x[4], 2)) * x[4] - Fropey) / massDrone - gravitation);
			// dynamics for cargo
			xdot.push_back(x[7]);
			xdot.push_back(x[8]);
			xdot.push_back((							- CdragCargo * sqrt(pow(x[7], 2) + pow(x[8], 2)) * x[7] + Fropex) / massCargo);
			xdot.push_back((							- CdragCargo * sqrt(pow(x[7], 2) + pow(x[8], 2)) * x[8] + Fropey) / massCargo - gravitation);
		}

		return xdot;
	}

	// utility function for product of scalar{p} and vector{x}
	std::vector<double> scalarVectorProd(double p, std::vector<double> x)
	{
		std::vector<double> y;
		for (std::vector<double>::iterator it = std::begin(x); it < std::end(x); it++)
		{
			y.push_back((*it) * p);
		}
		return y;
	}

	// utility function for sum of 2 vectors{x1},{x2}
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