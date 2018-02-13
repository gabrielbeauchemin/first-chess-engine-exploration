#ifndef Piece_H
#define Piece_H

#include <vector>

enum class Piece : unsigned char
{
	whiteKing,
	blackKing,
	whiteQueen,
	blackQueen,
	whiteRook,
	blackRook,
	whiteBishop,
	blackBishop,
	whiteKnight,
	blackKnight,
	whitePawn,
	blackPawn,
	none
};

//Functions could be optimise with operations on the bits
bool isPieceKing(Piece p);
bool isPieceQueen(Piece p);
bool isPieceBishop(Piece p);
bool isPieceKnight(Piece p);
bool isPiecePawn(Piece p);
bool isPieceRook(Piece p);
bool isPieceNone(Piece p);

bool isPieceWhite(Piece p);
bool isPieceBlack(Piece p);
std::vector<Piece> getWhitePieces();
std::vector<Piece> getBlackPieces();

#endif
