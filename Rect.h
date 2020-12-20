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

	void AlignLeftToLeft(const Rect& r, int offset = 0) { 
		x = r.Left() + offset;  
	}

	void AlignLeftToRight(const Rect& r, int offset = 0) {
		x = r.Right() + offset;
	}

	void AlignTopToTop(const Rect& r, int offset = 0) {
		y = r.Top() + offset;
	}
	
	void AlignTopToBottom(const Rect& r, int offset = 0) {
		y = r.Bottom() + offset;
	}

	void AlignRightToRight(const Rect& r, int offset = 0) {
		x += r.Right() - Right() + offset;
	}

	void AlignRightToLeft(const Rect& r, int offset = 0) {
		x += r.Left() - Right() + offset;
	}

	void AlignBottomToBottom(const Rect& r, int offset = 0) {
		y += r.Bottom() - Bottom() + offset;
	}

	void AlignBottomToTop(const Rect& r, int offset = 0) {
		y += r.Top() - Bottom() + offset;
	}

	void CenterHorizontalWith(const Rect& r, int offset = 0) {
		x += r.Center().x - Center().x + offset;
	}
	void CenterVerticalWith(const Rect& r, int offset = 0) {
		y += r.Center().y - Center().y + offset;
	}

	void CenterWith(const Rect& r, Point offset = { 0, 0 }) {
		x += r.Center().x - Center().x + offset.x;
		y += r.Center().y - Center().y + offset.y;
	}
	
};

}