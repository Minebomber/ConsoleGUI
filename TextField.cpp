#include "TextField.h"

namespace gui {

TextField::TextField(Rect b) : Label(b) { mAlignH = TEXT_ALIGN_MIN; mAlignV = TEXT_ALIGN_MIN; Init(); }
TextField::TextField(Rect b, int m) : Label(b), mMode(m) { mAlignH = TEXT_ALIGN_MIN; mAlignV = TEXT_ALIGN_MIN; Init(); }
TextField::TextField(const TextField& e) : Label(e), mMode(e.mMode) { Init(); }

bool TextField::ValidKeyForMode(int k) {
	if (mMode == TEXT_MODE_ALL) return true;

	if (k == 0x08) return true;

	if (TEXT_MODE_NUMBERS & mMode)
		if (k >= '0' && k <= '9') return true;

	if (TEXT_MODE_ALPHABET & mMode)
		if ((k >= 'A' && k <= 'Z') || k == ' ' || k == 0x10 || k == 0x0D) return true;

	if (TEXT_MODE_SPECIAL & mMode)
		if (!(k >= 'A' && k <= 'Z') && !(k >= '0' && k <= '9')) return true;

	return false;
}

void TextField::Init() {
	EventHandler* h = new EventHandler();
	h->SetMouseDownAction([this](Window* w, int m) {
		w->SetFocusedElement(this);
		mState = ELEMENT_FOCUSED;

		if (!mCursorFlashFuture.valid() || mCursorFlashFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
			mCursorFlashFuture = std::async(std::launch::async, std::bind(&TextField::FlashCursor, this));
		}
	});

	h->SetKeyDownAction([this](Window* w, int k) {
		if (ValidKeyForMode(k)) {
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
		}
	});

	h->SetKeyUpAction([this](Window* w, int k) {
		if (ValidKeyForMode(k)) {
			if (k == VK_SHIFT) mCapitalize = false;
		}
	});

	AddEventHandler(h);
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
	if (mMode & TEXT_MODE_SECURE) {
		std::wstring text = std::wstring(mText.length(), L'*');
		RenderText(
			w,
			GetInnerBounds(),
			text + (mState == ELEMENT_FOCUSED && mShowCursor ? L"_" : L""),
			GetCurrentForegroundColor()
		);
	} else
		RenderText(
			w,
			GetInnerBounds(),
			mText + (mState == ELEMENT_FOCUSED && mShowCursor ? L"_" : L""),
			GetCurrentForegroundColor()
		);
}

}