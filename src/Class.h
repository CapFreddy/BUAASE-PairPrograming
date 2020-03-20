#include <math.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <set>
#include <vector>
#include <unordered_set>
using namespace std;
double constexpr EPS = 1e-10;


class GeoObject
{
public:
	char m_type;
	GeoObject(char type) : m_type(type) {}
};

class Circle : public GeoObject
{
public:
	long long m_x, m_y, m_r;
	Circle(char type, int x, int y, int r) : GeoObject(type), m_x(x), m_y(y), m_r(r) {}
};

class Line : public GeoObject
{
public:
	long long m_x1, m_y1, m_x2, m_y2;
	long long m_xdiff, m_ydiff, m_det;
	Line(char type, int x1, int y1, int x2, int y2) : GeoObject(type), m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2)
	{
		m_xdiff = m_x1 - m_x2;
		m_ydiff = m_y1 - m_y2;
		m_det = m_x1 * m_y2 - m_x2 * m_y1;
	}
};


class Node
{
public:
	double m_x, m_y;
	Node(double x, double y) : m_x(x), m_y(y) {}

	bool online(Line* line)
	{
		bool online;
		if (line->m_type == 'L')
		{
			online = true;
		}
		else if (line->m_type == 'R')
		{
			online = line->m_xdiff * (line->m_x1 - m_x) + line->m_ydiff * (line->m_y1 - m_y) >= -EPS;
		}
		else if (line->m_type == 'S')
		{
			online = min(line->m_x1, line->m_x2) <= m_x && m_x <= max(line->m_x1, line->m_x2) &&
				min(line->m_y1, line->m_y2) <= m_y && m_y <= max(line->m_y1, line->m_y2);
		}
		return online;
	}
};

struct NodePointerHasher
{
	size_t operator()(const Node* node) const noexcept
	{
		return hash<double>{}(round((node->m_x + node->m_y) * 1.0 / EPS));
	}
};

struct NodePointerComparator
{
	bool operator()(const Node* node1, const Node* node2) const noexcept
	{
		return fabs(node1->m_x - node2->m_x) <= EPS && fabs(node1->m_y - node2->m_y) <= EPS;
	}
};

class Intersect
{
public:
	Intersect(string inputFile)
	{
		char type;
		int N;
		GeoObject* newGeoObject;
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
				newGeoObject = new Line(type, x1, y1, x2, y2);
				m_allLines.push_back(newGeoObject);
				break;
			}
			case 'C':
			{
				int x, y, r;

				ifile >> x >> y >> r;
				newGeoObject = new Circle(type, x, y, r);
				m_allCircles.push_back(newGeoObject);
			}
			default:
				break;
			}
			m_allGeoObjects.push_back(newGeoObject);
		}
		ifile.close();
	}

	void CalculateIntersections()
	{
		int i, j;
		int lineNum = m_allLines.size();
		int circleNum = m_allCircles.size();
		GeoObject *lineOut, *lineIn, *circleOut, *circleIn;
		
		// line-line and line-circle
		for (i = 0; i < lineNum; i++)
		{
			lineOut = m_allLines[i];
			for (j = i + 1; j < lineNum; j++)
			{
				lineIn = m_allLines[j];
				LineLineIntersect(lineOut, lineIn);
			}
			for (j = 0; j < circleNum; j++)
			{
				circleIn = m_allCircles[j];
				LineCircleIntersect(lineOut, circleIn);
			}
		}

		// circle-circle
		for (i = 0; i < circleNum; i++)
		{
			circleOut = m_allCircles[i];
			for (j = i + 1; j < circleNum; j++)
			{
				circleIn = m_allCircles[j];
				CircleCircleIntersect(circleOut, circleIn);
			}
		}
		return;

	}

	int GetIntersectionNumber() { return m_allIntersections.size(); }

	void ViewIntersections()
	{
		Node* curIntersection;
		unordered_set<Node*, NodePointerHasher, NodePointerComparator>::iterator it;
		for (it = m_allIntersections.begin(); it != m_allIntersections.end(); it++)
		{
			curIntersection = *it;
			cout << curIntersection->m_x << " " << curIntersection->m_y << endl;
		}
		return;
	}

private:
	vector<GeoObject*> m_allLines;
	vector<GeoObject*> m_allCircles;
	vector<GeoObject*> m_allGeoObjects;
	unordered_set<Node*, NodePointerHasher, NodePointerComparator> m_allIntersections;

	void LineLineIntersect(GeoObject* _line1, GeoObject* _line2)
	{
		Line *line1 = (Line*)_line1, *line2 = (Line*)_line2;

		long long den = line1->m_xdiff * line2->m_ydiff - line1->m_ydiff * line2->m_xdiff;
		if (den != 0)
		{
			long long xmol = line1->m_det * line2->m_xdiff - line1->m_xdiff * line2->m_det;
			long long ymol = line1->m_det * line2->m_ydiff - line1->m_ydiff * line2->m_det;
			double x = (double)xmol / (double)den;
			double y = (double)ymol / (double)den;
			Node* intersection = new Node(x, y);
			if (intersection->online(line1) && intersection->online(line2))
			{
				m_allIntersections.insert(intersection);
			}
		}
		return;
	}

	void LineCircleIntersect(GeoObject* _line, GeoObject* _circle)
	{
		Line* line = (Line*)_line;
		Circle* circle = (Circle*)_circle;

		double dis = sqrt(line->m_xdiff * line->m_xdiff + line->m_ydiff * line->m_ydiff);
		long long det = (line->m_x1 - circle->m_x) * (line->m_y2 - circle->m_y) - (line->m_x2 - circle->m_x) * (line->m_y1 - circle->m_y);
		double delta = circle->m_r * circle->m_r * dis * dis - det * det;
		if (!(delta < -EPS))
		{
			// delta >= 0
			if (fabs(delta) <= EPS)
			{
				// delta == 0
				double x = (det * -line->m_ydiff) / (dis * dis) + circle->m_x;
				double y = (det * line->m_xdiff) / (dis * dis) + circle->m_y;
				Node* intersection = new Node(x, y);
				if (intersection->online(line))
				{
					m_allIntersections.insert(intersection);
				}
			}
			else
			{
				// delta > 0
				double x1 = (det * -line->m_ydiff + copysign(1, -line->m_ydiff) * -line->m_xdiff * sqrt(delta)) / (dis * dis) + circle->m_x;
				double y1 = (det * line->m_xdiff + fabs(line->m_ydiff) * sqrt(delta)) / (dis * dis) + circle->m_y;
				Node* intersection = new Node(x1, y1);
				if (intersection->online(line))
				{
					m_allIntersections.insert(intersection);
				}

				double x2 = (det * -line->m_ydiff - copysign(1, -line->m_ydiff) * -line->m_xdiff * sqrt(delta)) / (dis * dis) + circle->m_x;
				double y2 = (det * line->m_xdiff - fabs(line->m_ydiff) * sqrt(delta)) / (dis *  dis) + circle->m_y;
				intersection = new Node(x2, y2);
				if (intersection->online(line))
				{
					m_allIntersections.insert(intersection);
				}
			}
		}
		return;
	}

	void CircleCircleIntersect(GeoObject* _circle1, GeoObject* _circle2)
	{
		Circle *circle1 = (Circle*)_circle1, *circle2 = (Circle*)_circle2;

		double dis = sqrt((circle1->m_x - circle2->m_x) * (circle1->m_x - circle2->m_x) + (circle1->m_y - circle2->m_y) * (circle1->m_y - circle2->m_y));
		if (!(circle1->m_r + circle2->m_r < dis || fabs(circle1->m_r - circle2->m_r) > dis))
		{
			double a = (circle1->m_r * circle1->m_r - circle2->m_r * circle2->m_r + dis * dis) / (2 * dis);
			double h = sqrt(circle1->m_r * circle1->m_r - a * a);
			double x = circle1->m_x + a * (circle2->m_x - circle1->m_x) / dis;
			double y = circle1->m_y + a * (circle2->m_y - circle1->m_y) / dis;

			if (fabs(h) < EPS)
			{
				Node* intersection = new Node(x, y);
				m_allIntersections.insert(intersection);
			}
			else
			{
				double x1 = x + h * (circle2->m_y - circle1->m_y) / dis;
				double y1 = y - h * (circle2->m_x - circle1->m_x) / dis;
				Node* intersection = new Node(x1, y1);
				m_allIntersections.insert(intersection);

				double x2 = x - h * (circle2->m_y - circle1->m_y) / dis;
				double y2 = y + h * (circle2->m_x - circle1->m_x) / dis;
				intersection = new Node(x2, y2);
				m_allIntersections.insert(intersection);
			}
		}
		return;
	}
};