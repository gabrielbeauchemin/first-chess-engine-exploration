#include <iostream>
#include "UCIProtocol.h"

void printUsage()
{
	std::cout << "usage: omegaEngine \r\n No options for now" << std::endl;
}
int main(int argc, char* argv[])
{
	std::cout << "Patate" << std::endl;
	if (argc == 1) {
		UCIProtocol protocol;
		protocol.run();;
	}
	else if (argc > 1) {
		printUsage();
	}
	
}