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


	void WriteCSVHelper(std::string fileName, std::vector<std::array<double, 2>> u, std::vector<double> t, std::vector<std::vector<double>> x)
	{
		char delimiter = ',';

		std::ofstream output(fileName);

		output << "time" << delimiter << "input thrust" << delimiter << "input tilt" << delimiter << "x_drone" << delimiter << "y_drone"<< delimiter << "theta" << delimiter << "xdot_drone" << delimiter << "ydot_drone";
		if ((x.front()).size() == 9)
		{
			output << "x_cargo" << delimiter << "y_cargo" << delimiter << "xdot_cargo" << delimiter << "ydot_cargo";
		}
		output << '\n';

		for (size_t i = 0; i < t.size(); i++)
		{
			output << t.at(i) << delimiter << u.at(i).at(0) << delimiter << u.at(i).at(1);

			for (size_t j = 0; j < x.at(j+1).size(); j++)
			{
				output << delimiter << x.at(i).at(j);
			}
			output << '\n';
		}
		output.close();
	}

private:
	std::vector<double> dynamics(std::vector<double> x, std::array<double, 2> u)
	{
		std::vector<double>  xdot;

		xdot.push_back(x[3]);
		xdot.push_back(x[4]);
		xdot.push_back(u[1]);
		xdot.push_back((-u[0] * sin(x[2]) - CdragDrone * sqrt(pow(x[3], 2) + pow(x[4], 2)) * x[3]) / massDrone);
		xdot.push_back((u[0] * cos(x[2]) - CdragDrone * sqrt(pow(x[3], 2) + pow(x[4], 2)) * x[4]) / massDrone - gravitation);
	
		if (x.size() == 9)
		{
			xdot.push_back(x[7]);
			xdot.push_back(x[8]);
			xdot.push_back((-CdragCargo * sqrt(pow(x[7], 2) + pow(x[8], 2)) * x[7]) / massCargo);
			xdot.push_back((-CdragCargo * sqrt(pow(x[7], 2) + pow(x[8], 2)) * x[8]) / massCargo - gravitation);
		}

		return xdot;
	}

	std::vector<double> scalarVectorProd(double p, std::vector<double> x)
	{
		std::vector<double> y;
		for (std::vector<double>::iterator it = std::begin(x); it < std::end(x); it++)
		{
			y.push_back((*it) * p);
		}
		return y;
	}

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
	static const double lengthRope;
	static const double stiffnessRope;
	static const double dampingRope;
	static const double gravitation;
};//end class definition

const double Simulator::massDrone = 3;
const double Simulator::massCargo = 2;
const double Simulator::CdragDrone = 0.1;
const double Simulator::CdragCargo = 0.1;
const double Simulator::lengthRope = 1.5;
const double Simulator::stiffnessRope = 40000;
const double Simulator::dampingRope = 50;
const double Simulator::gravitation = 9.81;


