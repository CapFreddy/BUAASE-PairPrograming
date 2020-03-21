#pragma once
#include <math.h>
using namespace std;


double constexpr EPS = 1e-10;
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
		switch (m_type)
		{
		case 'L':
			return true;
		case 'R':
			return m_xdiff * (m_x1 - x) + m_ydiff * (m_y1 - y) >= -EPS;
		case 'S':
			return m_minx <= x && x <= m_maxx && m_miny <= y && y <= m_maxy;
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