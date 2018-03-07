#include "Search.h"
#include "Evaluation.h"
#include "MoveGeneration.hpp"




Search::Search(int msMaxByMove)
	: timer{msMaxByMove},
	minMax{ Evaluation::evaluate, MoveGeneration::generateMoves, [&](BoardRepresentation b) {return timer.isTimeOut(); } }
{
}

Move Search::run(BoardRepresentation b)
{
	timer.reset();
	return minMax.run(b);
}

void Search::stop()
{
	timer.forceTimeOut();
}

void Search::setTimeMax(int msMaxByMove)
{
	timer.setTimeMax(msMaxByMove);
}
