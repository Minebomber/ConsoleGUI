#include "ConsoleGUI.h"

class Test : public gui::Console {
private:
	gui::TextField* tf = nullptr;
	gui::Label* lab = nullptr;
	gui::Button* btn = nullptr;
public:
	bool Initialize() override {

		tf = new gui::TextField({ 5, 5, 25, 7 }, gui::CharsetT::ALPHANUM);
		tf->SetText(L"Textfield");
		tf->SetTextColor(FG_WHITE | BG_DARK_GREY);
		tf->SetBorder({ L' ', BG_GREY, 0 });
		tf->SetBackgroundColor(BG_DARK_GREY);
		AddElement(tf);

		lab = new gui::Label({ 5, 10, 25, 12 });
		lab->SetText(L"");
		lab->SetTextColor(FG_WHITE);
		lab->SetBackgroundColor(BG_BLACK);
		lab->SetBorder({ L'-', FG_DARK_GREY });
		lab->SetAlignHorizontal(gui::TEXT_ALIGN_MID);
		AddElement(lab);

		btn = new gui::Button({ 30, 5, 39, 7 });
		btn->SetText(L"Submit");
		btn->SetTextColor(BG_GREY | FG_BLACK);
		btn->SetBackgroundColor(BG_GREY);
		btn->SetPressedTextColor(BG_DARK_GREY | FG_WHITE);
		btn->SetPressedBackgroundColor(BG_DARK_GREY);
		btn->SetAlignHorizontal(gui::TEXT_ALIGN_MID);
		btn->SetAlignVertical(gui::TEXT_ALIGN_MID);
		btn->SetButtons(gui::MOUSE_LEFT_BUTTON);

		btn->SetPressAction(
			[&](int m) {
				if (tf->GetText() == L"TEST") {
					lab->SetText(L"Valid");
					lab->SetTextColor(FG_DARK_GREEN);
				} else {
					lab->SetText(L"Invalid");
					lab->SetTextColor(FG_DARK_RED);
				}
				gui::RunAfterDelay(1500, [this]() {
					lab->SetText(L"");
					lab->SetTextColor(FG_WHITE);
					}
				);
			}
		);

		AddElement(btn);
		return true;
	}

	~Test() {
		delete tf;
		delete btn;
		delete lab;
	}
};

int main() {
	Test t;
	t.CreateConsole(96, 54, 12, 12);
	t.Run();
	return 0;
}