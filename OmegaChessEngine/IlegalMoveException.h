#ifndef IlegalMoveException_H
#define IlegalMoveException_H

#include <exception>
#include <iostream>

struct IlegalMoveException : public std::exception
{
	const char * what() const throw ()
	{
		return "Illegal Move Exception";
	}
};

#endif

