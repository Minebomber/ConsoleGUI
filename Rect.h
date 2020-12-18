#pragma once

namespace gui {

class Point {
public:
	int x, y;
	Point() : x(0), y(0) {}
	Point(int x, int y) : x(x), y(y) {}

	bool operator==(const Point& p) const { return x == p.x && y == p.y; }
	bool operator!=(const Point& p) const { return !(*this == p); }
};

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

	Rect& AlignLeftToLeft(const Rect& r, int offset = 0) { 
		x = r.Left() + offset; 
		return *this; 
	}

	Rect& AlignLeftToRight(const Rect& r, int offset = 0) {
		x = r.Right() + offset;
		return *this;
	}

	Rect& AlignTopToTop(const Rect& r, int offset = 0) {
		y = r.Top() + offset;
		return *this;
	}
	
	Rect& AlignTopToBottom(const Rect& r, int offset = 0) {
		y = r.Bottom() + offset;
		return *this;
	}

	Rect& AlignRightToRight(const Rect& r, int offset = 0) {
		x += r.Right() - Right() + offset;
		return *this;
	}

	Rect& AlignRightToLeft(const Rect& r, int offset = 0) {
		x += r.Left() - Right() + offset;
		return *this;
	}

	Rect& AlignBottomToBottom(const Rect& r, int offset = 0) {
		y += r.Bottom() - Bottom() + offset;
		return *this;
	}

	Rect& AlignBottomToTop(const Rect& r, int offset = 0) {
		y += r.Top() - Bottom() + offset;
		return *this;
	}

	Rect& CenterHorizontalWith(const Rect& r, int offset = 0) {
		x += r.Center().x - Center().x + offset;
		return *this;
	}
	Rect& CenterVerticalWith(const Rect& r, int offset = 0) {
		y += r.Center().y - Center().y + offset;
		return *this;
	}

	Rect& CenterWith(const Rect& r, Point offset = { 0, 0 }) {
		x += r.Center().x - Center().x + offset.x;
		y += r.Center().y - Center().y + offset.y;
		return *this;
	}
	
};

}