#include "CppUnitTest.h"
#include "../IntersectProject/Intersect.h"
#include "../IntersectProjectDLL/Interface.h"
#pragma comment(lib, "..\\Debug\\IntersectProjectDLL.dll")
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(OldTest)
	{
	public:
		string testFolder = "../testcase/old/";
		TEST_METHOD(LineTest)
		{
			vector<int> results = { 0, 0, 1, 2, 3 };
			string lineFolder = testFolder + "line/";

			for (size_t i = 1; i <= results.size(); ++i)
			{
				string testfile = lineFolder + "line" + to_string(i) + ".txt";
				
				Intersect intersect;
				intersect.AddGeometryObjectFromFile(testfile);
				intersect.CalculateIntersections();
				
				Assert::AreEqual(intersect.GetIntersectionNumber(), results[i - 1]);
			}
		}

		TEST_METHOD(CircleTest)
		{
			vector<int> results = { 0, 0, 4, 8, 13 };
			string circleFolder = testFolder + "circle/";

			for (size_t i = 1; i <= results.size(); ++i)
			{
				string testfile = circleFolder + "circle" + to_string(i) + ".txt";

				Intersect intersect;
				intersect.AddGeometryObjectFromFile(testfile);
				intersect.CalculateIntersections();

				Assert::AreEqual(intersect.GetIntersectionNumber(), results[i - 1]);
			}
		}

		TEST_METHOD(CombinedTest)
		{
			vector<int> results = { 1, 2, 3, 4, 9 };
			string combinedFolder = testFolder + "combined/";

			for (size_t i = 1; i <= results.size(); ++i)
			{
				string testfile = combinedFolder + "combined" + to_string(i) + ".txt";

				Intersect intersect;
				intersect.AddGeometryObjectFromFile(testfile);
				intersect.CalculateIntersections();

				Assert::AreEqual(intersect.GetIntersectionNumber(), results[i - 1]);
			}
		}
	};

	TEST_CLASS(NewTest)
	{
		string testFolder = "../testcase/new/";

		TEST_METHOD(SegmentTest)
		{
			vector<int> results = { 0, 1, 2, 3, 4, 1 };
			string segmentFolder = testFolder + "segment/";

			for (size_t i = 1; i <= results.size(); ++i)
			{
				string testfile = segmentFolder + "segment" + to_string(i) + ".txt";

				Intersect intersect;
				intersect.AddGeometryObjectFromFile(testfile);
				intersect.CalculateIntersections();

				Assert::AreEqual(intersect.GetIntersectionNumber(), results[i - 1]);
			}
		}

		TEST_METHOD(RayTest)
		{
			vector<int> results = { 0, 1, 3, 1, 1 };
			string segmentFolder = testFolder + "ray/";

			for (size_t i = 1; i <= results.size(); ++i)
			{
				string testfile = segmentFolder + "ray" + to_string(i) + ".txt";

				Intersect intersect;
				intersect.AddGeometryObjectFromFile(testfile);
				intersect.CalculateIntersections();

				Assert::AreEqual(intersect.GetIntersectionNumber(), results[i - 1]);
			}
		}

		TEST_METHOD(ComprehensiveTest)
		{
			vector<int> results = { 6, 11, 13, 13 };
			string segmentFolder = testFolder + "comprehensive/";

			for (size_t i = 1; i <= results.size(); ++i)
			{
				string testfile = segmentFolder + "comprehensive" + to_string(i) + ".txt";

				Intersect intersect;
				intersect.AddGeometryObjectFromFile(testfile);
				intersect.CalculateIntersections();

				Assert::AreEqual(intersect.GetIntersectionNumber(), results[i - 1]);
			}
		}
	};

	TEST_CLASS(InterfaceTest)
	{
		TEST_METHOD(TestDLL)
		{
			string testFolder = "../testcase/dll/";
			string testFile = testFolder + "dll.txt";
			readFile(testFile);
			addGeometryObject("R 0 -1 0 -2");
			addGeometryObject("R -1 0 -2 0");
			Assert::AreEqual((int)(getResult().second).size(), 13);
			removeGeometryObject("C 0 2 2");
			Assert::AreEqual((int)(getResult().second).size(), 10);
		}
	};
}
