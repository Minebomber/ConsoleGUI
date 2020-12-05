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
		tf->SetTextColor(gui::FG_WHITE | gui::BG_DARK_GREY);
		tf->SetBorder({ L' ', gui::BG_GREY, 0 });
		tf->SetBackgroundColor(gui::BG_DARK_GREY);
		AddElement(tf);

		lab = new gui::Label({ 5, 10, 25, 12 });
		lab->SetText(L"");
		lab->SetTextColor(gui::FG_WHITE);
		lab->SetBackgroundColor(gui::BG_BLACK);
		lab->SetBorder({ L'-', gui::FG_DARK_GREY });
		lab->SetAlignHorizontal(gui::TEXT_ALIGN_MID);
		AddElement(lab);

		btn = new gui::Button({ 30, 5, 39, 7 });
		btn->SetText(L"Submit");
		btn->SetTextColor(gui::BG_GREY | gui::FG_BLACK);
		btn->SetBackgroundColor(gui::BG_GREY);
		btn->SetPressedTextColor(gui::BG_DARK_GREY | gui::FG_WHITE);
		btn->SetPressedBackgroundColor(gui::BG_DARK_GREY);
		btn->SetAlignHorizontal(gui::TEXT_ALIGN_MID);
		btn->SetAlignVertical(gui::TEXT_ALIGN_MID);
		btn->SetButtons(gui::MOUSE_LEFT_BUTTON);

		btn->SetPressAction(
			[&](int m) {
				if (tf->GetText() == L"TEST") {
					lab->SetText(L"Valid");
					lab->SetTextColor(gui::FG_DARK_GREEN);
				} else {
					lab->SetText(L"Invalid");
					lab->SetTextColor(gui::FG_DARK_RED);
				}
				gui::RunAfterDelay(1500, [this]() {
					lab->SetText(L"");
					lab->SetTextColor(gui::FG_WHITE);
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