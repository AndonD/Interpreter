#include "Interpreter\Interpreter.h"
#include <iostream>

int main()
{
	InputReader input("Testing programs\\test1.txt");
	Interpreter interpreter(input);

	try
	{
		interpreter.start();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
