#include "Elements.h"

namespace gui {

void Border::Draw(Window* w, Rect b) {
	int x0 = b.Left(); int x1 = b.Right();
	int y0 = b.Top(); int y1 = b.Bottom();

	w->SetChar(x0, y0, L'\x250F', mColor);
	w->SetChar(x1, y0, L'\x2513', mColor);
	w->SetChar(x0, y1, L'\x2517', mColor);
	w->SetChar(x1, y1, L'\x251B', mColor);

	for (int x = x0 + 1; x < x1; x++) {
		w->SetChar(x, y0, L'\x2501', mColor);
		w->SetChar(x, y1, L'\x2501', mColor);
	}

	for (int y = y0 + 1; y < y1; y++) {
		w->SetChar(x0, y, L'\x2503', mColor);
		w->SetChar(x1, y, L'\x2503', mColor);
	}
}

void TitledBorder::Draw(Window* w, Rect b) {
	Border::Draw(w, b);
	int tW = mTitle.length();
	int x0 = b.origin.x + ((b.size.width - tW) / 2) + 1;
	w->SetChar(x0 - 1, b.origin.y, L'\x252B', mColor);
	w->WriteString(x0, b.origin.y, mTitle, mColor);
	w->SetChar(x0 + tW, b.origin.y, L'\x2523', mColor);
}

void Element::Draw(Window* w) {
	w->DrawRect(mBounds, L' ', mBackgroundColor, true);
	mBorder->Draw(w, mBounds);
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
	int borderWidth = mBorder->GetEnabled();
	RenderText(w, {
		mBounds.origin.x + borderWidth,
		mBounds.origin.y + borderWidth,
		mBounds.size.width - 2 * borderWidth,
		mBounds.size.height - 2 * borderWidth,
		}, mText, mTextColor);
}

void Button::Init() {
	AddEventHandler(new EventHandler(
		[this](Window* w, int m) { if (m & mButtons) mPressed = true; },
		[this](Window* w, int m) { if (m & mButtons) mPressed = false; },
		nullptr, nullptr, nullptr
	));
}

void Button::Draw(Window* w) {
	WORD bgC = mPressed ? mPressedBackgroundColor : mBackgroundColor;
	WORD tC = mPressed ? mPressedTextColor : mTextColor;

	w->DrawRect(mBounds, L' ', bgC, true);
	if (mPressed) mPressedBorder->Draw(w, mBounds);
	else mBorder->Draw(w, mBounds);
	int borderWidth = (mPressed ? mPressedBorder->GetEnabled() : mBorder->GetEnabled());
	RenderText(w, {
		mBounds.origin.x + borderWidth,
		mBounds.origin.y + borderWidth,
		mBounds.size.width - 2 * borderWidth,
		mBounds.size.height - 2 * borderWidth,
		}, mText, tC);
}

void TextField::Init() {
	AddEventHandler(new EventHandler(
		[this](Window* w, int m) {
			w->SetFocusedElement(this);
			mDisabled = false;
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

void TextField::Draw(Window* w) {
	WORD bgC = mDisabled ? mDisabledBackgroundColor : mBackgroundColor;
	WORD tC = mDisabled ? mDisabledTextColor : mTextColor;
	
	w->DrawRect(mBounds, L' ', bgC, true);
	if (mDisabled) mDisabledBorder->Draw(w, mBounds);
	else mBorder->Draw(w, mBounds);

	int borderWidth = (mDisabled ? mDisabledBorder->GetEnabled() : mBorder->GetEnabled());
	RenderText(w, {
		mBounds.origin.x + borderWidth,
		mBounds.origin.y + borderWidth,
		mBounds.size.width - 2 * borderWidth,
		mBounds.size.height - 2 * borderWidth,
	}, mText, tC);
}

void Checkbox::Init() {
	AddEventHandler(new EventHandler(
		[this](Window* w, int m) { mChecked = !mChecked; },
		nullptr, nullptr, nullptr, nullptr
	));
}

void Checkbox::Draw(Window* w) {
	Element::Draw(w);
	int borderWidth = mBorder->GetEnabled();
	RenderText(w, {
		mBounds.origin.x + borderWidth,
		mBounds.origin.y + borderWidth,
		3,
		1,
		}, mChecked ? L"[\x25CF]" : L"[ ]", mTextColor);
	RenderText(w, {
		mBounds.origin.x + borderWidth + 4,
		mBounds.origin.y + borderWidth,
		mBounds.size.width - 2 * borderWidth - 4,
		mBounds.size.height - 2 * borderWidth,
		}, mText, mTextColor);
}

}