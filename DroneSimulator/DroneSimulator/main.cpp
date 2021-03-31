#include "InputStatic.h"
#include "Graphics.h"

int main(int argc, char* args[])
{
	double timestep = 0.1;
	InputStatic input("input.csv", timestep);
	input.getInput(13.3, 22.1);
	//std::array<double, 2> i1 = input.getInput(22);
	//std::array<double, 2> i2 = input.getInput(3);
	//std::array<double, 2> i3 = input.getInput(25);
	//std::array<double, 2> i4 = input.getInput(17);
	//std::array<double, 2> i5 = input.getInput(18);


	//std::cout << "\n\nt = 22; input u =" << i1[0] << ", " << i1[1] << std::endl;
	//std::cout << "t = 3; input u =" << i2[0] << ", " << i2[1] << std::endl;
	//std::cout << "t = 25; input u =" << i3[0] << ", " << i3[1] << std::endl;
	//std::cout << "t = 17; input u =" << i4[0] << ", " << i4[1] << std::endl;
	//std::cout << "t = 18; input u =" << i5[0] << ", " << i5[1] << std::endl;

	//i1 = input.getInput(1);
	//std::cout << "\n\nt = 1; input u =" << i1[0] << ", " << i1[1] << std::endl;

	//i1 = input.getInput(8);
	//std::cout << "\n\nt = 8; input u =" << i1[0] << ", " << i1[1] << std::endl;

	//i1 = input.getInput(23);
	//std::cout << "\n\nt = 23; input u =" << i1[0] << ", " << i1[1] << std::endl;

	//i1 = input.getInput(0);
	//std::cout << "\n\nt = 0; input u =" << i1[0] << ", " << i1[1] << std::endl;

	//i1 = input.getInput(10);
	//std::cout << "\n\nt = 10; input u =" << i1[0] << ", " << i1[1] << std::endl;

	//i1 = input.getInput(28);
	//std::cout << "\n\nt = 28; input u =" << i1[0] << ", " << i1[1] << std::endl;

	Graphics gArrowTexture;

	bool quit = false;

	SDL_Event e;

	double degrees = 0;
	int x = 0;
	int y = 0;

	SDL_RendererFlip flipType = SDL_FLIP_NONE;

	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_k:
					degrees -= 10;
					break;

				case SDLK_l:
					degrees += 10;
					break;

				case SDLK_a: // to left
					x -= 6;
					break;

				case SDLK_d: // to right
					x += 6;
					break;

				case SDLK_w:  // up
					y -= 6;
					break;

				case SDLK_s: // down
					y += 6;
					break;
				}
			}
		}
		gArrowTexture.updateGraphics(x, y, degrees, true, 0, 0);
	}

	gArrowTexture.close();

	return 0;
}

