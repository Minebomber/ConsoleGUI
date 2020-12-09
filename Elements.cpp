#include "Elements.h"

namespace gui {

void Border::Draw(Window* c, Bounds b) {

	for (int wi = 0; wi < mWidth; wi++) {
		int x0 = b.origin.x + wi; int x1 = b.origin.x + b.size.width - wi - 1;
		int y0 = b.origin.y + wi; int y1 = b.origin.y + b.size.height - wi - 1;

		c->Set(x0, y0, L'\x250F', mColor);
		c->Set(x1, y0, L'\x2513', mColor);
		c->Set(x0, y1, L'\x2517', mColor);
		c->Set(x1, y1, L'\x251B', mColor);

		for (int x = x0 + 1; x < x1; x++) {
			c->Set(x, y0, L'\x2501', mColor);
			c->Set(x, y1, L'\x2501', mColor);
		}

		for (int y = y0 + 1; y < y1; y++) {
			c->Set(x0, y, L'\x2503', mColor);
			c->Set(x1, y, L'\x2503', mColor);
		}
	}
}

void TitledBorder::Draw(Window* c, Bounds b) {
	Border::Draw(c, b);
	int tW = mTitle.length();
	int x0 = b.origin.x + ((b.size.width - tW) / 2) + 1;
	c->Set(x0 - 1, b.origin.y, L'\x252B', mColor);
	c->Write(x0, b.origin.y, mTitle, mColor);
	c->Set(x0 + tW, b.origin.y, L'\x2523', mColor);
}

void Element::Draw(Window* c) {
	c->Rect(mBounds, L' ', mBackgroundColor, true);
	mBorder->Draw(c, mBounds);
}

void Label::RenderText(Window* c, Bounds b, const std::wstring& s, WORD cl) {
	int textWidth = b.size.width;
	int textHeight = b.size.height;

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
						auto spaceIdx = std::distance(s.begin(), iSpace.base()) - 1;
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

	for (int i = 0, y = b.origin.y + yOffset; i < currentLine; i++, y++)
		c->Write(b.origin.x + lines[i].dX, y, s.substr(lines[i].sI, lines[i].lW), cl);

	delete[] lines;
}

void Label::Draw(Window* c) {
	Element::Draw(c);
	int borderWidth = mBorder->GetWidth();
	RenderText(c, {
		mBounds.origin.x + borderWidth,
		mBounds.origin.y + borderWidth,
		mBounds.size.width - 2 * borderWidth,
		mBounds.size.height - 2 * borderWidth,
		}, mText, mTextColor);
}

void Button::SetupHandlers() {
	mMouseHandler = new MouseHandler(mBounds, MOUSE_LEFT_BUTTON);
	mMouseHandler->SetPressAction([this](Window* c, int m) { mPressed = true; if (mPressAction) mPressAction(m); });
	mMouseHandler->SetReleaseAction([this](Window* c, int m) { mPressed = false; if (mReleaseAction) mReleaseAction(m); });
}

void Button::Draw(Window* c) {
	WORD bgC = mPressed ? mPressedBackgroundColor : mBackgroundColor;
	WORD tC = mPressed ? mPressedTextColor : mTextColor;

	c->Rect(mBounds, L' ', bgC, true);
	if (mPressed) mPressedBorder->Draw(c, mBounds);
	else mBorder->Draw(c, mBounds);
	int borderWidth = (mPressed ? mPressedBorder->GetWidth() : mBorder->GetWidth());
	RenderText(c, {
		mBounds.origin.x + borderWidth,
		mBounds.origin.y + borderWidth,
		mBounds.size.width - 2 * borderWidth,
		mBounds.size.height - 2 * borderWidth,
		}, mText, tC);
}

void TextField::SetupHandlers() {

	mMouseHandler = new MouseHandler(mBounds, MOUSE_LEFT_BUTTON);

	mMouseHandler->SetPressAction([this](Window* c, int m) {
		c->SetActiveKeyboardHandler(mKeyboardHandler);
		mDisabled = false;
	});

	mKeyboardHandler = new KeyboardHandler(
		L"\x08\x0D\x10\xBA\xBB\xBC\xBD\xBE\xBF\xC0\xDB\xDC\xDD\xDE" + mCharset
	);

	mKeyboardHandler->SetPressAction([this](Window* c, int k) {
		if (k == VK_SHIFT) mCapitalize = true;
		else if (k == VK_BACK) {
			if (mText.length() > 0) mText = mText.substr(0, mText.length() - 1);
		} else if (k == VK_RETURN) {
			mText += L'\n';
		} else {
			// Get text for vk
			UINT sc = MapVirtualKey(k, MAPVK_VK_TO_VSC);
			static byte b[256];
			GetKeyboardState(b);
			if (mCapitalize) b[0x10] = 0x80;
			WCHAR c;
			int r = ToUnicode((UINT)k, sc, b, &c, 1, 0);
			mText += c;
		}
	});

	mKeyboardHandler->SetReleaseAction([this](Window* c, int k) {
		if (k == VK_SHIFT) mCapitalize = false;
	});
}

void TextField::Draw(Window* c) {
	WORD bgC = mDisabled ? mDisabledBackgroundColor : mBackgroundColor;
	WORD tC = mDisabled ? mDisabledTextColor : mTextColor;

	c->Rect(mBounds, L' ', bgC, true);
	if (mDisabled) mDisabledBorder->Draw(c, mBounds);
	else mBorder->Draw(c, mBounds);

	int borderWidth = (mDisabled ? mDisabledBorder->GetWidth() : mBorder->GetWidth());
	RenderText(c, {
		mBounds.origin.x + borderWidth,
		mBounds.origin.y + borderWidth,
		mBounds.size.width - 2 * borderWidth,
		mBounds.size.height - 2 * borderWidth,
	}, mText, tC);

	/*RenderText(c,
		mBounds.origin.x + (mDisabled ? mDisabledBorder->GetWidth() : mBorder->GetWidth()),
		mBounds.origin.x + mBounds.size.width - (mDisabled ? mDisabledBorder->GetWidth() : mBorder->GetWidth()),
		mBounds.origin.y + (mDisabled ? mDisabledBorder->GetWidth() : mBorder->GetWidth()),
		mBounds.origin.y + mBounds.size.height - (mDisabled ? mDisabledBorder->GetWidth() : mBorder->GetWidth()),
		mText,
		tC
	);*/
}

void Panel::Draw(Window* c) {
	Element::Draw(c);
	mTitleLabel.Draw(c);
}

void ContentPanel::Draw(Window* c) {
	Panel::Draw(c);
	if (mContent) mContent->Draw(c);
}

}