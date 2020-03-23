#pragma once
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <strstream>
#include "GeoObject.h"
int constexpr MAX_INTERSECT = 5000000;
typedef pair<double, double> Point;
inline constexpr double sign(const double x) {
	return x < EPS ? -1 : 1;
}


class Intersect
{
public:
	void AddGeometryObjectFromFile(string inputFile)
	{
		char type;
		int N;
		ifstream ifile;

		ifile.open(inputFile, ios::in);
		ifile >> N;
		while (N--)
		{
			ifile >> type;
			switch (type)
			{
			case 'L': case 'R': case 'S':
			{
				int x1, y1, x2, y2;

				ifile >> x1 >> y1 >> x2 >> y2;
				m_allLines.emplace_back(type, x1, y1, x2, y2);
				break;
			}
			case 'C':
			{
				int x, y, r;

				ifile >> x >> y >> r;
				m_allCircles.emplace_back(x, y, r);
				break;
			}
			default:
				break;
			}
		}
		ifile.close();
	}

	void AddGeometryObjectByString(string geoObject)
	{
		char type;
		istrstream strin(geoObject.c_str());

		strin >> type;
		switch (type)
		{
		case 'L': case 'R': case 'S':
		{
			int x1, y1, x2, y2;

			strin >> x1 >> y1 >> x2 >> y2;
			m_allLines.emplace_back(type, x1, y1, x2, y2);
			break;
		}
		case 'C':
		{
			int x, y, r;

			strin >> x >> y >> r;
			m_allCircles.emplace_back(x, y, r);
			break;
		}
		default:
			break;
		}
		return;
	}

	void RemoveGeometryObjectByString(string geoObject)
	{
		char type;
		istrstream strin(geoObject.c_str());

		strin >> type;
		switch (type)
		{
		case 'L': case 'R': case 'S':
		{
			int x1, y1, x2, y2;
			
			strin >> x1 >> y1 >> x2 >> y2;
			Line removeLine(type, x1, y1, x2, y2);
			
			vector<Line>::iterator it = m_allLines.begin();
			while (it != m_allLines.end())
			{
				if (*it == removeLine)
				{
					it = m_allLines.erase(it);
					continue;
				}
				++it;
			}
			break;
		}
		case 'C':
		{
			int x, y, r;

			strin >> x >> y >> r;
			Circle removeCircle(x, y, r);

			vector<Circle>::iterator it = m_allCircles.begin();
			while (it != m_allCircles.end())
			{
				if (*it == removeCircle)
				{
					it = m_allCircles.erase(it);
					continue;
				}
			}
			break;
		}
		default:
			break;
		}
	}
	
	void CalculateIntersections()
	{
		int i, j;
		int lineNum = m_allLines.size();
		int circleNum = m_allCircles.size();

		// line-line and line-circle
		for (i = 0; i < lineNum; ++i)
		{
			for (j = i + 1; j < lineNum; ++j)
			{
				LineLineIntersect(m_allLines[i], m_allLines[j]);
			}
			for (j = 0; j < circleNum; ++j)
			{
				LineCircleIntersect(m_allLines[i], m_allCircles[j]);
			}
		}

		// circle-circle
		for (i = 0; i < circleNum; ++i)
		{
			for (j = i + 1; j < circleNum; ++j)
			{
				CircleCircleIntersect(m_allCircles[i], m_allCircles[j]);
			}
		}
		return;

	}

	int GetIntersectionNumber()
	{
		return m_allIntersections.size();
	}

	void ViewIntersections()
	{
		unordered_set<Node>::iterator it;
		for (it = m_allIntersections.begin(); it != m_allIntersections.end(); ++it)
		{
			cout << (*it).m_x << " " << (*it).m_y << endl;
		}
		return;
	}

	vector<string> GetGeoObjects()
	{
		vector<string> geoObjects;
		vector<Line>::iterator itLine;
		vector<Circle>::iterator itCircle;

		for (itLine = m_allLines.begin(); itLine != m_allLines.end(); ++itLine)
		{
			string geoObject;
			geoObject.push_back((*itLine).m_type);
			geoObject += " " +
				to_string((*itLine).m_x1) + " " + to_string((*itLine).m_y1) + " " +
				to_string((*itLine).m_x2) + " " + to_string((*itLine).m_y2);
			geoObjects.push_back(geoObject);
		}

		for (itCircle = m_allCircles.begin(); itCircle != m_allCircles.end(); ++itCircle)
		{
			string geoObject;
			geoObject.push_back('C');
			geoObject += " " +
				to_string((*itCircle).m_x) + " " + to_string((*itCircle).m_y) + " " + to_string((*itCircle).m_r);
			geoObjects.push_back(geoObject);
		}
		return geoObjects;
	}
	
	vector<Point> GetIntersections()
	{
		vector<Point> intersections;
		unordered_set<Node>::iterator it;

		for (it = m_allIntersections.begin(); it != m_allIntersections.end(); ++it)
		{
			Point curPoint((*it).m_x, (*it).m_y);
			intersections.push_back(curPoint);
		}
		return intersections;
	}

private:
	vector<Line> m_allLines;
	vector<Circle> m_allCircles;
	unordered_set<Node> m_allIntersections;

	void LineLineIntersect(Line line1, Line line2)
	{
		int line1_xdiff = line1.m_xdiff;
		int line1_ydiff = line1.m_ydiff;
		int line2_xdiff = line2.m_xdiff;
		int line2_ydiff = line2.m_ydiff;

		// intermediate result type follows the first argument
		long long den = (long long)line1_xdiff * line2_ydiff - (long long)line2_xdiff * line1_ydiff;
		if (den == 0 && line1.m_type != 'L' && line2.m_type != 'L')
		{
			int x1 = line1.m_x1;
			int y1 = line1.m_y1;
			int x2 = line1.m_x2;
			int y2 = line1.m_y2;
			
			if ((x1 == line2.m_x1 && y1 == line2.m_y1) ||
				(x1 == line2.m_x2 && y1 == line2.m_y2))
			{
				m_allIntersections.emplace(x1, y1);
			}
			else if ((x2 == line2.m_x1 && y2 == line2.m_y1) ||
				(x2 == line2.m_x2 && y2 == line2.m_y2))
			{
				m_allIntersections.emplace(x2, y2);
			}
		}
		if (den != 0)
		{
			long long line1_det = line1.m_det;
			long long line2_det = line2.m_det;

			long long xmol = line1_det * line2_xdiff - line2_det * line1_xdiff;
			long long ymol = line1_det * line2_ydiff - line2_det * line1_ydiff;
			double x = (double)xmol / den;
			double y = (double)ymol / den;

			if (line1.online(x, y) && line2.online(x, y))
			{
				m_allIntersections.emplace(x, y);
			}
		}
		return;
	}

	void LineCircleIntersect(Line line, Circle circle)
	{
		double line_xdiff = line.m_xdiff;
		double line_ydiff = line.m_ydiff;
		double line_dis2 = line_xdiff * line_xdiff + line_ydiff * line_ydiff;

		double circle_x = circle.m_x;
		double circle_y = circle.m_y;
		double circle_r2 = circle.m_r2;

		double dx1 = line.m_x1 - circle_x;
		double dx2 = line.m_x2 - circle_x;
		double dy1 = line.m_y1 - circle_y;
		double dy2 = line.m_y2 - circle_y;
		double det = dx1 * dy2 - dx2 * dy1;

		double delta = circle_r2 * line_dis2 - det * det;
		if (!(delta < 0))
		{
			if (delta == 0)
			{
				double x = (det * -line_ydiff) / line_dis2 + circle_x;
				double y = (det * line_xdiff) / line_dis2 + circle_y;

				if (line.online(x, y))
				{
					m_allIntersections.emplace(x, y);
				}
			}
			else
			{
				double sqrt_delta = sqrt(delta);
				double x1 = (det * -line_ydiff + sign(-line_ydiff) * -line_xdiff * sqrt_delta) / line_dis2 + circle_x;
				double y1 = (det * line_xdiff + fabs(line_ydiff) * sqrt_delta) / line_dis2 + circle_y;

				if (line.online(x1, y1))
				{
					m_allIntersections.emplace(x1, y1);
				}

				double x2 = (det * -line_ydiff - sign(-line_ydiff) * -line_xdiff * sqrt_delta) / line_dis2 + circle_x;
				double y2 = (det * line_xdiff - fabs(line_ydiff) * sqrt_delta) / line_dis2 + circle_y;

				if (line.online(x2, y2))
				{
					m_allIntersections.emplace(x2, y2);
				}
			}
		}
		return;
	}

	void CircleCircleIntersect(Circle circle1, Circle circle2)
	{
		double circle1_x = circle1.m_x;
		double circle1_y = circle1.m_y;
		double circle1_r = circle1.m_r;
		double circle1_r2 = circle1.m_r2;

		double circle2_x = circle2.m_x;
		double circle2_y = circle2.m_y;
		double circle2_r = circle2.m_r;
		double circle2_r2 = circle2.m_r2;

		double dis2 = (circle1_x - circle2_x) * (circle1_x - circle2_x) + (circle1_y - circle2_y) * (circle1_y - circle2_y);
		double dis = sqrt(dis2);
		if (!(circle1_r + circle2_r < dis || fabs(circle1_r - circle2_r) > dis))
		{
			double a = (circle1_r2 - circle2_r2 + dis2) / (2 * dis);
			double h = sqrt(circle1_r2 - a * a);
			double x = circle1_x + a * (circle2_x - circle1_x) / dis;
			double y = circle1_y + a * (circle2_y - circle1_y) / dis;

			if (fabs(h) < EPS)
			{
				m_allIntersections.emplace(x, y);
			}
			else
			{
				double x1 = x + h * (circle2_y - circle1_y) / dis;
				double y1 = y - h * (circle2_x - circle1_x) / dis;

				m_allIntersections.emplace(x1, y1);

				double x2 = x - h * (circle2_y - circle1_y) / dis;
				double y2 = y + h * (circle2_x - circle1_x) / dis;

				m_allIntersections.emplace(x2, y2);
			}
		}
		return;
	}
	
	inline bool LineLineOverlap(Line line1, Line line2)
	{

	}
};