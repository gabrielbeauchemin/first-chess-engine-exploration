#include "stdafx.h"
#include "CppUnitTest.h"
#include "../OmegaChessEngine/Move.h"
#include "string"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(MoveTest)
	{
	public:
		
		TEST_METHOD(GetCoordinateAlgebraicNotationCorners)
		{
			//Test all corners (limit case)
			Move corner1and2{ 0, 7 };
			Move corner3and4{ 56, 63 };

			Assert::IsTrue(corner1and2.getCoordinateAlgebraicNotation() == "a1h1");
			Assert::IsTrue(corner3and4.getCoordinateAlgebraicNotation() == "a8h8");
		}

		//TEST_METHOD(GetCoordinateAlgebraicNotationOutBound)
		//{
		//	//They shoould assert
		//	Move corner1and2{ -1, 23 };
		//	Move corner1and2{ 35, 64 }; 
		//}

		TEST_METHOD(GetCoordinateAlgebraicNotationRandomCases)
		{
			Move corner1and2{ 23, 34 };
			Move corner3and4{ 11, 14 };

			Assert::IsTrue(corner1and2.getCoordinateAlgebraicNotation() == "h3c5");
			Assert::IsTrue(corner3and4.getCoordinateAlgebraicNotation() == "d2g2");
		}
	};
}