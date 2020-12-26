#pragma once

namespace gui {

class Point {
public:
	int x, y;
	Point() : x(0), y(0) {}
	Point(int x, int y) : x(x), y(y) {}

	Point& operator+=(const Point& p) { x += p.x; y += p.y; return *this; }
	Point& operator-=(const Point& p) { x -= p.x; y -= p.y; return *this; }
};

inline bool operator==(const Point& a, const Point& b) { return a.x == b.x && a.y == b.y; }
inline bool operator!=(const Point& a, const Point& b) { return !operator==(a, b); }
inline Point operator+(Point a, const Point& b) { a += b; return a; }
inline Point operator-(Point a, const Point& b) { a -= b; return a; }

class Rect {
public:
	int x, y, width, height;

	Rect() : x(0), y(0), width(0), height(0) {}
	Rect(int x, int y) : x(x), y(y), width(0), height(0) {}
	Rect(int x, int y, int w, int h) : x(x), y(y), width(w), height(h) {}
	
	int Left() const { return x; }
	int Right() const { return x + width - 1; }
	int Top() const { return y; }
	int Bottom() const { return y + height - 1; }

	Point Min() const { return {x, y}; }
	Point Center() const { return { x + width / 2, y + height / 2 }; }
	Point Max() const { return { Right(), Bottom() }; }

	bool Contains(const Point& p) const {
		return (
			p.x >= Left() &&
			p.x <= Right() &&
			p.y >= Top() &&
			p.y <= Bottom()
		);
	}

};

}