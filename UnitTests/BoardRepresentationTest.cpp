#include "stdafx.h"
#include "CppUnitTest.h"
#include "../OmegaChessEngine/BoardRepresentation.h"
#include "../OmegaChessEngine/BoardRepresentation.cpp"
#include "string"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(BoardRepresentationTest)
	{
	public:

		TEST_METHOD(TestToString)
		{
			BoardRepresentation board;
			std::string stringBoard = board.toString();
			
			const std::string validStringBoard = 
				"#################################################\r\n" 
				"#     #     #     #     #     #     #     #     #\r\n" 
				"#  r  #  n  #  b  #  k  #  q  #  b  #  n  #  r  #\r\n" 
				"#     #     #     #     #     #     #     #     #\r\n" 
				"#################################################\r\n" 
				"#     #     #     #     #     #     #     #     #\r\n" 
				"#  p  #  p  #  p  #  p  #  p  #  p  #  p  #  p  #\r\n" 
				"#     #     #     #     #     #     #     #     #\r\n" 
				"#################################################\r\n" 
				"#     #     #     #     #     #     #     #     #\r\n" 
				"#     #     #     #     #     #     #     #     #\r\n" 
				"#     #     #     #     #     #     #     #     #\r\n" 
				"#################################################\r\n" 
				"#     #     #     #     #     #     #     #     #\r\n" 
				"#     #     #     #     #     #     #     #     #\r\n" 
				"#     #     #     #     #     #     #     #     #\r\n" 
				"#################################################\r\n" 
				"#     #     #     #     #     #     #     #     #\r\n" 
				"#     #     #     #     #     #     #     #     #\r\n" 
				"#     #     #     #     #     #     #     #     #\r\n" 
				"#################################################\r\n" 
				"#     #     #     #     #     #     #     #     #\r\n" 
				"#     #     #     #     #     #     #     #     #\r\n" 
				"#     #     #     #     #     #     #     #     #\r\n" 
				"#################################################\r\n" 
				"#     #     #     #     #     #     #     #     #\r\n" 
				"#  p  #  p  #  p  #  p  #  p  #  p  #  p  #  p  #\r\n" 
				"#     #     #     #     #     #     #     #     #\r\n" 
				"#################################################\r\n" 
				"#     #     #     #     #     #     #     #     #\r\n" 
				"#  r  #  n  #  b  #  k  #  q  #  b  #  n  #  r  #\r\n" 
				"#     #     #     #     #     #     #     #     #\r\n" 
				"#################################################\r\n"; 

			Assert::AreEqual(stringBoard, validStringBoard);
		}


	};
}