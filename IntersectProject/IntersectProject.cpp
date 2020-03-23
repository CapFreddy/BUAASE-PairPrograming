#include "Intersect.h"
#include "Exception.h"


int main(int argc, char* argv[])
{

	try
	{
		string inputFile, outputFile;

		for (int i = 1; i < argc; i++)
		{
			if (string(argv[i]) == "-i")
			{
				inputFile = string(argv[++i]);
			}
			else if (string(argv[i]) == "-o")
			{
				outputFile = string(argv[++i]);
			}
			else
			{
				throw Exception(CMDFormatException,
					"cmd instruction should only contain -i or -o as parameter lead.");
			}
		}

		Intersect intersect;
		intersect.AddGeometryObjectFromFile(inputFile);
		intersect.CalculateIntersections();

		ofstream ofs;
		ofs.open(outputFile, ios::out);
		ofs << intersect.GetIntersectionNumber();
		ofs.close();
	}
	catch (Exception e)
	{
		e.print();
	}
	return 0;
}