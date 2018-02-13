#include "Piece.h"

//Functions could be optimise with operations on the bits
bool isPieceKing(Piece p) { return (p == Piece::whiteKing || p == Piece::blackKing); }
bool isPieceQueen(Piece p) { return (p == Piece::whiteQueen || p == Piece::blackQueen); }
bool isPieceBishop(Piece p) { return (p == Piece::whiteBishop || p == Piece::blackBishop); }
bool isPieceKnight(Piece p) { return (p == Piece::whiteKnight || p == Piece::blackKnight); }
bool isPiecePawn(Piece p) { return (p == Piece::whitePawn || p == Piece::blackPawn); }
bool isPieceRook(Piece p) { return (p == Piece::whiteRook || p == Piece::blackRook); }
bool isPieceNone(Piece p) { return p == Piece::none; }

bool isPieceWhite(Piece p)
{
	return (p == Piece::whiteKing || p == Piece::whiteQueen || p == Piece::whiteBishop
		|| p == Piece::whiteKnight || p == Piece::whitePawn) || p == Piece::whiteRook;
}
bool isPieceBlack(Piece p)
{
	return (p == Piece::blackKing || p == Piece::blackQueen || p == Piece::blackBishop
		|| p == Piece::blackKnight || p == Piece::blackPawn || p == Piece::blackRook);
}
std::vector<Piece> getWhitePieces()
{
	return std::vector<Piece>{ Piece::whiteBishop, Piece::whiteKnight, Piece::whiteRook,
		Piece::whiteKing, Piece::whiteQueen, Piece::whitePawn};
}

std::vector<Piece> getBlackPieces()
{
	return std::vector<Piece>{ Piece::blackBishop, Piece::blackKnight, Piece::blackRook,
		Piece::blackKing, Piece::blackQueen, Piece::blackPawn};
}