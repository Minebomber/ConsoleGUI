#include "ConsoleGUI.h"

class Test : public gui::Console {
private:
	gui::Panel* panel;
	gui::Button* lButton;
	gui::Button* rButton;

	gui::Label* lLabel;
	gui::Label* rLabel;

	int lClicks = 0;
	int rClicks = 0;
public:
	bool Initialize() override {

		panel = new gui::Panel({ 1, 1, 48, 28 });
		panel->SetBackgroundColor(BG_DARK_GREY);
		panel->GetTitleLabel().SetText(L"Click Counter Test");
		panel->GetTitleLabel().SetTextColor(FG_BLACK | BG_GREY);
		panel->GetTitleLabel().SetBackgroundColor(BG_GREY);
		AddElement(panel);

		lButton = new gui::Button({3, 15, 23, 26});
		lButton->SetText(L"Left\nClick\nButton");
		lButton->SetAlignHorizontal(gui::TEXT_ALIGN_MID);
		lButton->SetAlignVertical(gui::TEXT_ALIGN_MID);
		lButton->SetTextColor(FG_BLACK | BG_WHITE);
		lButton->SetPressedTextColor(FG_WHITE | BG_GREY);
		lButton->SetPressedBackgroundColor(BG_GREY);
		lButton->SetButtons(gui::MOUSE_LEFT_BUTTON);
		lButton->SetPressAction([this](int m) {
			lClicks++;
			lLabel->SetText(L"Clicks\n" + std::to_wstring(lClicks));
		});
		AddElement(lButton);

		rButton = new gui::Button({ 26, 15, 46, 26 });
		rButton->SetText(L"Right\nClick\nButton");
		rButton->SetAlignHorizontal(gui::TEXT_ALIGN_MID);
		rButton->SetAlignVertical(gui::TEXT_ALIGN_MID);
		rButton->SetTextColor(FG_BLACK | BG_WHITE);
		rButton->SetPressedTextColor(FG_WHITE | BG_GREY);
		rButton->SetPressedBackgroundColor(BG_GREY);
		rButton->SetButtons(gui::MOUSE_RIGHT_BUTTON);
		rButton->SetPressAction([this](int m) {
			rClicks++;
			rLabel->SetText(L"Clicks\n" + std::to_wstring(rClicks));
		});
		AddElement(rButton);

		lLabel = new gui::Label({3, 5, 23, 13});
		lLabel->SetText(L"Clicks\n0");
		lLabel->SetTextColor(FG_WHITE | BG_BLACK);
		lLabel->SetBackgroundColor(BG_BLACK);
		lLabel->SetAlignHorizontal(gui::TEXT_ALIGN_MID);
		lLabel->SetAlignVertical(gui::TEXT_ALIGN_MID);
		lLabel->GetBorder().SetColor(BG_GREY);
		lLabel->GetBorder().SetWidth(1);
		AddElement(lLabel);

		rLabel = new gui::Label({ 26, 5, 46, 13 });
		rLabel->SetText(L"Clicks\n0");
		rLabel->SetTextColor(FG_WHITE | BG_BLACK);
		rLabel->SetBackgroundColor(BG_BLACK);
		rLabel->SetAlignHorizontal(gui::TEXT_ALIGN_MID);
		rLabel->SetAlignVertical(gui::TEXT_ALIGN_MID);
		rLabel->GetBorder().SetColor(BG_GREY);
		rLabel->GetBorder().SetWidth(1);
		AddElement(rLabel);


		return true;
	}

	~Test() {
		delete panel;
		delete lButton;
		delete rButton;
		delete lLabel;
		delete rLabel;
	}
};

int main() {
	Test t;
	t.CreateConsole(50, 50, 14, 14);
	t.Run();
	return 0;
}