#ifndef Notation_H
#define Notation_H

#include <string>
#include "InvalidChessCaseException.h"
#include "NotImplementedException.h"

struct Notation
{
public:
	Notation() = delete;

	Notation(int from, int to)
		: from{from}, to{to}
	{
		auto isOutBound = [](int chessCase) { return (chessCase > 64 || chessCase < 0); };
		if (isOutBound(from) || isOutBound(to))
			throw InvalidChessCaseException();

		throw NotImplementedException();
	}

	std::string pureCoordinate;
	int from;
	int to;
	
};

#endif

