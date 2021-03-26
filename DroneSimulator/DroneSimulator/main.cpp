#include "InputStatic.h"

int main(int argc, char* args[])
{
	InputStatic input("input.csv");
	std::array<double, 2> i1 = input.getInput(22);
	std::array<double, 2> i2 = input.getInput(3);
	std::array<double, 2> i3 = input.getInput(25);
	std::array<double, 2> i4 = input.getInput(17);
	std::array<double, 2> i5 = input.getInput(18);


	std::cout << "\n\nt = 22; input u =" << i1[0] << ", " << i1[1] << std::endl;
	std::cout << "t = 3; input u =" << i2[0] << ", " << i2[1] << std::endl;
	std::cout << "t = 25; input u =" << i3[0] << ", " << i3[1] << std::endl;
	std::cout << "t = 17; input u =" << i4[0] << ", " << i4[1] << std::endl;
	std::cout << "t = 18; input u =" << i5[0] << ", " << i5[1] << std::endl;

	i1 = input.getInput(1);
	std::cout << "\n\nt = 1; input u =" << i1[0] << ", " << i1[1] << std::endl;

	i1 = input.getInput(8);
	std::cout << "\n\nt = 8; input u =" << i1[0] << ", " << i1[1] << std::endl;

	i1 = input.getInput(23);
	std::cout << "\n\nt = 23; input u =" << i1[0] << ", " << i1[1] << std::endl;

	i1 = input.getInput(0);
	std::cout << "\n\nt = 0; input u =" << i1[0] << ", " << i1[1] << std::endl;

	i1 = input.getInput(10);
	std::cout << "\n\nt = 10; input u =" << i1[0] << ", " << i1[1] << std::endl;

	i1 = input.getInput(28);
	std::cout << "\n\nt = 28; input u =" << i1[0] << ", " << i1[1] << std::endl;

	return 0;
}