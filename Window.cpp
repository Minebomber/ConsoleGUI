#include "Window.h"

namespace gui {

WindowScheme* WindowScheme::Default() {
	return new WindowScheme(
		FG_WHITE, BG_BLACK, FG_WHITE,
		FG_DARK_GREY, BG_BLACK, FG_DARK_GREY,
		FG_GREY, BG_BLACK, FG_GREY, 
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

void Window::Rect(RECT r, WCHAR chr, WORD clr, bool fill) {
	for (int i = r.top; i <= r.bottom; i++) {
		for (int j = r.left; j <= r.right; j++) {
			if (fill) Set(j, i, chr, clr);
			else if (i == r.top || i == r.bottom || j == r.left || j == r.right) Set(j, i, chr, clr);
		}
	}
}

void Window::Write(int x, int y, std::wstring str, WORD clr) {
	for (size_t i = 0; i < str.size(); i++) {
		mBuffer[y * mWidth + x + i].Char.UnicodeChar = str[i];
		mBuffer[y * mWidth + x + i].Attributes = clr;
	}
}

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

	if (mColorScheme) {
		e->SetBackgroundColor(mColorScheme->GetBackgroundColor());
		e->GetBorder()->SetColor(mColorScheme->GetBorderColor());
		e->GetBorder()->SetWidth(mColorScheme->GetBorderWidth());

		if (auto lab = dynamic_cast<Label*>(e)) {
			lab->SetTextColor(mColorScheme->GetTextColor());
		}

		if (auto btn = dynamic_cast<Button*>(e)) {
			btn->SetPressedTextColor(mColorScheme->GetPressedTextColor());
			btn->SetPressedBackgroundColor(mColorScheme->GetPressedBackgroundColor());
			btn->GetPressedBorder()->SetColor(mColorScheme->GetPressedBorderColor());
			btn->GetPressedBorder()->SetWidth(mColorScheme->GetPressedBorderWidth());
		}

		if (auto tf = dynamic_cast<TextField*>(e)) {
			tf->SetDisabledTextColor(mColorScheme->GetDisabledTextColor());
			tf->SetDisabledBackgroundColor(mColorScheme->GetDisabledBackgroundColor());
			tf->GetDisabledBorder()->SetColor(mColorScheme->GetDisabledBorderColor());
			tf->GetDisabledBorder()->SetWidth(mColorScheme->GetDisabledBorderWidth());
		}

		if (auto lab = dynamic_cast<Label*>(e)) {
			lab->UpdateTextLines();
			lab->UpdateTextOffsetY();
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