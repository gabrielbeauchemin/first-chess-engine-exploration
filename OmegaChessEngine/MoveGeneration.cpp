#include "MoveGeneration.hpp"
#include <algorithm>
//#define NDEBUG 
#include <assert.h>
#include <string>

namespace MoveGeneration
{
	static int mailbox[120] = {
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1,  0,  1,  2,  3,  4,  5,  6,  7, -1,
		-1,  8,  9, 10, 11, 12, 13, 14, 15, -1,
		-1, 16, 17, 18, 19, 20, 21, 22, 23, -1,
		-1, 24, 25, 26, 27, 28, 29, 30, 31, -1,
		-1, 32, 33, 34, 35, 36, 37, 38, 39, -1,
		-1, 40, 41, 42, 43, 44, 45, 46, 47, -1,
		-1, 48, 49, 50, 51, 52, 53, 54, 55, -1,
		-1, 56, 57, 58, 59, 60, 61, 62, 63, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1
	};

	static const int mailbox64[64] = {
		21, 22, 23, 24, 25, 26, 27, 28,
		31, 32, 33, 34, 35, 36, 37, 38,
		41, 42, 43, 44, 45, 46, 47, 48,
		51, 52, 53, 54, 55, 56, 57, 58,
		61, 62, 63, 64, 65, 66, 67, 68,
		71, 72, 73, 74, 75, 76, 77, 78,
		81, 82, 83, 84, 85, 86, 87, 88,
		91, 92, 93, 94, 95, 96, 97, 98
	};

	static const std::vector<int> knightOffset = { -21, -19, -12, -8, 8, 12, 19, 21 };
	static const std::vector<int> bishopOffset = { -11,  -9,  9, 11};
	static const std::vector<int> blackPawnOffset = { -9,  -11};
	static const std::vector<int> whitePawnOffset = {9, 11 };
	static const std::vector<int> rookOffset = { -10,  -1,  1, 10};
	static const std::vector<int> queenOffset = { -11, -10, -9, -1, 1,  9, 10, 11 };
	static const std::vector<int> kingOffset = { -11, -10, -9, -1, 1,  9, 10, 11 };

	std::vector<Move> generateMoves(BoardRepresentation& boardRepresentation)
	{
		//Find king index
		int kingIndex = -1;
		for (int caseIndex = 0; caseIndex < 64; ++caseIndex)
		{
			if (isPieceKing(boardRepresentation.board[caseIndex]) &&
				isPieceWhite(boardRepresentation.board[caseIndex]) == boardRepresentation.isWhiteTurn)
			{
				kingIndex = caseIndex;
				break;
			}
		}
		assert(kingIndex != -1);

		//If the king is cheked
		bool isKingInCheck = false;
		if (isPieceAttacked(boardRepresentation, kingIndex))
		{
			isKingInCheck = true;
		}

		std::vector<Move> moves;
		for (int caseIndex = 0; caseIndex < 64; ++caseIndex)
		{
			Piece currentPiece = boardRepresentation.board[caseIndex];

			//Move only the pieces of the color which is turn to play
			if (isPieceWhite(currentPiece) != boardRepresentation.isWhiteTurn)
				continue;

			if (isPieceNone(currentPiece))
			{
				continue;
			}
			else
			{
				//Pieces that are in absolute pin can move only if it doesnt check the king
				//Take note of absolute pin to send it to generate moves functions
				//The exceptions are the knights, which cant move in any direction without cheking its king
				bool isPiecePinned = false;
				if (!isPieceKing(currentPiece) &&
					isPieceInAbsolutePin(boardRepresentation, caseIndex, kingIndex))
				{
					isPiecePinned = true;
					if (isPieceKnight(currentPiece))
						continue;
				}

				if (isPiecePawn(currentPiece))
				{
					auto newMoves = generatePawnMoves(boardRepresentation, caseIndex, isPiecePinned, kingIndex);
					if (isKingInCheck) filterMovesUncheckingKing(boardRepresentation, newMoves, kingIndex);
					moves.insert(moves.end(), newMoves.begin(), newMoves.end());
				}
				else if (isPieceBishop(currentPiece))
				{
					auto newMoves = generateBishopMoves(boardRepresentation, caseIndex, isPiecePinned);
					if (isKingInCheck) filterMovesUncheckingKing(boardRepresentation, newMoves, kingIndex);
					moves.insert(moves.end(), newMoves.begin(), newMoves.end());
				}
				else if (isPieceKnight(currentPiece))
				{
					auto newMoves = generateKnightMoves(boardRepresentation, caseIndex);
					if (isKingInCheck) filterMovesUncheckingKing(boardRepresentation, newMoves, kingIndex);
					moves.insert(moves.end(), newMoves.begin(), newMoves.end());
				}
				else if (isPieceRook(currentPiece))
				{
					auto newMoves = generateRookMoves(boardRepresentation, caseIndex, isPiecePinned);
					if (isKingInCheck) filterMovesUncheckingKing(boardRepresentation, newMoves, kingIndex);
					moves.insert(moves.end(), newMoves.begin(), newMoves.end());
				}
				else if (isPieceKing(currentPiece))
				{
					auto newMoves = generateKingMoves(boardRepresentation, caseIndex);
					moves.insert(moves.end(), newMoves.begin(), newMoves.end());
					if (!isKingInCheck)
					{
						newMoves = generateCastlingMoves(boardRepresentation, caseIndex);
						moves.insert(moves.end(), newMoves.begin(), newMoves.end());
					}
					
				}
				else if (isPieceQueen(currentPiece))
				{
					auto newMoves = generateQueenMoves(boardRepresentation, caseIndex, isPiecePinned);
					if (isKingInCheck) filterMovesUncheckingKing(boardRepresentation, newMoves, kingIndex);
					moves.insert(moves.end(), newMoves.begin(), newMoves.end());
				}
			}
			
		}

		return moves;
	}

	std::vector<Move> generateRookMoves(const BoardRepresentation& boardRepresentation, int rookCase, bool isPiecePinned = false)
	{
		std::vector<Move> moves;
		std::vector<int> filteredOffsets; //Offset filtered in case of pinned piece
		if (isPiecePinned)
			filteredOffsets = getLegalOffsetPinnedPiece(boardRepresentation, rookCase, rookOffset);
		else
			filteredOffsets = rookOffset;

		//Each iteration, move the piece of one case in the direction it can move to generate the moves
		for (const int& currentOffset : filteredOffsets)
		{
			int currentCaseIndex = rookCase;
			//Generate all possible moves in the direction the current offset
			while (true)
			{
				currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];
				if (currentCaseIndex == -1) break; //Outside of the board

				//The case is occupied
				if (!isPieceNone(boardRepresentation.board[currentCaseIndex]))
				{
					if (isPieceWhite(boardRepresentation.board[currentCaseIndex]) != boardRepresentation.isWhiteTurn)
						moves.push_back(Move{ rookCase,currentCaseIndex }); //Capture

					//its a capture or the piece got blocked by a piece of its camp, 
					//in both case pass to the next direction
					break;
				}
				else //The case is empty
				{
					moves.push_back(Move{ rookCase,currentCaseIndex });
				}
				
			}
		}

		return moves;
	}

	std::vector<Move> generateKnightMoves(const BoardRepresentation& boardRepresentation, int knightCase)
	{
		std::vector<Move> moves;

		//Each iteration, move the piece of one case in the direction it can move to generate the moves
		for (const int& currentOffset : knightOffset)
		{
			int currentCaseIndex = knightCase;
			//Generate one move in the direction the current offset
			currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];
			if (currentCaseIndex == -1) continue; //Outside of the board

			//The case is occupied
			if (!isPieceNone(boardRepresentation.board[currentCaseIndex]))
			{
				if (isPieceWhite(boardRepresentation.board[currentCaseIndex]) != boardRepresentation.isWhiteTurn)
					moves.push_back(Move{ knightCase,currentCaseIndex }); //Capture

				//its a capture or the piece got blocked by a piece of its camp, 
				//in both case pass to the next direction																
				continue;
			}
			else //The case is empty
			{
				moves.push_back(Move{ knightCase,currentCaseIndex });

			}
		}

		return moves;
	}

	std::vector<Move> generateBishopMoves(const BoardRepresentation& boardRepresentation, int bishopCase, bool isPiecePinned)
	{
		std::vector<Move> moves;
		std::vector<int> filteredOffsets; //Offset filtered in case of pinned piece
		if (isPiecePinned)
			filteredOffsets = getLegalOffsetPinnedPiece(boardRepresentation, bishopCase, bishopOffset);
		else
			filteredOffsets = bishopOffset;

		//Each iteration, move the piece of one case in the direction it can move to generate the moves
		for (const int& currentOffset : filteredOffsets)
		{
			int currentCaseIndex = bishopCase;
			//Generate all possible moves in the direction the current offset
			while (true)
			{
				currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];
				if (currentCaseIndex == -1) break; //Outside of the board

				//The case is occupied
				if (!isPieceNone(boardRepresentation.board[currentCaseIndex]))
				{
					if (isPieceWhite(boardRepresentation.board[currentCaseIndex]) != boardRepresentation.isWhiteTurn)
						moves.push_back(Move{ bishopCase,currentCaseIndex }); //Capture
					//its a capture or the piece got blocked by a piece of its camp, 
					//in both case pass to the next direction
					break;
				}
				else //The case is empty
				{
					moves.push_back(Move{ bishopCase,currentCaseIndex });
				}
			}
		}

		return moves;
	}

	//Do not include the castling move to be more modular
	std::vector<Move> generateKingMoves(BoardRepresentation& boardRepresentation, int kingCase)
	{
		std::vector<Move> moves;

		//Each iteration, move the piece of one case in the direction it can move to generate the moves
		for (const int& currentOffset : kingOffset)
		{
			int currentCaseIndex = kingCase;
			//Generate one move in the direction the current offset
			currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];
			if (currentCaseIndex == -1) continue; //Outside of the board

			//The case is occupied
			if (!isPieceNone(boardRepresentation.board[currentCaseIndex]))
			{
				//Capture opposite piece
				if (isPieceWhite(boardRepresentation.board[currentCaseIndex]) != boardRepresentation.isWhiteTurn)
				{
					//The opposite piece should not be protected to be captured by the king
					Move tmpMove = Move{ kingCase, currentCaseIndex };
					boardRepresentation.makeMove(tmpMove );
					boardRepresentation.isWhiteTurn = !boardRepresentation.isWhiteTurn;
					if(!isPieceAttacked(boardRepresentation, currentCaseIndex))
					   moves.push_back(Move{ kingCase,currentCaseIndex }); 
					boardRepresentation.isWhiteTurn = !boardRepresentation.isWhiteTurn;
					boardRepresentation.unmakeMove(tmpMove);
				}

				//its a capture or the piece got blocked by a piece of its camp, 
				//in both case pass to the next direction																
				continue;
			}
			else //The case is empty
			{
				//The king should not be in check at this new case
				//Remove temporarly the king to evaluate if the case is attacked
				Piece tmpKing = boardRepresentation.board[kingCase]; 
				assert(isPieceKing(tmpKing));
				boardRepresentation.board[kingCase] = Piece::none;
				if (!isPieceAttacked(boardRepresentation, currentCaseIndex))
				   moves.push_back(Move{ kingCase,currentCaseIndex });
				boardRepresentation.board[kingCase] = tmpKing;
			}
			
		}

		return moves;
	}

	//Do not check if king is in check, it is checked in generateMoves
	std::vector<Move> generateCastlingMoves(BoardRepresentation& boardRepresentation, int kingCase)
	{
		//GenerateCastlingMoves should not be call if king hes in check.
		//It should have be checked before, else its a waste of time to check it two times
		assert(!isPieceAttacked(boardRepresentation, kingCase));

		std::vector<Move> possibleCastlings;
		std::vector<Move> castlingMoves;
		if (boardRepresentation.isWhiteTurn)
		{
			if (boardRepresentation.canWhiteKingCastle)
				castlingMoves.push_back(Move{ 4,6 });
			if (boardRepresentation.canWhiteQueenCastle)
				castlingMoves.push_back(Move{ 4,2 });
		}
		else
		{
			if (boardRepresentation.canBlackKingCastle)
				castlingMoves.push_back(Move{ 60,62 });
			if (boardRepresentation.canBlackQueenCastle)
				castlingMoves.push_back(Move{ 60,58 });
		}

		//For possible castling
		for (auto castle : castlingMoves)
		{
			//If there is place between the king and the rook
			bool isSpaceToCastle = true;
			int kingDirection = (castle.from - castle.to < 0) ? 1 : -1;
			for (int indexCase = kingCase + kingDirection;
				!isPieceRook(boardRepresentation.board[indexCase]); indexCase += kingDirection)
			{
				if (!isPieceNone(boardRepresentation.board[indexCase]) ||
					isPieceAttacked(boardRepresentation, indexCase))
				{
					isSpaceToCastle = false;
					break; 
				}
			}
			if (isSpaceToCastle)
			{
				possibleCastlings.push_back(castle);
			}

		}
		
		return possibleCastlings;
	}

	std::vector<Move> generateQueenMoves(const BoardRepresentation& boardRepresentation, int queenCase, bool isPiecePinned)
	{
		std::vector<Move> moves;
		std::vector<int> filteredOffsets; //Offset filtered in case of pinned piece
		if (isPiecePinned)
			filteredOffsets = getLegalOffsetPinnedPiece(boardRepresentation, queenCase, queenOffset);
		else
			filteredOffsets = queenOffset;

		//Each iteration, move the piece of one case in the direction it can move to generate the moves
		for (const int& currentOffset : filteredOffsets)
		{
			int currentCaseIndex = queenCase;
			//Generate all possible moves in the direction the current offset
			while (true)
			{
				currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];
				if (currentCaseIndex == -1) break; //Outside of the board

			   //The case is occupied
				if (!isPieceNone(boardRepresentation.board[currentCaseIndex]))
				{
					if (isPieceWhite(boardRepresentation.board[currentCaseIndex]) != boardRepresentation.isWhiteTurn)
						moves.push_back(Move{ queenCase,currentCaseIndex }); //Capture
					//its a capture or the piece got blocked by a piece of its camp, 
				   //in both case pass to the next direction
					break;
				}
				else //The case is empty
				{
					moves.push_back(Move{ queenCase,currentCaseIndex });
				}
			}
		}

		return moves;
	}

	//Does not include the move pawn for a promotion to be more modular, see function generatePawnPromotion
	std::vector<Move> generatePawnMoves(BoardRepresentation& boardRepresentation, int pawnCase, bool isPiecePinned, int kingIndex)
	{
		std::vector<Move> moves;
		const int row = 8;

		auto isMoveCheckingKing = [kingIndex, &boardRepresentation](Move m)
		{
			if (!isPieceNone(boardRepresentation.board[m.to])) return false;
			boardRepresentation.makeMove(m);
			boardRepresentation.isWhiteTurn = !boardRepresentation.isWhiteTurn;
			bool isKingInCheck = isPieceAttacked(boardRepresentation, kingIndex);
			boardRepresentation.isWhiteTurn = !boardRepresentation.isWhiteTurn;
			boardRepresentation.unmakeMove(m);
			return isKingInCheck;
		};

		//In case of promotion
		if (isPawnMovePromotion(boardRepresentation, pawnCase))
		{
			std::vector<Piece> possiblePromotion = (boardRepresentation.isWhiteTurn)
				? std::vector<Piece>{Piece::whiteBishop, Piece::whiteKnight, Piece::whiteRook, Piece:: whiteQueen}
			    : std::vector<Piece>{ Piece::blackBishop, Piece::blackKnight, Piece::blackRook, Piece::blackQueen };

			for (auto& promotionType : possiblePromotion)
			{
				std::vector<Move> promotionMoves = generatePawnPromotionMoves(boardRepresentation, pawnCase, promotionType);
				if (isPiecePinned)
				{
					promotionMoves.erase( //Remove all promotion move that are checking the king
						std::remove_if(promotionMoves.begin(), promotionMoves.end(),
							[&](const Move & m){return isMoveCheckingKing(m);}), promotionMoves.end() );
				}
				moves.insert(moves.end(), promotionMoves.begin(), promotionMoves.end());
			}
			return moves;
		}
		
		//Moving forward of one case if the case is empty
		int step = (boardRepresentation.isWhiteTurn) ? row : -row;
		bool isNextCaseEmpty = isPieceNone(boardRepresentation.board[pawnCase + step]);
		bool isMoveForwardLegal = true;
		if (isPiecePinned) isMoveForwardLegal = !isMoveCheckingKing(Move{ pawnCase,pawnCase + step });
		if (isNextCaseEmpty && isMoveForwardLegal)
			moves.push_back(Move{ pawnCase,pawnCase + step });
		
		//Moving forward of two cases is only possible if the pawn is on its initial case
		bool isOnInitialRow = (boardRepresentation.isWhiteTurn) ? pawnCase / row == 1
			                                      : pawnCase / row == 6;
		if (isOnInitialRow)
		{
			if(isNextCaseEmpty && isPieceNone(boardRepresentation.board[pawnCase + step + step])
				&& isMoveForwardLegal)
			    moves.push_back(Move{ pawnCase,pawnCase + step + step });
		}

		//Capture a piece
		std::vector<int> pawnOffset = (boardRepresentation.isWhiteTurn) ? whitePawnOffset : blackPawnOffset;
		for (const int& currentOffset : pawnOffset)
		{
			int currentCaseIndex = pawnCase;
			//Generate one move in the direction the current offset
			currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];
			if (currentCaseIndex == -1) continue; //Outside of the board

			//The case is occupied
			if (!isPieceNone(boardRepresentation.board[currentCaseIndex]))
			{
				Move capture{ pawnCase,currentCaseIndex };
				//Capture opposite piece
				if (isPieceWhite(boardRepresentation.board[currentCaseIndex]) != boardRepresentation.isWhiteTurn)
				{
					if(!isPiecePinned || !isMoveCheckingKing(capture))
					moves.push_back(capture);
				}
				//Else the pawn is blocked by a piece of its camp, nothing to do
			}
			//Else the case is empty, pawn cannot eat it
		}

		//Coup en passant if it is possible
		if (boardRepresentation.isEnPensantPossible.first )
		{
			static int directionsToCheck[] = { -1,1 };
			for (int& sideDirection : directionsToCheck)
			{
				int sideOfPossibleEnPassant = mailbox[mailbox64[boardRepresentation.isEnPensantPossible.second] + sideDirection];
				if (sideOfPossibleEnPassant == -1) break; //Outside of the board
				if (sideOfPossibleEnPassant == pawnCase)
				{
					int posAfterEnPassant = boardRepresentation.isEnPensantPossible.second + step;
					Move enPassant{ pawnCase, posAfterEnPassant };
					if(!isPiecePinned || !isMoveCheckingKing(enPassant))
					moves.push_back(enPassant);

				}
			}
		}

		return moves;
	}

	std::vector<Move> generatePawnPromotionMoves(const BoardRepresentation& board, int pawnCase, Piece promotion)
	{
		std::vector<Move> moves;
		const int row = 8;
		auto isOutBound = [](int pos) { return pos < 0 || pos > 63; };
		int step = (board.isWhiteTurn) ? row : -row;
		//Avance of one case
		if (isPieceNone(board.board[pawnCase + step]))
		{
			moves.push_back(Move{ pawnCase,pawnCase + step, promotion });
		}
		//Capture Left
		if (!isPieceNone(board.board[pawnCase + step - 1])
			&& isPieceWhite(board.board[pawnCase + step -1]) != board.isWhiteTurn
			&& !isOutBound(pawnCase + step -1))
		{
			moves.push_back(Move{ pawnCase,pawnCase + step - 1, promotion });
		}
		//Capture Right
		if (!isPieceNone(board.board[pawnCase + step + 1])
			&& isPieceWhite(board.board[pawnCase + step +1]) != board.isWhiteTurn
			&& !isOutBound(pawnCase + step + 1))
		{
			moves.push_back(Move{ pawnCase,pawnCase + step + 1, promotion });
		}

		return moves;
	}

	bool isPieceAttacked(BoardRepresentation& boardRepresentation, int pieceCase)
	{
		//Remove the piece for the evaluation, as it can block potential attack
		//Put it back at the end
		Piece piecePotentiallyAttacked = boardRepresentation.board[pieceCase];
		boardRepresentation.board[pieceCase] = Piece::none;

		//Each iteration, move the piece in diagonal
		//If he meets bishop, queen, pawn or king from the opposite camp, hes in check
		for (const int& currentOffset : bishopOffset /*Diagonal Moves*/)
		{
			int currentCaseIndex = pieceCase;

			//Generate all possible moves in the direction the current diagonal
			bool firstOffset = true;
			while (true)
			{
				currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];
				if (currentCaseIndex == -1) break; //Outside of the board

				//piece meets a piece
				auto currentCase = boardRepresentation.board[currentCaseIndex];
				if (!isPieceNone(currentCase))
				{
					//piece meets pawn after only one offset
					if (isPieceWhite(currentCase) != boardRepresentation.isWhiteTurn
						&& (isPiecePawn(currentCase)) && firstOffset)
					{
						//Pawns can only eat moving forward
						if (isPieceWhite(currentCase))
						{
							if (currentCaseIndex < pieceCase)
							{
								boardRepresentation.board[pieceCase] = piecePotentiallyAttacked;
								return true;
							}
						}
						else
						{
							if (currentCaseIndex > pieceCase)
							{
								boardRepresentation.board[pieceCase] = piecePotentiallyAttacked;
								return true;
							}
						}
						
					}
					//piece meets king after only one offset
					else if (isPieceWhite(currentCase) != boardRepresentation.isWhiteTurn
						&& (isPieceKing(currentCase)) && firstOffset)
					{
						boardRepresentation.board[pieceCase] = piecePotentiallyAttacked;
						return true;
					}
					//piece meets a bishop or queen from opposite camp, hes in check
					else if (isPieceWhite(currentCase) != boardRepresentation.isWhiteTurn &&
						     (isPieceQueen(currentCase) || isPieceBishop(currentCase)) )
					{
						boardRepresentation.board[pieceCase] = piecePotentiallyAttacked;
						return true;
					}
					//He meets a piece of its own camp or a piece from the opposite camp
					//That cant put him in check. Look into another direction
					else
					{
						break;
					}
				}

				//The case is empty, continue searching in this direction
				firstOffset = false;
			}
		}

		//Each iteration, move the piece in lines
		//If he meets rook,queen or king from the opposite camp, hes in check
		for (const int& currentOffset : rookOffset /*Moves in line*/)
		{
			int currentCaseIndex = pieceCase;
			bool firstOffset = true;

			//Generate all possible moves in the direction the current offset
			while (true)
			{
				currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];
				if (currentCaseIndex == -1) break; //Outside of the board

				//piece meets a piece
				auto currentCase = boardRepresentation.board[currentCaseIndex];
				if (!isPieceNone(currentCase))
				{
					//piece meets king after only one offset
					if (isPieceWhite(currentCase) != boardRepresentation.isWhiteTurn
						&& isPieceKing(currentCase) && firstOffset)
					{
						boardRepresentation.board[pieceCase] = piecePotentiallyAttacked;
						return true;
					}
					//piece meets a rook or queen from opposite camp, hes in check
					else if (isPieceWhite(currentCase) != boardRepresentation.isWhiteTurn
						&& (isPieceQueen(currentCase) || isPieceRook(currentCase)))
					{
						boardRepresentation.board[pieceCase] = piecePotentiallyAttacked;
						return true;
					}
					//He meets a piece of its own camp or a piece from the opposite camp
					//That cant put him in check. Look into another direction
					else
					{
						break;
					}
				}

				//The case is empty, nothing to do, continue searching in this direction
				firstOffset = false;
			}
		}

		//Move the piece like a knight: 
		//If he meets a knight from the opposite camp, hes in check
		for (const int& currentOffset : knightOffset)
		{
			int currentCaseIndex = pieceCase;
			currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];

			if (currentCaseIndex == -1) continue; //Outside of the board

			auto currentCase = boardRepresentation.board[currentCaseIndex];
			//piece meets a rook or queen from opposite camp, hes in check
			if (isPieceWhite(currentCase) != boardRepresentation.isWhiteTurn
				&& isPieceKnight(currentCase))
			{
				boardRepresentation.board[pieceCase] = piecePotentiallyAttacked;
				return true;
			}
		}

		//piece didnt find any pieces of the opposite camp that are attacking him
		boardRepresentation.board[pieceCase] = piecePotentiallyAttacked;
		return false;

	}

	int nbrTimesPieceAttacked(BoardRepresentation& boardRepresentation, int pieceCase)
	{
		int nbrTimesPieceAttacked = 0;
		//Remove the piece for the evaluation, as it can block potential attack
		//Put it back at the end
		Piece piecePotentiallyAttacked = boardRepresentation.board[pieceCase];
		boardRepresentation.board[pieceCase] = Piece::none;

		//Each iteration, move the piece in diagonal
		//If he meets bishop, queen, pawn or king from the opposite camp, hes in check
		for (const int& currentOffset : bishopOffset /*Diagonal Moves*/)
		{
			int currentCaseIndex = pieceCase;

			//Generate all possible moves in the direction the current diagonal
			bool firstOffset = true;
			while (true)
			{
				currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];
				if (currentCaseIndex == -1) break; //Outside of the board

	            //piece meets a piece
				auto currentCase = boardRepresentation.board[currentCaseIndex];
				if (!isPieceNone(currentCase))
				{
					//piece meets pawn after only one offset
					if (isPieceWhite(currentCase) != boardRepresentation.isWhiteTurn
						&& (isPiecePawn(currentCase) && firstOffset))
					{
						//Pawns can only eat forward
						if (isPieceWhite(currentCase))
						{
							if (currentCaseIndex < pieceCase)
							{
								nbrTimesPieceAttacked++;
							}
						}
						else
						{
							if (currentCaseIndex > pieceCase)
							{
								nbrTimesPieceAttacked++;
							}
						}
					}
					//piece meets king after only one offset
					else if (isPieceWhite(currentCase) != boardRepresentation.isWhiteTurn
						&& (isPieceKing(currentCase)) && firstOffset)
					{
						nbrTimesPieceAttacked++;
					}
					//piece meets pawn after only one offset
					else if (isPieceWhite(currentCase) != boardRepresentation.isWhiteTurn
						&& isPiecePawn(currentCase) && firstOffset)
					{
						nbrTimesPieceAttacked++;
					}
					//piece meets a bishop or queen from opposite camp, hes in check
					else if (isPieceWhite(currentCase) != boardRepresentation.isWhiteTurn &&
						(isPieceQueen(currentCase) || isPieceBishop(currentCase)))
					{
						nbrTimesPieceAttacked++;
					}
					//He meets a piece of its own camp or a piece from the opposite camp
					//That cant put him in check. Look into another direction
					else
					{
						break;
					}
				}

				//The case is empty, continue searching in this direction
				firstOffset = false;
			}
		}

		//Each iteration, move the piece in lines
		//If he meets rook or queen from the opposite camp, hes in check
		for (const int& currentOffset : rookOffset /*Moves in line*/)
		{
			int currentCaseIndex = pieceCase;
			bool firstOffset = true;
			//Generate all possible moves in the direction the current offset
			while (true)
			{
				currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];
				if (currentCaseIndex == -1) break; //Outside of the board

												   //piece meets a piece
				auto currentCase = boardRepresentation.board[currentCaseIndex];
				if (!isPieceNone(currentCase))
				{
					//piece meets king after only one offset
					if (isPieceWhite(currentCase) != boardRepresentation.isWhiteTurn
						&& isPieceKing(currentCase) && firstOffset)
					{
						boardRepresentation.board[pieceCase] = piecePotentiallyAttacked;
						return true;
					}
					//piece meets a rook or queen from opposite camp, hes in check
					else if (isPieceWhite(currentCase) != boardRepresentation.isWhiteTurn
						&& (isPieceQueen(currentCase) || isPieceRook(currentCase)))
					{
						nbrTimesPieceAttacked++;
					}
					//He meets a piece of its own camp or a piece from the opposite camp
					//That cant put him in check. Look into another direction
					else
					{
						break;
					}
				}

				//The case is empty, nothing to do, continue searching in this direction
				firstOffset = false;
			}
		}

		//Move the piece like a knight: 
		//If he meets a knight from the opposite camp, hes in check
		for (const int& currentOffset : knightOffset)
		{
			int currentCaseIndex = pieceCase;
			currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];

			if (currentCaseIndex == -1) continue; //Outside of the board

			auto currentCase = boardRepresentation.board[currentCaseIndex];
			//piece meets a rook or queen from opposite camp, hes in check
			if (isPieceWhite(currentCase) != boardRepresentation.isWhiteTurn
				&& isPieceKnight(currentCase))
			{
				nbrTimesPieceAttacked++;
			}
		}

		//piece didnt find any pieces of the opposite camp that are attacking him
		boardRepresentation.board[pieceCase] = piecePotentiallyAttacked;
		return nbrTimesPieceAttacked;
	}

	bool isPieceInAbsolutePin(BoardRepresentation& boardRepresentation, int pieceCase, int kingCase)
	{
		//If the piece is in absolute pin, that means that if
		//It is gone, the king is in check one more time (the king may already be in check)
		int nbrAttacksOnKing = nbrTimesPieceAttacked(boardRepresentation, kingCase);
		Piece tmp = boardRepresentation.board[pieceCase];
		boardRepresentation.board[pieceCase] = Piece::none;
		int nbrAttacksOnKingPieceOff = nbrTimesPieceAttacked(boardRepresentation, kingCase);
		boardRepresentation.board[pieceCase] = tmp;
		bool isPieceAsolutePin = nbrAttacksOnKing < nbrAttacksOnKingPieceOff;
		return isPieceAsolutePin;
	}

	bool isPawnMovePromotion(const BoardRepresentation& boardRepresentation, int pawnCaseBeforeMove)
	{
		if (boardRepresentation.isWhiteTurn)
		{
			return ((pawnCaseBeforeMove / 8) == 6);
		}
		else
		{
			return ((pawnCaseBeforeMove / 8) == 1);
		}
	}

	void filterMovesUncheckingKing(BoardRepresentation& boardRepresentation, std::vector<Move>& movesToFilter, int kingCase)
	{
		movesToFilter.erase(
			std::remove_if(movesToFilter.begin(), movesToFilter.end(),
				[&](const Move & move)
		{
			boardRepresentation.makeMove(move);
			boardRepresentation.isWhiteTurn = !boardRepresentation.isWhiteTurn; //Necessary so isPieceAttacked evaluate for the good camp
			bool isKingInCheck = isPieceAttacked(boardRepresentation, kingCase);
			boardRepresentation.isWhiteTurn = !boardRepresentation.isWhiteTurn;
			boardRepresentation.unmakeMove(move);
			return isKingInCheck;
		})
			, movesToFilter.end());
	}

	bool isKingCheckmate(BoardRepresentation & boardRepresentation, int kingCase)
	{
		assert(isPieceKing(boardRepresentation.board[kingCase]));
		return isPieceAttacked(boardRepresentation, kingCase) &&
			   generateMoves(boardRepresentation).size() == 0;
	}

	bool isKingStealMate(BoardRepresentation & boardRepresentation, int kingCase)
	{
		return generateMoves(boardRepresentation).size() == 0 &&
			  !isPieceAttacked(boardRepresentation, kingCase);
	}

	std::vector<int> getLegalOffsetPinnedPiece(const BoardRepresentation & boardRepresentation, int pinnedPieceCase, std::vector<int> possibleOffsets)
	{
		//If the Piece is a knight or a pawn, this function should not be call
		//Because knight cant move without checking his king
		//And pawn does its own logic if it is pinned
		auto pinnedPiece = boardRepresentation.board[pinnedPieceCase];
		assert(!isPieceKnight(pinnedPiece) && !isPiecePawn(pinnedPiece));

		//If the Piece Pinned is a bishop, rook or queen, search the King
		//and return the offset in the opposite direction of the king (the piece still protect the king)
		for (const int& currentOffset : possibleOffsets)
		{
			int currentCaseIndex = pinnedPieceCase;
			while (true)
			{
				currentCaseIndex = mailbox[mailbox64[currentCaseIndex] + currentOffset];

				if (currentCaseIndex == -1) break; //Outside of the board

				auto currentCase = boardRepresentation.board[currentCaseIndex];
				if (isPieceKing(currentCase) && boardRepresentation.isWhiteTurn == isPieceWhite(currentCase))
				{
					std::vector<int> pinnedPieceLegalOffset;
					pinnedPieceLegalOffset.push_back(-1 * currentOffset);
					return pinnedPieceLegalOffset;
				}
			}
		}

		//King not found: that means that the pinned piece cannot move without checking the king
		return std::vector<int>();
	}

	bool isEnPassant(const BoardRepresentation& boardRepresentation, Move move)
	{
		if (!isPiecePawn(boardRepresentation.board[move.from]))
			return false; 
		if (!isPieceNone(boardRepresentation.board[move.to]))
			return false;
		
		if (isPieceWhite(boardRepresentation.board[move.from]))
		{
			if (move.from / 8 != 4)
			{
				return false;
			}
		}
		else
		{
			if (move.from / 8 != 3)
			{
				return false;
			}
		}

		const int row = 8;
		int step = isPieceWhite(boardRepresentation.board[move.from]) ? row : -1 * row;
		static int sideDirections[2] = { -1,1 };
		for (int& sideDirection : sideDirections)
		{
			int possiblePosEnPassant = mailbox[mailbox64[move.from] + step + sideDirection];
			if (possiblePosEnPassant != -1 && possiblePosEnPassant == move.to)
			{
				return true;
			}
		}

		return false;
	}
}

