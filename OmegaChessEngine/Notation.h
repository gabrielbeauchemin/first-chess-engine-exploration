#ifndef Notation_H
#define Notation_H 

#include <string>
#include "InvalidChessCaseException.h"
#include <unordered_map>
#include "Piece.h"

/* Representation of a chess move that a piece have from
   a case to another.*/
struct Notation
{
public:
	int from;
	int to;
	PieceType promotion;

	Notation() = delete;

	// The constructor takes two case position in entry: from and to
	// The notation of a case position is the following:
	/* 
	   56 57 58 59 60 61 62 63
	   48 49 50 51 52 53 54 55
	   40 41 42 43 44 45 46 47
	   32 33 34 35 36 37 38 39
	   24 25 26 27 28 29 30 31
	   16 17 18 19 20 21 22 23
	   8  9  10 11 12 13 14 15
	   0  1  2  3  4  5  6  7 
	*/ 

	Notation(int from, int to, PieceType promotion = PieceType::none)
		: from{from}, to{to}, promotion{promotion}
	{
		auto isOutBound = [](int chessCase) { return (chessCase > 63 || chessCase < 0); };
		if (isOutBound(from) || isOutBound(to))
			throw InvalidChessCaseException();
	}

	std::string getCoordinateAlgebraicNotation()
	{
		std::string pureCoordinate;
		pureCoordinate = chessCaseToAlgebraic(from);
		pureCoordinate += chessCaseToAlgebraic(to);
		return pureCoordinate;
	}

	bool operator==(const Notation &other)
	{
		return other.from == this->from && other.to == this->to && other.promotion == this->promotion;
	}

private:
	std::unordered_map<int, std::string> rowIndexToAlgebraic{ { 0,"a" },{ 1,"b" },{ 2,"c" },{ 3,"d" },{ 4,"e" },{ 5,"f" },{ 6,"g" },{ 7,"h" } };

	std::string chessCaseToAlgebraic(int chessCase)
	{ 
		std::string algebraicNotation;
		const int COLUMN_SIZE = 8;
		int columnIndex = chessCase % COLUMN_SIZE;
		int rowIndex = chessCase / COLUMN_SIZE;

		algebraicNotation = rowIndexToAlgebraic[columnIndex];
		algebraicNotation += std::to_string(rowIndex + 1);

		return algebraicNotation;
	}
};

#endif

