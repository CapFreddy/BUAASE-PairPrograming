#include "interface.h"
#include "../IntersectProject/Intersect.h"


Intersect intersect;


void readFile(string file)
{
	intersect.AddGeometryObjectFromFile(file);
	return;
}


void addGeometryObject(string geoObject)
{
	intersect.AddGeometryObjectByString(geoObject);
	return;
}


void removeGeometryObject(string geoObject)
{
	intersect.clear();
	intersect.RemoveGeometryObjectByString(geoObject);
	return;
}


pair<vector<string>, vector<Point>> getResult()
{
	intersect.CalculateIntersections();
	return pair<vector<string>, vector<Point>>(intersect.GetGeoObjects(), intersect.GetIntersections());
}
