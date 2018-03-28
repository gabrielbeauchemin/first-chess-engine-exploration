//The protocol is based and inspired by the one of this open source engine:
//https://github.com/fluxroot/pulse/blob/master/src/main/cpp/pulse.cpp

#include "UCIProtocol.h"
#include <iostream>
#include <sstream>

void UCIProtocol::run() {
	std::cin.exceptions(std::iostream::eofbit | std::iostream::failbit | std::iostream::badbit);
	while (true) {
		std::string line;
		std::getline(std::cin, line);
		std::istringstream input(line);

		std::string token;
		input >> std::skipws >> token;
		if (token == "uci") {
			receiveInitialize();
		}
		else if (token == "isready") {
			receiveReady();
		}
		else if (token == "ucinewgame") {
			receiveNewGame();
		}
		else if (token == "position") {
			receivePosition(input);
		}
		else if (token == "go") {
			receiveGo(input);
		}
		else if (token == "stop") {
			receiveStop();
		}
		else if (token == "ponderhit") {
			receivePonderHit();
		}
		else if (token == "quit") {
			receiveQuit();
			break;
		}
	}
}

void UCIProtocol::receiveQuit() {
	// We received a quit command. Stop calculating now and
	// cleanup!
	search.stop();
}

void UCIProtocol::receiveInitialize() {
	search.stop();

	// We received an initialization request.

	// We could do some global initialization here. Probably it would be best
	// to initialize all tables here as they will exist until the end of the
	// program.

	// We must send an initialization answer back!
	std::cout << "id name Omega Chess engine" << std::endl;
	std::cout << "id author Gabriel Beauchemin" << std::endl;
	std::cout << "uciok" << std::endl;
}

void UCIProtocol::receiveReady() {
	// We received a ready request. We must send the token back as soon as we
	// can. However, because we launch the search in a separate thread, our main
	// thread is able to handle the commands asynchronously to the search. If we
	// don't answer the ready request in time, our engine will probably be
	// killed by the GUI.
	std::cout << "readyok" << std::endl;
}

void UCIProtocol::receiveNewGame() {
	search.stop();
	search = Search(this->msMaxByMove );

	// We received a new game command.
	// Initialize per-game settings here.
	currentPosition = BoardRepresentation{};
}

void UCIProtocol::receivePosition(std::istringstream& input) {
	search.stop();

	// We received an position command. Just setup the position.
	std::string token;
	input >> token;
	if (token == "startpos") {
		currentPosition = BoardRepresentation{};
		if (input >> token) {
			if (token != "moves") {
				throw std::exception();
			}
		}
	}
	else if (token == "fen") {
		std::string fen;

		while (input >> token) {
			if (token != "moves") {
				fen += token + " ";
			}
			else {
				break;
			}
		}

		currentPosition = BoardRepresentation(fen);
	}
	else {
		throw std::exception();
	}

	while (input >> token) {
		// Verify if the move done by Opposite camp is a legal move
		// And make the move to complete the board representation position
		std::vector<Move> moves = MoveGeneration::generateMoves(currentPosition);

		bool found = false;
		for (Move& move : moves) {

			if (move.getCoordinateAlgebraicNotation() == token) {
				currentPosition.makeMove(move);
				found = true;
				break;
			}
		}
		std::cout << std::endl;

		if (!found) {
			throw std::exception();
		}
		
	}

	// Don't start searching though!
}

void UCIProtocol::receiveGo(std::istringstream& input) {
	search.stop();

	// We received a start command. Extract all parameters from the
	// command and start the search.
	std::string token;
	input >> token;
	if (token == "depth") {
		int searchDepth;
		if (input >> searchDepth) {
			//TO DO
			//search->newDepthSearch(*currentPosition, searchDepth);
		}
		else {
			throw std::exception();
		}
	}
	else if (token == "nodes") {
		uint64_t searchNodes;
		if (input >> searchNodes) {
			//TO DO
			//search->newNodesSearch(*currentPosition, searchNodes);
		}
	}
	else if (token == "movetime") {
		uint64_t secSearchTime;
		if (input >> secSearchTime) {
		   this->search.setTimeMax(secSearchTime);
		}
	}
	else if (token == "infinite") {
		//TO DO
		//search->newInfiniteSearch(*currentPosition);
	}
	else {
		uint64_t whiteTimeLeft = 1;
		uint64_t whiteTimeIncrement = 0;
		uint64_t blackTimeLeft = 1;
		uint64_t blackTimeIncrement = 0;
		int searchMovesToGo = 40;
		bool ponder = false;

		do {
			if (token == "wtime") {
				if (!(input >> whiteTimeLeft)) {
					throw std::exception();
				}
				//less than one minute for the engine: play faster!
				if (this->currentPosition.isWhiteTurn && whiteTimeLeft < 60000)
				{
					this->search.setTimeMax(whiteTimeLeft/6);
				}
			}
			else if (token == "winc") {
				if (!(input >> whiteTimeIncrement)) {
					throw std::exception();
				}
			}
			else if (token == "btime") {
				if (!(input >> blackTimeLeft)) {
					throw std::exception();
				}
				//less than one minute for the engine: play faster!
				if (!this->currentPosition.isWhiteTurn && blackTimeLeft < 60000)
				{
					this->search.setTimeMax(blackTimeLeft / 6);
				}
			}
			else if (token == "binc") {
				if (!(input >> blackTimeIncrement)) {
					throw std::exception();
				}
			}
			else if (token == "movestogo") {
				if (!(input >> searchMovesToGo)) {
					throw std::exception();
				}
			}
			else if (token == "ponder") {
				ponder = true;
			}
		} while (input >> token);

		if (ponder) {
			//TO DO
		}
		else {
			//TO DO:
			//Add setting found about the search
		}
	}

	std::cout << "before playing best Move" << std::endl;
	Move bestMove = this->search.run(currentPosition);
	sendBestMove(bestMove, Move{ -1,-1 });
}

void UCIProtocol::receivePonderHit() {
	// We received a ponder hit command. 
	//TO DO
	//search.ponderhit();
}

void UCIProtocol::receiveStop() {
	// We received a stop command. If a search is running, stop it.
	search.stop();
}


void UCIProtocol::sendBestMove(Move bestMove, Move ponderMove) {
	std::cout << "bestmove ";

	if (bestMove.from != -1 && bestMove.to != -1) {
		std::cout << bestMove.getCoordinateAlgebraicNotation();

		if (bestMove.from != -1 && bestMove.to != -1) {
			std::cout << " ponder " << ponderMove.getCoordinateAlgebraicNotation();
		}
	}
	else {
		std::cout << "nomove";
	}

	std::cout << std::endl;
}


//Next are examples how to cout info to the UI
//void UCIProtocol::sendStatus(
//	int currentDepth, int currentMaxDepth, uint64_t totalNodes, int currentMove, int currentMoveNumber) {
//	if (std::chrono::duration_cast<std::chrono::milliseconds>(
//		std::chrono::system_clock::now() - statusStartTime).count() >= 1000) {
//		sendStatus(false, currentDepth, currentMaxDepth, totalNodes, currentMove, currentMoveNumber);
//	}
//}
//
//void UCIProtocol::sendStatus(
//	bool force, int currentDepth, int currentMaxDepth, uint64_t totalNodes, int currentMove,
//	int currentMoveNumber) {
//	auto timeDelta = std::chrono::duration_cast<std::chrono::milliseconds>(
//		std::chrono::system_clock::now() - startTime);
//
//	if (force || timeDelta.count() >= 1000) {
//		std::cout << "info";
//		std::cout << " depth " << currentDepth;
//		std::cout << " seldepth " << currentMaxDepth;
//		std::cout << " nodes " << totalNodes;
//		std::cout << " time " << timeDelta.count();
//		std::cout << " nps " << (timeDelta.count() >= 1000 ? (totalNodes * 1000) / timeDelta.count() : 0);
//
//		if (currentMove != Move::NOMOVE) {
//			std::cout << " currmove " << fromMove(currentMove);
//			std::cout << " currmovenumber " << currentMoveNumber;
//		}
//
//		std::cout << std::endl;
//
//		statusStartTime = std::chrono::system_clock::now();
//	}
//}
//
//void UCIProtocol::sendMove(RootEntry entry, int currentDepth, int currentMaxDepth, uint64_t totalNodes) {
//	auto timeDelta = std::chrono::duration_cast<std::chrono::milliseconds>(
//		std::chrono::system_clock::now() - startTime);
//
//	std::cout << "info";
//	std::cout << " depth " << currentDepth;
//	std::cout << " seldepth " << currentMaxDepth;
//	std::cout << " nodes " << totalNodes;
//	std::cout << " time " << timeDelta.count();
//	std::cout << " nps " << (timeDelta.count() >= 1000 ? (totalNodes * 1000) / timeDelta.count() : 0);
//
//	if (std::abs(entry.value) >= Value::CHECKMATE_THRESHOLD) {
//		// Calculate mate distance
//		int mateDepth = Value::CHECKMATE - std::abs(entry.value);
//		std::cout << " score mate " << ((entry.value > 0) - (entry.value < 0)) * (mateDepth + 1) / 2;
//	}
//	else {
//		std::cout << " score cp " << entry.value;
//	}
//
//	if (entry.pv.size > 0) {
//		std::cout << " pv";
//		for (int i = 0; i < entry.pv.size; i++) {
//			std::cout << " " << fromMove(entry.pv.moves[i]);
//		}
//	}
//
//	std::cout << std::endl;
//
//	statusStartTime = std::chrono::system_clock::now();
//}
//
//std::string UCIProtocol::fromMove(int move) {
//	std::string notation;
//
//	notation += Notation::fromSquare(Move::getOriginSquare(move));
//	notation += Notation::fromSquare(Move::getTargetSquare(move));
//
//	int promotion = Move::getPromotion(move);
//	if (promotion != PieceType::NOPIECETYPE) {
//		notation += std::tolower(Notation::fromPieceType(promotion));
//	}
//
//	return notation;
//}


