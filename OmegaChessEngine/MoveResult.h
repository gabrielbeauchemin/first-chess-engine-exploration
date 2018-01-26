#ifndef MoveResult_H
#define MoveResult_h

struct MoveResult
{
	bool isMoveLegal;
	bool isEndGame;
	bool isWhiteWinner;
	bool isStealMate;

	MoveResult()
		: isMoveLegal{ true }, isEndGame{ false }
	{
	}

	MoveResult(bool isMoveLegal)
		: isMoveLegal{ isMoveLegal }, isEndGame{ false }
	{
	}

	MoveResult(bool isMoveLegal, bool isEndGame, bool isWhiteWinner, bool isStealMate = false)
		: isMoveLegal{ isMoveLegal },
		isEndGame{ isEndGame },
		isWhiteWinner{isWhiteWinner},
		isStealMate{isStealMate}
	{
	}
};

#endif // !MoveResult_H

