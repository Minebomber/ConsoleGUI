#pragma once
#include "Label.h"

namespace gui {

struct TextMode {
	enum Mode {
		Numbers = 0x1, // 0001
		Alphabet = 0x2, // 0010
		Special = 0x4, // 0100
		Secure = 0x8, // 1000

		Alphanum = 0x3, // 0011
		All = 0x7, // 0111
	};
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
	TextMode::Mode mode = TextMode::All;

	TextField(Rect b);
	TextField(Rect b, TextMode::Mode m);
	TextField(const TextField& e);

	virtual void Autosize() override {}

	virtual void Draw(Window* w) override;
};

}