#include "Window.h"

namespace gui {

Window::Window(int w, int h, ElementStyle* defStyle) : mWidth(w), mHeight(h), 
mBuffer(new CHAR_INFO[w * h]), mStyleMap({ {std::type_index(typeid(Element)), defStyle} }) {}


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
	for (int i = 0; i < r.GetHeight(); i++) {
		for (int j = 0; j < r.GetWidth(); j++) {
			if (fill) SetChar(j + r.GetX(), i + r.GetY(), chr, clr);
			else if (i == 0 || i == r.GetHeight() - 1 || j == 0 || j == r.GetWidth() - 1) 
				SetChar(j + r.GetX(), i + r.GetY(), chr, clr);
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

void Window::AddElement(Element* e, bool applyStyle) {
	mElements.push_back(e);

	if (applyStyle) {
		if (ElementStyle* s = GetStyle(*e)) {
			e->SetDefaultForeground(s->GetDefaultForeground());
			e->SetDefaultBackground(s->GetDefaultBackground());

			e->SetFocusedForeground(s->GetFocusedForeground());
			e->SetFocusedBackground(s->GetFocusedBackground());

			e->SetDisabledForeground(s->GetDisabledForeground());
			e->SetDisabledBackground(s->GetDisabledBackground());

			e->SetBorders(s->GetBorders());
		}
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
}

}