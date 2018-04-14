#ifndef Search_H
#define Search_H 

#include "BoardRepresentation.h"
#include "AlphaBeta.h"
#include "Timer.h"

class Search
{
public:
	Search(int msMaxByMove);
	Move run(BoardRepresentation&);
	void stop();
	void setTimeMax(uint64_t msMaxByMove);

private:
	AlphaBeta alphaBeta;
	Timer timer;
};

#endif

