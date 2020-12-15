#pragma once

namespace gui {

struct Point {
	int x, y;

	bool operator==(const Point& p) const { return x == p.x && y == p.y; }
	bool operator!=(const Point& p) const { return !(*this == p); }
};

struct Size {
	int width, height;
};

struct Rect {
	Point origin;
	Size size;

	int Left() const { return origin.x; }
	int Right() const { return origin.x + size.width - 1; }
	int Top() const { return origin.y; }
	int Bottom() const { return origin.y + size.height - 1; }

	Point Center() const { return { origin.x + size.width / 2, origin.y + size.height / 2 }; }
	Point Max() const { return { Right(), Bottom() }; }

	bool Contains(const Point& p) const {
		return (
			p.x >= origin.x &&
			p.x < origin.x + size.width &&
			p.y >= origin.y &&
			p.y < origin.y + size.height
			);
	}
};

}