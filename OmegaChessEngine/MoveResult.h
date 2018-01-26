#ifndef MoveResult_H
#define MoveResult_h



struct MoveResult
{
	bool isMoveLegal;
	bool isEndGame;
	bool isWhiteWinner;

	MoveResult()
		: isMoveLegal{ true }, isEndGame{ false }
	{
	}

	MoveResult(bool isMoveLegal)
		: isMoveLegal{ isMoveLegal }, isEndGame{ false }
	{
	}

	MoveResult(bool isMoveLegal, bool isEndGame, bool isWhiteWinner)
		: isMoveLegal{ isMoveLegal }, isEndGame{ isEndGame }, isWhiteWinner{isWhiteWinner}
	{
	}
};

#endif // !MoveResult_H

