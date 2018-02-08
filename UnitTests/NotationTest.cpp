#include "stdafx.h"
#include "CppUnitTest.h"
#include "../OmegaChessEngine/Notation.h"
#include "../OmegaChessEngine/InvalidChessCaseException.h"
#include "string"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(NotationTest)
	{
	public:
		
		TEST_METHOD(GetCoordinateAlgebraicNotationCorners)
		{
			//Test all corners (limit case)
			Notation corner1and2{ 0, 7 };
			Notation corner3and4{ 56, 63 };

			Assert::IsTrue(corner1and2.getCoordinateAlgebraicNotation() == "a1h1");
			Assert::IsTrue(corner3and4.getCoordinateAlgebraicNotation() == "a8h8");
		}

		TEST_METHOD(GetCoordinateAlgebraicNotationOutBound)
		{
			//Outbound position should fail at construction
			Assert::ExpectException<InvalidChessCaseException>([] { Notation corner1and2{ -1, 23 }; });
			Assert::ExpectException<InvalidChessCaseException>([] { Notation corner1and2{ 35, 64 }; });
		}

		TEST_METHOD(GetCoordinateAlgebraicNotationRandomCases)
		{
			Notation corner1and2{ 23, 34 };
			Notation corner3and4{ 11, 14 };

			Assert::IsTrue(corner1and2.getCoordinateAlgebraicNotation() == "h3c5");
			Assert::IsTrue(corner3and4.getCoordinateAlgebraicNotation() == "d2g2");
		}

		

	};
}