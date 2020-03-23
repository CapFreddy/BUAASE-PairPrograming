#pragma once
#include <vector>
#include <iostream>
using namespace std;
typedef pair<double, double> Point;


__declspec(dllexport) void readFile(string);

__declspec(dllexport) void addGeometryObject(string);

__declspec(dllexport) void removeGeometryObject(string);

__declspec(dllexport) pair<vector<string>, vector<Point>> getResult();
