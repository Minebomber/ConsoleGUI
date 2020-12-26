#include "TextField.h"

namespace gui {

TextField::TextField(Rect b) : Label(b) { alignH = Alignment::Min; alignV = Alignment::Min; Init(); }
TextField::TextField(Rect b, TextMode::Mode m) : Label(b), mode(m) { alignH = Alignment::Min; alignV = Alignment::Min; Init(); }
TextField::TextField(const TextField& e) : Label(e), mode(e.mode) { Init(); }

bool TextField::ValidKeyForMode(int k) {
	if (mode == TextMode::All) return true;

	if (k == 0x08) return true;

	if (TextMode::Numbers & mode)
		if (k >= '0' && k <= '9') return true;

	if (TextMode::Alphabet & mode)
		if ((k >= 'A' && k <= 'Z') || k == ' ' || k == 0x10 || k == 0x0D) return true;

	if (TextMode::Special & mode)
		if (!(k >= 'A' && k <= 'Z') && !(k >= '0' && k <= '9')) return true;

	return false;
}

void TextField::Init() {
	AddEventHandler(EventHandler::New()->
		SetAction(EventType::MouseDown, [this](Window* w, int m) {
			w->focusedElement = this;
			state = State::Focused;

			if (!mCursorFlashFuture.valid() || mCursorFlashFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
				mCursorFlashFuture = std::async(std::launch::async, std::bind(&TextField::FlashCursor, this));
			}
		})->SetAction(EventType::KeyDown, [this](Window* w, int k) {
			if (ValidKeyForMode(k)) {
				if (k == VK_SHIFT) mCapitalize = true;
				else if (k == VK_BACK) {
					if (text.length() > 0) text = text.substr(0, text.length() - 1);
				} else if (k == VK_RETURN) text += L'\n';
				else {
					UINT sc = MapVirtualKey(k, MAPVK_VK_TO_VSC);
					static byte b[256];
					GetKeyboardState(b);
					if (mCapitalize) b[0x10] = 0x80;
					WCHAR c;
					if (ToUnicode((UINT)k, sc, b, &c, 1, 0)) text += c;
				}
			}
		})->SetAction(EventType::KeyUp, [this](Window* w, int k) {
			if (ValidKeyForMode(k)) {
				if (k == VK_SHIFT) mCapitalize = false;
			}
		})
	);
}

void TextField::FlashCursor() {
	mShowCursor = false;
	while (state == State::Focused) {
		mShowCursor = !mShowCursor;
		std::this_thread::sleep_for(std::chrono::milliseconds(350));
	}
}

void TextField::Draw(Window* w) {
	View::Draw(w);
	w->RenderText(
		InnerBounds(),
		((mode & TextMode::Secure) ? std::wstring(text.length(), L'*') : text) + 
		(state == State::Focused && mShowCursor ? L"_" : L""),
		CurrentForeground().value | CurrentBackground().value << 4,
		alignH,
		alignV,
		wrap
	);
}

}