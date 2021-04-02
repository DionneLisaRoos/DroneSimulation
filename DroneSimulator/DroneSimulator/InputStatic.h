#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <array>
#include <sstream>

class InputStatic
{
public:
	InputStatic(std::string fileName, double timestep) : initialTime(0), finalTime(0), timeStep(timestep) { readCSV(fileName); };
	~InputStatic() {};

	std::vector<double> getTime() {
		return time;
	}

	std::vector<std::array<double, 2>> getInput() {
		return input;
	}

	std::array<double, 2> getInput(double t) {
		if (t > time.back()) {
			return { };
		}
		int ind = binary_search(t);
		return input.at(ind);
	}

	std::vector<std::array<double, 2>> getInput(double tstart, double tend) {

		if (tend > time.back()) {
			std::cout << "ERROR - end time out of boundaries\n";
			return { };
		}

		while (abs(tstart - time.front()) > 1e-9) {
			time.erase(time.begin());
			input.erase(input.begin());
		}
		std::cout << time.size();
		while (abs(tend - time.back()) > 1e-9) {
			time.pop_back();
			input.pop_back();
		}

		initialTime = tstart;
		finalTime = tend;

		/*std::cout << "time test:\n";
		std::vector<std::array<double, 2>>::iterator inputit = std::begin(input);
		for (std::vector<double>::iterator it = std::begin(time); it != std::end(time); ++it) {
			std::cout << *it << "\t" << inputit->at(0) << "\t" << inputit->at(1) << "\n";
			inputit++;
		}*/

		return input;
	}

	double getInitalTime() {
		return initialTime;
	}

	double getFinalTime() {
		return finalTime;
	}

private:
	std::vector<double> time;
	std::vector<std::array<double,2>> input;
	double initialTime;
	double finalTime;
	double timeStep;

	void readCSV(std::string fileName) {
		std::ifstream inputFile(fileName);
		std::string line;
		std::vector<int> pos;
		char delimiter = ';';
		std::array<double, 2> temp = { 0.0, 0.0 };

		getline(inputFile, line); //skip first line because of headers

		while (getline(inputFile, line)) {
			for (std::string::size_type i = 0; i < line.size(); i++) {
				if (line[i] == delimiter) {
					pos.push_back(i);
				}
			}

			time.push_back(std::stod(line.substr(0, pos.at(0))));
			temp[0] = std::stod(line.substr(pos.at(0) + 1, pos.at(1)));
			temp[1] = std::stod(line.substr(pos.at(1) + 1, line.size()));
			pos.clear();

			input.push_back(temp);
		}

		initialTime = time.front();
		finalTime = time.back();

		interpolate();

		/*std::cout << "time test:\n";
		std::vector<std::array<double, 2>>::iterator inputit = std::begin(input);
		for (std::vector<double>::iterator it = std::begin(time); it != std::end(time); ++it) {
			std::cout << *it << "\t" << inputit->at(0) << "\t" << inputit->at(1) << "\n";
			inputit++;
		}*/

		/*std::cout << "\ninput test:\n";
		for (std::vector<std::array<double, 2>>::iterator it = std::begin(input); it != std::end(input); ++it) {
			std::cout << it->at(0) << "\t" << it->at(1) << "\n";
		}

		std::cout << "\ninitial time:   " << initialTime << "\n";

		std::cout << "\nfinal time:   " << finalTime << "\n";	*/

		/*std::cout << "\nsize time:   " << time.size() << "\n";

		std::cout << "\nsize input:   " << input.size() << "\n";*/
	}

	void interpolate() {
		std::vector<double>::iterator next = std::begin(time)+1;
		int current = 0;
		double t = time.at(0);
		std::vector<double> newTime;
		std::vector<std::array<double, 2>> newInput;

		newTime.push_back(time.at(0));
		newInput.push_back(input.at(0));

		while (next != std::end(time)) {
			t += timeStep;

			if ((abs(t - *next) < 1e-9) || (t > *next)) {
				current += 1;
				newTime.push_back(t);
				newInput.push_back(input.at(current));
				next++;
			} else if (t < *next) {
				newTime.push_back(t);
				newInput.push_back(input.at(current));
			}
		}

		time.clear();
		input.clear();

		time = newTime;
		input = newInput;
	}

	int binary_search(double t) {
		auto it = std::lower_bound(time.begin(), time.end(), t);
		if (it == time.end() || *it != t) {
			return -1;
		}
		else {
			std::size_t index = std::distance(time.begin(), it);
			return index;
		}
	}
};

