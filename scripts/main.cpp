#include <iostream>
#include <stdio.h>

#include "constants.hpp"

int main(void)
{
	printf("Hello world!\n");

	std::cout << "Version : " << TeenyTinyBits_VERSION_MAJOR << '.' << TeenyTinyBits_VERSION_MAJOR << '.' << TeenyTinyBits_VERSION_PATCH << '\n';
	return 0;
}
