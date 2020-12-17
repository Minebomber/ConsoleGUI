#include "Console.h"

class Program : public gui::Console {
public:
	gui::Window* SetupWindow() {
		auto w = new gui::Window(GetScreenWidth(), GetScreenHeight());
		w->SetStyle<gui::Element>(gui::ElementStyle::Green(true));
		SetCurrentWindow(w);
		return w;
	}

	void Initialize() override {
		using namespace gui;

		auto w = SetupWindow();

		std::vector<std::wstring> titles = { L"I", L"Fucking", L"Hate", L"C++", L"Because", L"It", L"Sucks", L"Dick" };
		Label* l = nullptr;
		int i = 0;
		for (std::wstring& s : titles) {
			auto lab = new Label({10, 3}, s, true);
			i++;
			if (l) {
				if (i % 2) lab->AlignTopToBottom(l, 1)->AlignLeftToLeft(l)->AlignRightToLeft(l, -3);
				else lab->AlignLeftToRight(l, 1)->AlignTopToTop(l);

			}
			l = lab;
			w->AddElement(l);
		}
	}
};

int main() {
	Program p;
	p.Create(50, 20, 8, 16);
	p.Run();
	return 0;
}