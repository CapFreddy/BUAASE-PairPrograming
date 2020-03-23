#pragma once
#include <vector>
#include <iostream>
using namespace std;
typedef pair<double, double> Point;


// parameter = file_name
__declspec(dllexport) void readFile(string);

// add from standard string fromat
__declspec(dllexport) void addGeometryObject(string);

// remove by standard string format, strictly equals
__declspec(dllexport) void removeGeometryObject(string);

// trigger calculation
__declspec(dllexport) pair<vector<string>, vector<Point>> getResult();
