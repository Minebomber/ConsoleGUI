#pragma once

namespace gui {

class Point {
	friend class Rect;
private:
	int mX, mY;
public:
	Point() : mX(0), mY(0) {}
	Point(int x, int y) : mX(x), mY(y) {}

	const int& GetX() const { return mX; }
	void SetX(int x) { mX = x; }

	const int& GetY() const { return mY; }
	void SetY(int y) { mY = y; }

	bool operator==(const Point& p) const { return mX == p.mX && mY == p.mY; }
	bool operator!=(const Point& p) const { return !(*this == p); }
};

class Rect {
private:
	int mX, mY, mWidth, mHeight;
public:
	Rect() : mX(0), mY(0), mWidth(0), mHeight(0) {}
	Rect(int x, int y) : mX(x), mY(y), mWidth(0), mHeight(0) {}
	Rect(int x, int y, int w, int h) : mX(x), mY(y), mWidth(w), mHeight(h) {}

	const int& GetX() const { return mX; }
	void SetX(int x) { mX = x; }

	const int& GetY() const { return mY; }
	void SetY(int y) { mY = y; }

	const int& GetWidth() const { return mWidth; }
	void SetWidth(int w) { mWidth = w; }

	const int& GetHeight() const { return mHeight; }
	void SetHeight(int h) { mHeight = h; }

	int Left() const { return mX; }
	int Right() const { return mX + mWidth - 1; }
	int Top() const { return mY; }
	int Bottom() const { return mY + mHeight - 1; }

	Point Center() const { return { mX + mWidth / 2, mY + mHeight / 2 }; }
	Point Max() const { return { Right(), Bottom() }; }

	bool Contains(const Point& p) const {
		return (
			p.GetX() >= Left() &&
			p.GetX() <= Right() &&
			p.GetY() >= Top() &&
			p.GetY() <= Bottom()
		);
	}
};

}