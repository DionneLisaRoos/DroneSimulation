#pragma once
#include "Simulator.h"
#include "InputKeyboard.h"
#include "Graphics.h"

class SimDynamic : public Simulator
{
public:
	SimDynamic(std::vector<double> x0, InputKeyboard input, double timestep, bool cargo) : graphics(cargo)
	{
		x.push_back(x0);
		t.push_back(0);
		u.push_back(input.getInput());
		timeStep = timestep;
		InputKeyboard input = input;
	};

	~SimDynamic()
	{};

	void dynamicSimulation()
	{
		Uint32 timeout = SDL_GetTicks() + 1000 / FPS;
		SDL_Event e;

		while (!quit)
		{
			xk = eulerForward(xk, uk);
			uk = input.getInput();
			graphics.updateGraphics(x.at(0), x.at(1), x.at(2));
			x.push_back(xk);
			u.push_back(uk);
			t.push_back(t.end() + 1000 / FPS);
			while (!SDL_TICKS_PASSED(SDL_GetTicks(), timeout))
			{
				while (SDL_PollEvent(&e))
				{

				}
			}
			timeout += 1000 / FPS;
		}
	}




private:
	//storage variables
	std::vector<double> t;
	std::vector<std::vector<double>> x;
	std::vector<double> xk;
	std::vector<std::array<double, 2>> u;
	std::array<double, 2> uk;
	const int FPS = 30;
	bool quit;
};
