#include <iostream>
#include "UCIProtocol.h"
#include <fstream>

void printUsage()
{
	std::cout << "usage: omegaEngine \r\n No options for now" << std::endl;
}
int main(int argc, char* argv[])
{
	std::ofstream out{ "log" };
	std::clog.rdbuf(out.rdbuf());
	if (argc == 1) {
		UCIProtocol protocol;
		protocol.run();;
	}
	else if (argc > 1) {
		printUsage();
	}
	std::clog.rdbuf(nullptr);
}