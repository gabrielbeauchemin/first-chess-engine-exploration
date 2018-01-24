#ifndef NotImplementedException_H
#define NotImplementedException_H

#include <exception>
#include <iostream>

class NotImplementedException : public std::logic_error
{
public:
	NotImplementedException() : std::logic_error("Function not yet implemented") { };
};

#endif


