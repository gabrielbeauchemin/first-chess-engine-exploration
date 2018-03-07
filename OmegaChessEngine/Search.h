#ifndef Search_H
#define Search_H 

#include "BoardRepresentation.h"
#include "MinMax.h"
#include "Timer.h"

class Search
{
public:
	Search(int msMaxByMove);
	Move run(BoardRepresentation);
	void stop();
	void setTimeMax(int msMaxByMove);

private:
	MinMax minMax;
	Timer timer;
};

#endif

