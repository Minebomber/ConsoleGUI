#include "ConsoleGUI.h"

class Test : public gui::Console {
public:
	bool Initialize() override {

		gui::ContentPanel* panel = new gui::ContentPanel({ 1, 1, 48, 48 });
		panel->GetTitleLabel().SetText(L"Panel Title");
		panel->GetTitleLabel().SetTextColor(FG_BLACK | BG_WHITE);
		panel->GetTitleLabel().SetBackgroundColor(BG_CYAN);

		AddElement(panel);

		return true;
	}
};

int main() {
	Test t;
	t.CreateConsole(50, 50, 12, 16);
	t.Run();
	return 0;
}