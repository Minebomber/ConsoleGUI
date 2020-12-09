#pragma once

struct Point {
	int x, y;
};

struct Size {
	int width, height;
};

struct Bounds {
	Point origin;
	Size size;

	int Left() { return origin.x; }
	int Right() { return origin.x + size.width - 1; }
	int Top() { return origin.y; }
	int Bottom() { return origin.y + size.height - 1; }

	Point Center() { return { origin.x + size.width / 2, origin.y + size.height / 2 }; }
	Point Max() { return { Right(), Bottom() }; }

	bool Contains(const Point& p) const {
		return (
			p.x >= origin.x &&
			p.x < origin.x + size.width &&
			p.y >= origin.y &&
			p.y < origin.y + size.height
		);
	}
};