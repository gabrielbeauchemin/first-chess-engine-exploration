#ifndef Piece_H
#define Piece_H

enum class PieceType
{
	king,
	queen,
	bishop,
	rook,
	knight,
	pawn,
	none
};

struct Piece
{
	PieceType type;
	bool isWhite;

	Piece() {}
	Piece(PieceType type, bool isWhite)
		:type{type}, isWhite{isWhite}
	{
	}
};

#endif
