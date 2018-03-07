#include "MinMax.h"
#include "Evaluation.h"

MinMax::MinMax()
{
}

MinMax::MinMax(std::function<int(BoardRepresentation)> getHeuristic, std::function<std::vector<Move>(BoardRepresentation)> generateLegalMoves, std::function<bool(BoardRepresentation)> stopSearching)
	:getHeuristic{getHeuristic},
	generateLegalMoves{generateLegalMoves},
	stopSearching{stopSearching}
{
}

std::pair<int, Move> MinMax::maxValue(BoardRepresentation boardRepresentation)
{
	if (this->stopSearching(boardRepresentation))
		return std::pair<int, Move>(this->getHeuristic(boardRepresentation), Move{ -1, -1 });

	int v = std::numeric_limits<int>::min();
	Move bestMove{ -1,-1 };
	std::vector<Move> possibleMoves = this->generateLegalMoves(boardRepresentation);
	for (Move& m : possibleMoves)
	{
		boardRepresentation.makeMove(m);
		int score = minValue(boardRepresentation);
		boardRepresentation.unmakeMove(m);
		if (score == Evaluation::biggestEvaluation) //Force checkmat for max player
			return std::pair<int, Move>(score, Move{ -1, -1 });
		if (v < score)
		{
			v = score;
			bestMove = m;
		}
	}

	return std::pair<int, Move>(v, bestMove);
	
}

int MinMax::minValue(BoardRepresentation boardRepresentation)
{
	if (this->stopSearching(boardRepresentation))
		return this->getHeuristic(boardRepresentation);

	int v = std::numeric_limits<int>::max();
	std::vector<Move> possibleMoves = this->generateLegalMoves(boardRepresentation);
	for (Move& m : possibleMoves)
	{
		boardRepresentation.makeMove(m);
		int score = maxValue(boardRepresentation).first;
		boardRepresentation.unmakeMove(m);
		if (score == -1 * Evaluation::biggestEvaluation) //Force checkmat for min player
			return score;
		if (v > score)
			v = score;
	}

	return v;
}

Move MinMax::run(BoardRepresentation boardRepresentation)
{
	return maxValue(boardRepresentation).second;
}
