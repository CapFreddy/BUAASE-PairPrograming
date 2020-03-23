#pragma once
#include <math.h>
#include <algorithm>
using namespace std;
double constexpr EPS = 1e-10;


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

	inline bool operator== (const Line& line) const
	{
		return m_type == line.m_type && m_x1 == line.m_x1 && m_y1 == line.m_y1 && m_x2 == line.m_x2 && m_y2 == line.m_y2;
	}

	inline bool online(double x, double y)
	{
		switch (m_type)
		{
		case 'L':
			return true;
		case 'R':
			return m_xdiff * (m_x1 - x) + m_ydiff * (m_y1 - y) >= -EPS;
		case 'S':
			return (m_minx - x <= EPS && x - m_maxx <= EPS) && (m_miny - y <= EPS && y - m_maxy <= EPS);
		default:
			break;
		}
		return false;
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

	inline bool operator== (const Circle& circle) const
	{
		return m_x == circle.m_x && m_y == circle.m_y && m_r == circle.m_r;
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

	inline bool operator== (const Node& node) const
	{
		return fabs(m_x - node.m_x) <= EPS && fabs(m_y - node.m_y) <= EPS;
	}
};

namespace std
{
	template <>
	struct hash<Node>

	{
		size_t operator() (const Node& obj) const
		{
			return hash<double>{}(round(obj.m_x * 1.0 / EPS)) ^ (hash<double>{}(round(obj.m_y * 1.0 / EPS)) << 1);
		}
	};
}
