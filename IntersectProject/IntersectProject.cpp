#include "Intersect.h"


int main(int argc, char* argv[])
{

	string inputFile, outputFile;

	for (int i = 0; i < argc; i++)
	{
		if (string(argv[i]) == "-i")
		{
			inputFile = string(argv[i + 1]);
		}
		else if (string(argv[i]) == "-o")
		{
			outputFile = string(argv[i + 1]);
		}
	}
	
	Intersect intersect;
	intersect.AddGeometryObjectFromFile(inputFile);
	intersect.CalculateIntersections();

	ofstream ofs;
	ofs.open(outputFile, ios::out);
	ofs << intersect.GetIntersectionNumber();
	ofs.close();

	return 0;
}