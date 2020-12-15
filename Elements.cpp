#include "Elements.h"

namespace gui {

WORD Element::GetCurrentForegroundColor() const {
	switch (mState) {
	case ELEMENT_FOCUSED:
		return mFocusedForegroundColor;
	case ELEMENT_DISABLED:
		return mDisabledForegroundColor;
	default:
		return mDefaultForegroundColor;
	}
}

WORD Element::GetCurrentBackgroundColor() const {
	switch (mState) {
	case ELEMENT_FOCUSED:
		return mFocusedBackgroundColor;
	case ELEMENT_DISABLED:
		return mDisabledBackgroundColor;
	default:
		return mDefaultBackgroundColor;
	}
}

Rect Element::GetInnerBounds() const {
	return {
		mBounds.origin.x + mDisplayBorders,
		mBounds.origin.y + mDisplayBorders,
		mBounds.size.width - (2 * mDisplayBorders),
		mBounds.size.height - (2 * mDisplayBorders)
	};
}

void Element::Draw(Window* w) {
	w->DrawRect(mBounds, L' ', GetCurrentBackgroundColor(), true);
	
	if (mDisplayBorders) {
		int x0 = mBounds.Left(); int x1 = mBounds.Right();
		int y0 = mBounds.Top(); int y1 = mBounds.Bottom();
		WORD cl = GetCurrentForegroundColor();

		w->SetChar(x0, y0, L'\x250F', cl);
		w->SetChar(x1, y0, L'\x2513', cl);
		w->SetChar(x0, y1, L'\x2517', cl);
		w->SetChar(x1, y1, L'\x251B', cl);

		for (int x = x0 + 1; x < x1; x++) {
			w->SetChar(x, y0, L'\x2501', cl);
			w->SetChar(x, y1, L'\x2501', cl);
		}

		for (int y = y0 + 1; y < y1; y++) {
			w->SetChar(x0, y, L'\x2503', cl);
			w->SetChar(x1, y, L'\x2503', cl);
		}
	}
}

void Label::RenderText(Window* c, Rect r, const std::wstring& s, WORD cl) {
	int textWidth = r.size.width;
	int textHeight = r.size.height;

	struct LineInfo {
		int dX, sI, lW;
	};
	LineInfo* lines = new LineInfo[textHeight];
	int currentLine = 0;

	int lineBeginIdx = 0;
	int currentIdx = 0;
	while (currentIdx < s.length() && currentLine < textHeight) {
		int lineWidth = currentIdx - lineBeginIdx + 1;
		// Adjust line spacing x
		int xOffset = 0;
		if (mAlignH == TEXT_ALIGN_MID) xOffset = (textWidth - lineWidth) / 2;
		else if (mAlignH == TEXT_ALIGN_MAX)  xOffset = textWidth - lineWidth;

		if (lineWidth >= textWidth) {
			// line too long
			if (mTextWrap == TEXT_WRAP_WORD) {
				// find space
				auto iStart = s.rbegin() + (s.length() - currentIdx - 1);
				auto iEnd = s.rbegin() + (s.length() - lineBeginIdx);
				auto iSpace = std::find(iStart, iEnd, L' ');

				if (iSpace != iEnd) {
					// if end of string, print line as is
					// else print until space, newline there
					if (currentIdx == s.length() - 1) {
						lines[currentLine] = { xOffset, lineBeginIdx, lineWidth }; currentLine++;
					} else {
						int spaceIdx = std::distance(s.begin(), iSpace.base()) - 1;
						lineWidth = spaceIdx - lineBeginIdx;
						if (mAlignH == TEXT_ALIGN_MID) xOffset = (textWidth - lineWidth) / 2;
						else if (mAlignH == TEXT_ALIGN_MAX)  xOffset = textWidth - lineWidth;

						lines[currentLine] = { xOffset, lineBeginIdx, lineWidth }; currentLine++;
						lineBeginIdx = spaceIdx + 1;
					}
				} else {
					// char wrap
					// display line from linebegin->current
					// move to next line
					lines[currentLine] = { xOffset, lineBeginIdx, lineWidth }; currentLine++;
					lineBeginIdx = currentIdx + 1;
				}
			} else {
				// char wrap
					// display line from linebegin->current
				// move to next line
				lines[currentLine] = { xOffset, lineBeginIdx, lineWidth }; currentLine++;
				lineBeginIdx = currentIdx + 1;
			}
		} else if (s[currentIdx] == L'\n') {
			// break on newline
			// dont want to include \n, print linewidth - 1
			lineWidth--;
			if (mAlignH == TEXT_ALIGN_MID) xOffset = (textWidth - lineWidth) / 2;
			else if (mAlignH == TEXT_ALIGN_MAX)  xOffset = textWidth - lineWidth;

			lines[currentLine] = { xOffset, lineBeginIdx, lineWidth }; currentLine++;
			// move to next line
			lineBeginIdx = currentIdx + 1;
		} else if (currentIdx == s.length() - 1) {
			// display to end
			// same line, no wrapping
			lines[currentLine] = { xOffset, lineBeginIdx, lineWidth }; currentLine++;
		}
		currentIdx++;
	}

	int yOffset = 0;
	if (mAlignV == TEXT_ALIGN_MID) yOffset = (textHeight - currentLine) / 2;
	else if (mAlignV == TEXT_ALIGN_MAX) yOffset = textHeight - currentLine;
	for (int i = 0; i < currentLine; i++)
		c->WriteString(r.origin.x + lines[i].dX, r.origin.y + yOffset + i, s, cl, lines[i].sI, lines[i].lW);

	delete[] lines;
}

void Label::Draw(Window* w) {
	Element::Draw(w);
	RenderText(w, GetInnerBounds(), mText, GetCurrentForegroundColor());
}

void Button::Init() {
	AddEventHandler(new EventHandler(
		[this](Window* w, int m) { if (m & mButtons) mState = ELEMENT_FOCUSED; },
		[this](Window* w, int m) { if (m & mButtons) mState = ELEMENT_DEFAULT; },
		nullptr, nullptr, nullptr
	));
}

void Button::Draw(Window* w) {
	Label::Draw(w);
}

void TextField::Init() {
	AddEventHandler(new EventHandler(
		[this](Window* w, int m) {
			w->SetFocusedElement(this);
			mState = ELEMENT_FOCUSED;

			if (!mCursorFlashFuture.valid() || mCursorFlashFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
				mCursorFlashFuture = ExecuteAsync(std::bind(&TextField::FlashCursor, this));
			}
		},
		nullptr, nullptr,
		[this](Window* w, int k) {
			if (k == VK_SHIFT) mCapitalize = true;
			else if (k == VK_BACK) {
				if (mText.length() > 0) mText = mText.substr(0, mText.length() - 1);
			} else if (k == VK_RETURN) mText += L'\n';
			else {
				UINT sc = MapVirtualKey(k, MAPVK_VK_TO_VSC);
				static byte b[256];
				GetKeyboardState(b);
				if (mCapitalize) b[0x10] = 0x80;
				WCHAR c;
				if (ToUnicode((UINT)k, sc, b, &c, 1, 0)) mText += c;
			}
		},
		[this](Window* w, int k) {
			if (k == VK_SHIFT) mCapitalize = false;
		}
	));
}

void TextField::FlashCursor() {
	mShowCursor = false;
	while (mState == ELEMENT_FOCUSED) {
		mShowCursor = !mShowCursor;
		std::this_thread::sleep_for(std::chrono::milliseconds(350));
	}
}

void TextField::Draw(Window* w) {
	Element::Draw(w);
	RenderText(
		w, 
		GetInnerBounds(), 
		mText + (mState == ELEMENT_FOCUSED && mShowCursor ? L"_" : L""), 
		GetCurrentForegroundColor()
	);
}

void Checkbox::Init() {
	AddEventHandler(new EventHandler(
		[this](Window* w, int m) { mState = ELEMENT_FOCUSED; },
		[this](Window* w, int m) { mState = ELEMENT_DEFAULT; mChecked = !mChecked; },
		nullptr, nullptr, nullptr
	));
}

void Checkbox::Draw(Window* w) {
	Element::Draw(w);

	Rect textBounds = GetInnerBounds();
	WORD cl = GetCurrentForegroundColor();
	w->WriteString(textBounds.origin.x, textBounds.origin.y, mChecked ? L"[\x25CF]" : L"[ ]", cl);

	textBounds.origin.x += 4;
	textBounds.size.width -= 4;
	RenderText(w, textBounds, mText, cl);
}

}