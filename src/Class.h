#pragma once

#include <math.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <set>
#include <vector>
#include <unordered_set>
using namespace std;
double constexpr EPS = 1e-10;
int constexpr MAX_INTERSECT = 5000000;

inline constexpr int sign(const int x) {
	return x < 0 ? -1 : 1;
}


class Line
{
public:
	char m_type;
	int m_x1, m_y1, m_x2, m_y2;
	int m_xdiff, m_ydiff;
	int m_maxx, m_minx, m_maxy, m_miny;
	long long m_det;
	Line(char type, int x1, int y1, int x2, int y2) : m_type(type), m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2)
	{
		m_xdiff = x1 - x2;
		m_ydiff = y1 - y2;
		if (type == 'S')
		{
			m_maxx = max(x1, x2);
			m_minx = min(x1, x2);
			m_maxy = max(y1, y2);
			m_miny = min(y1, y2);
		}
		m_det = (long long)x1 * y2 - (long long)x2 * y1;
	}

	inline bool online(double x, double y)
	{
		bool online;
		if (m_type == 'L')
		{
			online = true;
		}
		else if (m_type == 'R')
		{
			online = m_xdiff * (m_x1 - x) + m_ydiff * (m_y1 - y) >= -EPS;
		}
		else if (m_type == 'S')
		{
			online = m_minx <= x && x <= m_maxx &&
				m_miny <= y && y <= m_maxy;
		}
		return online;
	}
};


class Circle
{
public:
	int m_x, m_y, m_r;
	long long m_r2;
	Circle(int x, int y, int r) : m_x(x), m_y(y), m_r(r)
	{
		m_r2 = (long long)r * r;
	}
};


class Node
{
public:
	double m_x, m_y;
	Node(double x, double y) : m_x(x), m_y(y) {}

	inline bool operator< (const Node& node) const
	{
		if (*this == node)
		{
			return false;
		}
		if (fabs(m_x - node.m_x) <= EPS)
		{
			return m_y - node.m_y < EPS;
		}
		return m_x - node.m_x < EPS;
	}

	bool operator==(const Node& node) const
	{
		return fabs(m_x - node.m_x) <= EPS && fabs(m_y - node.m_y) <= EPS;
	}
};
namespace std
{
	template <>
	struct hash<Node>
	{
		size_t operator()(const Node& obj) const
		{
			return hash<double>()(round(pow(obj.m_x, 3) + pow(obj.m_y, 3)) * 1.0 / EPS);
		}
	};
}


class Intersect
{
public:
	Intersect(string inputFile)
	{
		char type;
		int N;
		Line* newLine;
		Circle* newCircle;
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
				newLine = new Line(type, x1, y1, x2, y2);
				m_allLines.push_back(newLine);
				break;
			}
			case 'C':
			{
				int x, y, r;

				ifile >> x >> y >> r;
				newCircle = new Circle(x, y, r);
				m_allCircles.push_back(newCircle);
			}
			default:
				break;
			}
		}
		ifile.close();
	}

	void CalculateIntersections()
	{
		m_allIntersections.reserve(5000000);

		int i, j;
		int lineNum = m_allLines.size();
		int circleNum = m_allCircles.size();
		Line *lineOut, *lineIn;
		Circle *circleOut, *circleIn;
		
		// line-line and line-circle
		for (i = 0; i < lineNum; ++i)
		{
			lineOut = m_allLines[i];
			for (j = i + 1; j < lineNum; ++j)
			{
				lineIn = m_allLines[j];
				LineLineIntersect(lineOut, lineIn);
				//if (test.size() > MAX_INTERSECT)
				//{
				//	Filter();
				//}
			}
			for (j = 0; j < circleNum; ++j)
			{
				circleIn = m_allCircles[j];
				LineCircleIntersect(lineOut, circleIn);
				//if (test.size() > MAX_INTERSECT)
				//{
				//	Filter();
				//}
			}
		}

		// circle-circle
		for (i = 0; i < circleNum; ++i)
		{
			circleOut = m_allCircles[i];
			for (j = i + 1; j < circleNum; ++j)
			{
				circleIn = m_allCircles[j];
				CircleCircleIntersect(circleOut, circleIn);
			}
			//if (test.size() > MAX_INTERSECT)
			//{
			//	Filter();
			//}
		}
		// Filter();
		return;

	}

	int GetIntersectionNumber()
	{ 
		return m_allIntersections.size();
		// return test.size();
	}

	void ViewIntersections()
	{
		//Node* curIntersection;
		//unordered_set<Node*, NodePointerHasher, NodePointerComparator>::iterator it;
		//for (it = m_allIntersections.begin(); it != m_allIntersections.end(); it++)
		//{
		//	curIntersection = *it;
		//	cout << curIntersection->m_x << " " << curIntersection->m_y << endl;
		//}
		
		for (int i = 0; i < test.size(); ++i)
		{
			cout << test[i].m_x << " " << test[i].m_y << endl;
		}
		return;
	}

private:
	vector<Line*> m_allLines;
	vector<Circle*> m_allCircles;
	unordered_set<Node> m_allIntersections;
	vector<Node> test;

	void LineLineIntersect(Line* line1, Line* line2)
	{
		int line1_xdiff = line1->m_xdiff;
		int line1_ydiff = line1->m_ydiff;
		int line2_xdiff = line2->m_xdiff;
		int line2_ydiff = line2->m_ydiff;

		// intermediate result type follows the first argument
		long long den = (long long)line1_xdiff * line2_ydiff - (long long)line2_xdiff * line1_ydiff;
		if (den != 0)
		{
			long long line1_det = line1->m_det;
			long long line2_det = line2->m_det;

			long long xmol = line1_det * line2_xdiff - line2_det * line1_xdiff;
			long long ymol = line1_det * line2_ydiff - line2_det * line1_ydiff;
			double x = (double)xmol / den;
			double y = (double)ymol / den;
			
			if (line1->online(x, y) && line2->online(x, y))
			{
				m_allIntersections.emplace(x, y);
				// test.emplace_back(x, y);
			}
		}
		return;
	}

	void LineCircleIntersect(Line* line, Circle* circle)
	{
		int line_xdiff = line->m_xdiff;
		int line_ydiff = line->m_ydiff;
		long long line_dis2 = line_xdiff * line_xdiff + line_ydiff * line_ydiff;
		
		int circle_x = circle->m_x;
		int circle_y = circle->m_y;
		long long circle_r2 = circle->m_r2;

		int dx1 = line->m_x1 - circle_x;
		int dx2 = line->m_x2 - circle_x;
		int dy1 = line->m_y1 - circle_y;
		int dy2 = line->m_y2 - circle_y;
		long long det = (long long)dx1 * dy2 - (long long)dx2 * dy1;

		long long delta = circle_r2 * line_dis2 - det * det;
		if (!(delta < 0))
		{
			if (delta == 0)
			{
				double x = (double)(det * -line_ydiff) / line_dis2 + circle_x;
				double y = (double)(det * line_xdiff) / line_dis2 + circle_y;
				
				if (line->online(x, y))
				{
					m_allIntersections.emplace(x, y);
					// test.emplace_back(x, y);
				}
			}
			else
			{
				double sqrt_delta = sqrt(delta);
				double x1 = (det * -line_ydiff + sign(-line_ydiff) * -line_xdiff * sqrt_delta) / line_dis2 + circle_x;
				double y1 = (det * line_xdiff + abs(line_ydiff) * sqrt_delta) / line_dis2 + circle_y;

				if (line->online(x1, y1))
				{
					m_allIntersections.emplace(x1, y1);
					// test.emplace_back(x1, y1);
				}

				double x2 = (det * -line_ydiff - sign(-line_ydiff) * -line_xdiff * sqrt_delta) / line_dis2 + circle_x;
				double y2 = (det * line_xdiff - abs(line_ydiff) * sqrt_delta) / line_dis2 + circle_y;

				if (line->online(x2, y2))
				{
					m_allIntersections.emplace(x2, y2);
					// test.emplace_back(x2, y2);
				}
			}
		}
		return;
	}

	void CircleCircleIntersect(Circle* circle1, Circle* circle2)
	{
		int circle1_x = circle1->m_x;
		int circle1_y = circle1->m_y;
		int circle1_r = circle1->m_r;
		long long circle1_r2 = circle1->m_r2;
		int circle2_x = circle2->m_x;
		int circle2_y = circle2->m_y;
		int circle2_r = circle2->m_r;
		long long circle2_r2 = circle2->m_r2;

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
				// test.emplace_back(x, y);
			}
			else
			{
				double x1 = x + h * (circle2_y - circle1_y) / dis;
				double y1 = y - h * (circle2_x - circle1_x) / dis;

				m_allIntersections.emplace(x1, y1);
				// est.emplace_back(x1, y1);

				double x2 = x - h * (circle2_y - circle1_y) / dis;
				double y2 = y + h * (circle2_x - circle1_x) / dis;

				m_allIntersections.emplace(x2, y2);
				// test.emplace_back(x2, y2);
			}
		}
		return;
	}

	void Filter()
	{
		sort(test.begin(), test.end());
		vector<Node>::iterator newEnd = unique(test.begin(), test.end());
		test.erase(newEnd, test.end());
		return;
	}
};