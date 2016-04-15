#include "stdafx.h"
#include "Calculator.h"
#include "StreamHandler.h"



int main()
{
	CCalculator calc;
	ÑStreamHandler calcCommandHandler(calc, std::cin, std::cout);

	while ((!std::cin.eof()) && (!std::cin.fail()))
	{
		std::cout << ">>";
		if (!calcCommandHandler.HandleCommand())
		{
			std::cout << "Unknown command!" << std::endl;
		}
	}

    return 0;
}

