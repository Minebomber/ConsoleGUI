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
	int left, top, right, bottom;

	Rect() : left(0), top(0), right(0), bottom(0) {}
	
	Rect(Point origin, int width, int height) : 
		left(origin.x), top(origin.y), right(origin.x + width - 1), bottom(origin.y + height - 1) {}
	
	Rect(int l, int t, int r, int b) : 
		left(l), top(t), right(r), bottom(b) {}
	
	int Width() const { return right - left + 1; }
	int Height() const { return bottom - top + 1; }

	void SetWidth(int w) { right = w + left - 1; }
	void SetHeight(int h) { bottom = h + top - 1; }

	Point Min() const { return { left, top }; }
	Point Center() const { return { left + Width() / 2, top + Height() / 2 }; }
	Point Max() const { return { right, bottom }; }

	bool Contains(const Point& p) const {
		return (
			p.x >= left &&
			p.x <= right &&
			p.y >= top &&
			p.y <= bottom
		);
	}

};

}