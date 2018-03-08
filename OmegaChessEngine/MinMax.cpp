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

std::pair<int, Move> MinMax::maxValue(BoardRepresentation boardRepresentation, bool makesWhiteWin)
{
	if (this->stopSearching(boardRepresentation))
		return std::pair<int, Move>(this->getHeuristic(boardRepresentation), Move{ -1, -1 });

	//Black wants the biggest negative score and white want the biggest positive score
	//Max player want the best move depending of its camp
	int v = makesWhiteWin  ? -1 * Evaluation::biggestEvaluation : Evaluation::biggestEvaluation;
	Move bestMove{ -1,-1 };
	std::vector<Move> possibleMoves = this->generateLegalMoves(boardRepresentation);
	for (Move& m : possibleMoves)
	{
		boardRepresentation.makeMove(m);
		int score = minValue(boardRepresentation, makesWhiteWin);
		boardRepresentation.unmakeMove(m);
		if (makesWhiteWin)
		{
			if (score == Evaluation::biggestEvaluation) //Force checkmath for max player
				return std::pair<int, Move>(score, Move{ -1, -1 });
			if (v < score)
			{
				v = score;
				bestMove = m;
			}
		}
		else
		{
			if (score == -1* Evaluation::biggestEvaluation) //Force checkmath for max player
				return std::pair<int, Move>(score, Move{ -1, -1 });
			if (v > score)
			{
				v = score;
				bestMove = m;
			}
		}
		
	}

	return std::pair<int, Move>(v, bestMove);
}

int MinMax::minValue(BoardRepresentation boardRepresentation, bool makesWhiteWin)
{
	if (this->stopSearching(boardRepresentation))
		return this->getHeuristic(boardRepresentation);
	//Black wants the biggest negative score and white want the biggest positive score
	//Min player want the best move depending of its camp
	int v = makesWhiteWin ? Evaluation::biggestEvaluation : -1 *Evaluation::biggestEvaluation;
	std::vector<Move> possibleMoves = this->generateLegalMoves(boardRepresentation);
	for (Move& m : possibleMoves)
	{
		boardRepresentation.makeMove(m);
		int score = maxValue(boardRepresentation, makesWhiteWin).first;
		boardRepresentation.unmakeMove(m);
		if (makesWhiteWin)
		{
			if (score == -1 * Evaluation::biggestEvaluation) //Force checkmath for min player
				return score;
			if (v > score)
				v = score;
		}
		else
		{
			if (score == Evaluation::biggestEvaluation) //Force checkmath for min player
				return score;
			if (v < score)
				v = score;
		}
		
	}

	return v;
}

std::pair<int,Move> MinMax::run(BoardRepresentation boardRepresentation)
{
	bool makesWhiteWin = boardRepresentation.isWhiteTurn;
	return maxValue(boardRepresentation, makesWhiteWin);
}
