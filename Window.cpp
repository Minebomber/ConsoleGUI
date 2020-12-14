#include "Window.h"

namespace gui {

WindowScheme* WindowScheme::Default() {
	return new WindowScheme(
		FG_GREY, BG_BLACK,
		FG_WHITE, BG_BLACK,
		FG_DARK_GREY, BG_BLACK,
		true 
	);
}

WindowScheme* WindowScheme::Green() {
	return new WindowScheme(
		FG_DARK_GREEN, BG_BLACK,
		FG_GREEN, BG_BLACK,
		FG_DARK_GREEN, BG_BLACK,
		true 
	);
}

WindowScheme* WindowScheme::Red() {
	return new WindowScheme(
		FG_DARK_RED, BG_BLACK,
		FG_RED, BG_BLACK,
		FG_DARK_RED, BG_BLACK,
		true
	);
}

void Window::SetChar(int x, int y, WCHAR chr, WORD clr) {
	mBuffer[y * mWidth + x].Char.UnicodeChar = chr;
	mBuffer[y * mWidth + x].Attributes = clr;
}

void Window::FillScreen(WCHAR chr, WORD clr) {
	for (int i = 0; i < mWidth * mHeight; i++) {
		mBuffer[i].Char.UnicodeChar = chr;
		mBuffer[i].Attributes = clr;
	}
}

void Window::DrawRect(Rect r, WCHAR chr, WORD clr, bool fill) {
	for (int i = 0; i < r.size.height; i++) {
		for (int j = 0; j < r.size.width; j++) {
			if (fill) SetChar(j + r.origin.x, i + r.origin.y, chr, clr);
			else if (i == 0 || i == r.size.height - 1 || j == 0 || j == r.size.width - 1) 
				SetChar(j + r.origin.x, i + r.origin.y, chr, clr);
		}
	}
}

void Window::WriteString(int x, int y, const std::wstring& str, WORD clr) {
	for (int i = 0; i < str.size(); i++) {
		mBuffer[y * mWidth + x + i].Char.UnicodeChar = std::max(str[i], L' ');
		mBuffer[y * mWidth + x + i].Attributes = clr;
	}
}

void Window::WriteString(int x, int y, const std::wstring& str, WORD clr, int st, int w) {
	for (int i = 0; i < w; i++) {
		mBuffer[y * mWidth + x + i].Char.UnicodeChar = std::max(str[st + i], L' ');
		mBuffer[y * mWidth + x + i].Attributes = clr;
	}
}

const Point& Window::GetMousePosition() const { return mMousePosition; }

void Window::AddElement(Element* e) {
	mElements.push_back(e);

	if (mScheme) {
		e->SetDefaultForegroundColor(mScheme->GetDefaultForeground());
		e->SetDefaultBackgroundColor(mScheme->GetDefaultBackground());

		e->SetFocusedForegroundColor(mScheme->GetFocusedForeground());
		e->SetFocusedBackgroundColor(mScheme->GetFocusedBackground());

		e->SetDisabledForegroundColor(mScheme->GetDisabledForeground());
		e->SetDisabledBackgroundColor(mScheme->GetDisabledBackground());
	}
}

void Window::RemoveElement(Element* e) { 
	if (mFocusedElement == e) mFocusedElement = nullptr; 
	mElements.erase(std::remove(mElements.begin(), mElements.end(), e), mElements.end()); 
}

Element* Window::GetElementAtPoint(const Point& p) {
	Element* r = nullptr;
	for (Element* e : mElements) if (e->mBounds.Contains(p)) r = e;
	return r;
}

void Window::Display() {
	FillScreen(mBaseChar, mBaseColor);
	for (Element* e : mElements) {
		e->Draw(this);
	}
}

Window::~Window() {
	for (Element* e : mElements) delete e;
	delete[] mBuffer;
	if (mScheme) delete mScheme;
}

}