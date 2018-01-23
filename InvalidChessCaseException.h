#ifndef InvalidChessCaseException_H
#define InvalidChessCaseException_H

#include <exception>
#include <iostream>

struct InvalidChessCaseException : public std::exception
{
	const char * what() const throw ()
	{
		return "Invalid Chess Case";
	}
};

#endif

