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
		
		TEST_METHOD(TestGetCoordinateAlgebraicNotationCorners)
		{
			//Test all corners (limit case)
			Notation corner1and2{ 0, 7 };
			Notation corner3and4{ 56, 63 };

			Assert::IsTrue(corner1and2.GetCoordinateAlgebraicNotation() == "a1h1");
			Assert::IsTrue(corner3and4.GetCoordinateAlgebraicNotation() == "a8h8");
		}

		TEST_METHOD(TestGetCoordinateAlgebraicNotationOutBound)
		{
			//Outbound position should fail at construction
			Assert::ExpectException<InvalidChessCaseException>([] { Notation corner1and2{ -1, 23 }; });
			Assert::ExpectException<InvalidChessCaseException>([] { Notation corner1and2{ 35, 64 }; });
		}

		TEST_METHOD(TestGetCoordinateAlgebraicNotationRandomCases)
		{
			Notation corner1and2{ 23, 34 };
			Notation corner3and4{ 11, 14 };

			Assert::IsTrue(corner1and2.GetCoordinateAlgebraicNotation() == "h3c5");
			Assert::IsTrue(corner3and4.GetCoordinateAlgebraicNotation() == "d2g2");
		}

		

	};
}