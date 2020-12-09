#include "Window.h"

namespace gui {

WindowScheme* WindowScheme::Default() {
	return new WindowScheme(
		FG_WHITE, BG_BLACK, FG_WHITE,
		FG_DARK_GREY, BG_BLACK, FG_DARK_GREY,
		FG_DARK_GREY, BG_BLACK, FG_DARK_GREY,
		true 
	);
}

WindowScheme* WindowScheme::Green() {
	return new WindowScheme(
		FG_DARK_GREEN, BG_BLACK, FG_DARK_GREEN,
		FG_GREEN, BG_BLACK, FG_GREEN,
		FG_GREEN, BG_BLACK, FG_GREEN,
		true 
	);
}

WindowScheme* WindowScheme::Red() {
	return new WindowScheme(
		FG_DARK_RED, BG_BLACK, FG_DARK_RED,
		FG_RED, BG_BLACK, FG_RED,
		FG_RED, BG_BLACK, FG_RED,
		true
	);
}

void Window::Set(int x, int y, WCHAR chr, WORD clr) {
	mBuffer[y * mWidth + x].Char.UnicodeChar = chr;
	mBuffer[y * mWidth + x].Attributes = clr;
}

void Window::Fill(WCHAR chr, WORD clr) {
	for (int i = 0; i < mWidth * mHeight; i++) {
		mBuffer[i].Char.UnicodeChar = chr;
		mBuffer[i].Attributes = clr;
	}
}

void Window::Rect(Bounds b, WCHAR chr, WORD clr, bool fill) {
	for (int i = 0; i < b.size.height; i++) {
		for (int j = 0; j < b.size.width; j++) {
			if (fill) Set(j + b.origin.x, i + b.origin.y, chr, clr);
			else if (i == 0 || i == b.size.height - 1 || j == 0 || j == b.size.width - 1) 
				Set(j + b.origin.x, i + b.origin.y, chr, clr);
		}
	}
}

void Window::Write(int x, int y, std::wstring str, WORD clr) {
	for (size_t i = 0; i < str.size(); i++) {
		mBuffer[y * mWidth + x + i].Char.UnicodeChar = std::max(str[i], L' ');
		mBuffer[y * mWidth + x + i].Attributes = clr;
	}
}

const Point& Window::GetMousePosition() const { return mMousePosition; }

void Window::AddElement(Element* e) {
	if (e->GetId() != -1) return;
	for (size_t i = 0; i < mElements.size(); i++) {
		if (mElements.at(i) == nullptr) {
			e->mId = i;
			mElements.at(i) = e;
			return;
		}
	}
	mElements.push_back(e);
	e->mId = mElements.size() - 1;

	if (e->mMouseHandler) AddMouseHandler(e->mMouseHandler);

	if (mScheme) {
		e->SetBackgroundColor(mScheme->GetBackgroundColor());
		e->GetBorder()->SetColor(mScheme->GetBorderColor());
		e->GetBorder()->SetWidth(mScheme->GetBorderWidth());

		if (auto lab = dynamic_cast<Label*>(e)) {
			lab->SetTextColor(mScheme->GetTextColor());
		}

		if (auto btn = dynamic_cast<Button*>(e)) {
			btn->SetPressedTextColor(mScheme->GetPressedTextColor());
			btn->SetPressedBackgroundColor(mScheme->GetPressedBackgroundColor());
			btn->GetPressedBorder()->SetColor(mScheme->GetPressedBorderColor());
			btn->GetPressedBorder()->SetWidth(mScheme->GetPressedBorderWidth());
		}

		if (auto tf = dynamic_cast<TextField*>(e)) {
			tf->SetDisabledTextColor(mScheme->GetDisabledTextColor());
			tf->SetDisabledBackgroundColor(mScheme->GetDisabledBackgroundColor());
			tf->GetDisabledBorder()->SetColor(mScheme->GetDisabledBorderColor());
			tf->GetDisabledBorder()->SetWidth(mScheme->GetDisabledBorderWidth());
		}
	}
}

Element* Window::GetElement(int i) {
	return mElements.at(i);
}

Element* Window::GetElement(Element* e) {
	return GetElement(e->mId);
}

void Window::RemoveElement(int i) {
	if (mElements.at(i)->mMouseHandler) RemoveMouseHandler(mElements.at(i)->mMouseHandler);

	mElements.at(i)->mId = -1;
	mElements.at(i) = nullptr;
}

void Window::RemoveElement(Element* e) {
	RemoveElement(e->mId);
}

void Window::AddMouseHandler(MouseHandler* h) {
	if (h->GetId() != -1) return;
	for (size_t i = 0; i < mMouseHandlers.size(); i++) {
		if (mMouseHandlers.at(i) == nullptr) {
			h->mId = i;
			mMouseHandlers.at(i) = h;
			return;
		}
	}
	mMouseHandlers.push_back(h);
	h->mId = mMouseHandlers.size() - 1;
}

MouseHandler* Window::GetMouseHandler(int i) { return mMouseHandlers.at(i); }

MouseHandler* Window::GetMouseHandler(MouseHandler* h) { return GetMouseHandler(h->mId); }

void Window::RemoveMouseHandler(int i) {
	mMouseHandlers.at(i)->mId = -1;
	mMouseHandlers.at(i) = nullptr;
}

void Window::RemoveMouseHandler(MouseHandler* h) { RemoveMouseHandler(h->mId); }

void Window::Display() {
	Fill(mBaseChar, mBaseColor);
	for (Element* e : mElements) {
		e->Draw(this);
	}
}

}