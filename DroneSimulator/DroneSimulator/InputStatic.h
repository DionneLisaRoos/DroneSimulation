//==============================================================
// Filename : InputStatic.h
// Authors : Dionne Ariëns and Marnick Los.
// Version : -
// License : -
// Description : This class reads an CSV input file and splits this into usable information for the dynamics calculation. 
//				 In addition, using the timestep, the input will be given for every time unit using interpolation.
//==============================================================

#pragma once
#include <vector>
#include <fstream>
#include <array>

class InputStatic
{
public:
	InputStatic(std::string fileName, double timestep) : initialTime(0), finalTime(0), timeStep(timestep) { readCSV(fileName); };
	~InputStatic() {};

	// Get the full time vector
	std::vector<double> getTime() {
		return time;
	}

	// Get the full input vector
	std::vector<std::array<double, 2>> getInput() {
		return input;
	}
	
	// Get the input at a certain time
	std::array<double, 2> getInput(double t) {
		// Return empty array when aked time is not in the constraint
		if (t > time.back()) {
			return { };
		}

		// Search for t. Since t is sorted we can use binary_search to find the index of the t we are looking for.
		int ind = binary_search(t);

		// Return the input at the given time t.
		return input.at(ind);
	}

	// With this function only a part of the whole constrained can be asked. Then a new initial and 
	// final time are given and these are used to create the net input and time vectors. 
	std::vector<std::array<double, 2>> getInput(double tstart, double tend) {

		// Print error to screen when the asked time is out of bounds
		if (tend > time.back()) {
			std::cout << "ERROR - end time out of boundaries\n";
			return { };
		}
		
		// Erease the first element of the time and input vector as long the starting time is not the first element of the time vector.
		while (abs(tstart - time.front()) > 1e-9) {
			time.erase(time.begin());
			input.erase(input.begin());
		}

		// Delete the last elements of the time and input vector as long the element end time is not at the back of the time vector.
		while (abs(tend - time.back()) > 1e-9) {
			time.pop_back();
			input.pop_back();
		}

		// Set new initial and final time
		initialTime = tstart;
		finalTime = tend;

		// Return the new input vector
		return input;
	}

	// Get initial time
	double getInitalTime() {
		return initialTime;
	}

	// Get final time
	double getFinalTime() {
		return finalTime;
	}

private:
	// Private variables
	std::vector<double> time;
	std::vector<std::array<double,2>> input;
	double initialTime;
	double finalTime;
	double timeStep;

	// This function reads input from CSV file
	void readCSV(std::string fileName) {
		std::ifstream inputFile(fileName);
		std::string line;
		std::vector<int> pos;
		char delimiter = ',';
		std::array<double, 2> temp = { 0.0, 0.0 };

		getline(inputFile, line); //skip first line because of headers

		// Get the information of the file line by line so we can process the data
		while (getline(inputFile, line)) {
			// Get all data from string by spitting the string at the delimiter and save this information in pos

			for (std::string::size_type i = 0; i < line.size(); i++) {
				if (line[i] == delimiter) {
					pos.push_back(i);
				}
			}

			// Fill the time and input vectors with the data read from the input file (from one line)
			time.push_back(std::stod(line.substr(0, pos.at(0))));
			temp[0] = std::stod(line.substr(pos.at(0) + 1, pos.at(1)));
			temp[1] = std::stod(line.substr(pos.at(1) + 1, line.size()));
			pos.clear();

			input.push_back(temp);
		}

		// Set initial and final times
		initialTime = time.front();
		finalTime = time.back();

		// Start the function interpolate to interpolate the time and input vectors for all timesteps. 
		interpolate();
	}

	// This function interpolates for all timestep values between initial and final time
	void interpolate() {
		std::vector<double>::iterator next = std::begin(time)+1;
		int current = 0;
		double t = time.at(0);
		std::vector<double> newTime;
		std::vector<std::array<double, 2>> newInput;

		// Save the initial time in the new vectors
		newTime.push_back(time.at(0));
		newInput.push_back(input.at(0));

		// Loop through the time vector and fill the the new time vector with all the 
		// timesteps between initial and final time and fill the input vector with the last 
		// given value of the input until a new value is found at a certain time t.
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

		// clear old time and input vectors
		time.clear();
		input.clear();

		// fill the empty time and input vectors with the new data
		time = newTime;
		input = newInput;
	}

	// binary_search algorithm to loop through time vector to find index of time unit that needs to be found. 
	// When the time unit that needs to be found is not present in the time vector, the function will return -1.  
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

