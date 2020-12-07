#include "Elements.h"

namespace gui {

void Border::Draw(Console* c, RECT bd) {
	for (int b = 0; b < mWidth; b++) c->Rect({ bd.left + b, bd.top + b, bd.right - b, bd.bottom - b }, mChar, mColor, false);
}

void Element::Draw(Console* c) {
	c->Rect(mBounds, mBackground, mBackgroundColor, true);
	mBorder.Draw(c, mBounds);
}

void Label::UpdateTextLines() {
	mTextLines = 1;
	for (int ci = 0, li = 0; ci < (int)mText.length(); ci++) {
		if (mText[ci] == L'\n') { mTextLines++; li = ci; }
		if (ci - li > mBounds.right - mBounds.left - 2 * mBorder.GetWidth()) { mTextLines++; li = ci; }
	}
}

void Label::UpdateTextOffsetY() {
	mTextOffsetY = 0;
	if (mAlignV == TEXT_ALIGN_MID) mTextOffsetY = (mBounds.bottom - mBounds.top - 2 * mBorder.GetWidth() - mTextLines + 1) / 2;
	else if (mAlignV == TEXT_ALIGN_MAX) mTextOffsetY = mBounds.bottom - mBounds.top - 2 * mBorder.GetWidth() - mTextLines;
}

void Label::RenderText(Console* c, int minX, int maxX, int minY, int maxY, std::wstring s, WORD cl) {
	int spanX = maxX - minX;
	int y = minY + mTextOffsetY;
	size_t nlPos = s.find(L'\n');
	size_t cIdx = 0;
	while (cIdx < s.length() && y <= maxY) {
		int elIdx = (nlPos == std::string::npos) ? s.length() : nlPos;
		int lineLen = elIdx - cIdx;
		// Stay within bounds
		if (lineLen > spanX) {
			if (mTextWrap == WRAP_WORD) {
				// Last char is idx of space
				elIdx = s.rfind(L' ', cIdx + spanX);
				lineLen = elIdx - cIdx;
			} else if (mTextWrap == WRAP_CHAR || lineLen > spanX) { 
				// def to char wrap if no spac
				elIdx -= (lineLen - spanX);
				lineLen = spanX + 1;
			}
		}
		// Adjust line spacing x
		int xOffset = 0;
		if (mAlignH == TEXT_ALIGN_MID) xOffset = (spanX - lineLen + 1) / 2;
		else if (mAlignH == TEXT_ALIGN_MAX)  xOffset = spanX - lineLen + 1;

		c->Write(minX + xOffset, y, s.substr(cIdx, lineLen), cl);

		y++;
		cIdx = elIdx + 1;
		nlPos = s.find(L'\n', cIdx);
	}
}

void Label::Draw(Console* c) {
	Element::Draw(c);
	RenderText(
		c, 
		mBounds.left + mBorder.GetWidth(), 
		mBounds.right - mBorder.GetWidth(), 
		mBounds.top + mBorder.GetWidth(), 
		mBounds.bottom - mBorder.GetWidth(), 
		mText, 
		mTextColor
	);
}

void Button::SetupHandlers() {
	mMouseHandler = new MouseHandler(mBounds, MOUSE_LEFT_BUTTON);
	mMouseHandler->SetPressAction([this](Console* c, int m) { mPressed = true; if (mPressAction) mPressAction(m); });
	mMouseHandler->SetReleaseAction([this](Console* c, int m) { mPressed = false; if (mReleaseAction) mReleaseAction(m); });
}

void Button::Draw(Console* c) {
	WCHAR bg = mPressed ? mPressedBackground : mBackground;
	WORD bgC = mPressed ? mPressedBackgroundColor : mBackgroundColor;
	WORD tC = mPressed ? mPressedTextColor : mTextColor;

	c->Rect(mBounds, bg, bgC, true);
	if (mPressed) mPressedBorder.Draw(c, mBounds);
	else mBorder.Draw(c, mBounds);

	RenderText(c,
		mBounds.left + (mPressed ? mPressedBorder.GetWidth() : mBorder.GetWidth()),
		mBounds.right - (mPressed ? mPressedBorder.GetWidth() : mBorder.GetWidth()),
		mBounds.top + (mPressed ? mPressedBorder.GetWidth() : mBorder.GetWidth()),
		mBounds.bottom - (mPressed ? mPressedBorder.GetWidth() : mBorder.GetWidth()),
		mText,
		tC
	);
}

void TextField::Backspace() {
	while (mDeleting && mText.length() > 0) {
		mText = mText.substr(0, mText.length() - 1);
		mNumDeleted++;
		std::this_thread::sleep_for(std::chrono::milliseconds(300 - std::min(250, mNumDeleted * 50)));
	}
}

void TextField::SetupHandlers() {

	mMouseHandler = new MouseHandler(mBounds, MOUSE_LEFT_BUTTON);

	mMouseHandler->SetPressAction([this](Console* c, int m) {
		c->SetActiveKeyboardHandler(mKeyboardHandler);
		mEnabled = true;
	});

	mKeyboardHandler = new KeyboardHandler(L"\x08\x0D\x10" + mCharset);

	mKeyboardHandler->SetPressAction([this](Console* c, int k) {
		if (k == VK_SHIFT) mCapitalize = true;
		else if (k == VK_BACK) {
			mDeleting = true;
			// Have only 1 thread at a time, create new only when exit
			if (!mDeleteFuture.valid() || mDeleteFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
				mDeleteFuture = ExecuteAsync(std::bind(&TextField::Backspace, this));
			}
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

	mKeyboardHandler->SetReleaseAction([this](Console* c, int k) {
		if (k == VK_SHIFT) mCapitalize = false;
		else if (k == VK_BACK) {
			mDeleting = false;
			mNumDeleted = 0;
		}
	});
}

void TextField::Draw(Console* c) {

	WCHAR bg = mEnabled ? mEnabledBackground : mBackground;
	WORD bgC = mEnabled ? mEnabledBackgroundColor : mBackgroundColor;
	WORD tC = mEnabled ? mEnabledTextColor : mTextColor;

	c->Rect(mBounds, bg, bgC, true);
	if (mEnabled) mEnabledBorder.Draw(c, mBounds);
	else mBorder.Draw(c, mBounds);

	RenderText(c,
		mBounds.left + (mEnabled ? mEnabledBorder.GetWidth() : mBorder.GetWidth()),
		mBounds.right - (mEnabled ? mEnabledBorder.GetWidth() : mBorder.GetWidth()),
		mBounds.top + (mEnabled ? mEnabledBorder.GetWidth() : mBorder.GetWidth()),
		mBounds.bottom - (mEnabled ? mEnabledBorder.GetWidth() : mBorder.GetWidth()),
		mText + (mEnabled ? L"_" : L""),
		tC
	);
}

void Panel::Draw(Console* c) {
	Element::Draw(c);
	mTitleLabel.Draw(c);
}

void ContentPanel::Draw(Console* c) {
	Panel::Draw(c);
	if (mContent) mContent->Draw(c);
}

}