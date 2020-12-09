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

	bool Contains(const Point& p) const {
		return (
			p.x >= origin.x &&
			p.x < origin.x + size.width &&
			p.y >= origin.y &&
			p.y < origin.y + size.height
			);
	}
};