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

	Rect& AlignLeftToLeft(const Rect& r, int offset = 0) { 
		mX = r.Left() + offset; 
		return *this; 
	}

	Rect& AlignLeftToRight(const Rect& r, int offset = 0) {
		mX = r.Right() + offset;
		return *this;
	}

	Rect& AlignTopToTop(const Rect& r, int offset = 0) {
		mY = r.Top() + offset;
		return *this;
	}
	
	Rect& AlignTopToBottom(const Rect& r, int offset = 0) {
		mY = r.Bottom() + offset;
		return *this;
	}

	Rect& AlignRightToRight(const Rect& r, int offset = 0) {
		mX += r.Right() - Right() + offset;
		return *this;
	}

	Rect& AlignRightToLeft(const Rect& r, int offset = 0) {
		mX += r.Left() - Right() + offset;
		return *this;
	}

	Rect& AlignBottomToBottom(const Rect& r, int offset = 0) {
		mY += r.Bottom() - Bottom() + offset;
		return *this;
	}

	Rect& AlignBottomToTop(const Rect& r, int offset = 0) {
		mY += r.Top() - Bottom() + offset;
		return *this;
	}

	Rect& CenterHorizontalWith(const Rect& r, int offset = 0) {
		mX += r.Center().mX - Center().mX + offset;
		return *this;
	}
	Rect& CenterVerticalWith(const Rect& r, int offset = 0) {
		mY += r.Center().mY - Center().mY + offset;
		return *this;
	}

	Rect& CenterWith(const Rect& r, Point offset = { 0, 0 }) {
		mX += r.Center().mX - Center().mX + offset.mX;
		mY += r.Center().mY - Center().mY + offset.mY;
		return *this;
	}
	
};

}