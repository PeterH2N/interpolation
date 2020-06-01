#ifndef POINT_H
#define POINT_H
#include <ostream>
#include <iostream>

#include <vector>
#include <algorithm>
#include <memory>

struct Point
{
    double x, y;
};

inline bool operator < (Point a, Point b) { return a.x < b.x; }

inline std::ostream& operator << (std::ostream& os, const Point& p)
{
    return os << "(" << p.x << "," << p.y << ")";
}

static Point operator + (const Point & a, const Point & b)
{
	return { a.x + b.x, a.y + b.y };
}

static Point operator - (const Point & a, const Point & b)
{
	return { a.x - b.x, a.y - b.y };
}



#endif // POINT_H
