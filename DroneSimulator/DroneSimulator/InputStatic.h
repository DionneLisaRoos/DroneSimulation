#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <array>
#include <sstream>

class InputStatic
{
public:
	InputStatic(std::string fileName) : initialTime(0), finalTime(0) { readCSV(fileName); };
	~InputStatic() {};

	std::vector<double> getTime() {
		return time;
	}

	std::vector<std::array<double, 2>> getInput() {
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

		/*std::cout << "time test:\n";
		for (std::vector<double>::iterator it = std::begin(time); it != std::end(time); ++it) {
			std::cout << *it << "\n";
		}

		std::cout << "\ninput test:\n";
		for (std::vector<std::array<double, 2>>::iterator it = std::begin(input); it != std::end(input); ++it) {
			std::cout << it->at(0) << "\t" << it->at(1) << "\n";
		}

		std::cout << "\ninitial time:   " << initialTime << "\n";

		std::cout << "\nfinal time:   " << finalTime << "\n";*/	
	}
};

