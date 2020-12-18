#pragma once
#include "Label.h"

namespace gui {

enum TextMode {
	TEXT_MODE_NUMBERS = 0x1, // 0001
	TEXT_MODE_ALPHABET = 0x2, // 0010
	TEXT_MODE_SPECIAL = 0x4, // 0100
	TEXT_MODE_SECURE = 0x8, // 1000

	TEXT_MODE_ALPHANUM = 0x3, // 0011
	TEXT_MODE_ALL = 0x7, // 0111
};

class TextField : public Label {
private:
	void Init();
protected:
	bool mCapitalize = false;
	bool mShowCursor = false;
	std::future<void> mCursorFlashFuture;

	bool ValidKeyForMode(int k);
	void FlashCursor();
public:
	int mode = TEXT_MODE_ALL;

	TextField(Rect b);
	TextField(Rect b, int m);
	TextField(const TextField& e);

	virtual void Draw(Window* w) override;
};

}