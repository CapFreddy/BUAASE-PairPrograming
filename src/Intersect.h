#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "GeoObject.h"


int constexpr MAX_INTERSECT = 5000000;


class Intersect
{
public:
	Intersect()
	{
		m_allIntersections.reserve(MAX_INTERSECT);
	}

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
			}
			default:
				break;
			}
		}
		ifile.close();
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
				// test.emplace_back(x, y);
			}
		}
		return;
	}

	void LineCircleIntersect(Line line, Circle circle)
	{
		int line_xdiff = line.m_xdiff;
		int line_ydiff = line.m_ydiff;
		long long line_dis2 = line_xdiff * line_xdiff + line_ydiff * line_ydiff;

		int circle_x = circle.m_x;
		int circle_y = circle.m_y;
		long long circle_r2 = circle.m_r2;

		int dx1 = line.m_x1 - circle_x;
		int dx2 = line.m_x2 - circle_x;
		int dy1 = line.m_y1 - circle_y;
		int dy2 = line.m_y2 - circle_y;
		long long det = (long long)dx1 * dy2 - (long long)dx2 * dy1;

		long long delta = circle_r2 * line_dis2 - det * det;
		if (!(delta < 0))
		{
			if (delta == 0)
			{
				double x = (double)(det * -line_ydiff) / line_dis2 + circle_x;
				double y = (double)(det * line_xdiff) / line_dis2 + circle_y;

				if (line.online(x, y))
				{
					m_allIntersections.emplace(x, y);
				}
			}
			else
			{
				double sqrt_delta = sqrt(delta);
				double x1 = (det * -line_ydiff + sign(-line_ydiff) * -line_xdiff * sqrt_delta) / line_dis2 + circle_x;
				double y1 = (det * line_xdiff + abs(line_ydiff) * sqrt_delta) / line_dis2 + circle_y;

				if (line.online(x1, y1))
				{
					m_allIntersections.emplace(x1, y1);
				}

				double x2 = (det * -line_ydiff - sign(-line_ydiff) * -line_xdiff * sqrt_delta) / line_dis2 + circle_x;
				double y2 = (det * line_xdiff - abs(line_ydiff) * sqrt_delta) / line_dis2 + circle_y;

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
		int circle1_x = circle1.m_x;
		int circle1_y = circle1.m_y;
		int circle1_r = circle1.m_r;
		long long circle1_r2 = circle1.m_r2;
		int circle2_x = circle2.m_x;
		int circle2_y = circle2.m_y;
		int circle2_r = circle2.m_r;
		long long circle2_r2 = circle2.m_r2;

		long long dis2 = (circle1_x - circle2_x) * (circle1_x - circle2_x) + (circle1_y - circle2_y) * (circle1_y - circle2_y);
		double dis = sqrt((double)dis2);
		if (!(circle1_r + circle2_r < dis || abs(circle1_r - circle2_r) > dis))
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
};