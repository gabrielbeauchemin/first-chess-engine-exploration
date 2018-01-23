#ifndef Piece_H
#define Piece_H

enum class PieceType
{
	king,
	queen,
	bishop,
	knight,
	pawn
};

struct Piece
{
	PieceType type;
	bool isWhite;

	Piece() = delete;
	Piece(PieceType type, bool isWhite)
		:type{type}, isWhite{isWhite}
	{
	}
};

#endif
