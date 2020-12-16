#include "Console.h"

class Program : public gui::Console {
private:
	bool mNum = true;
	bool mAlp = true;
	bool mSpc = true;
	bool mSec = false;

	gui::TextField* tf = nullptr;

	gui::Label* numLab = nullptr;
	gui::Button* numBtn = nullptr;
	
	gui::Label* alpLab = nullptr;
	gui::Button* alpBtn = nullptr;

	gui::Label* spcLab = nullptr;
	gui::Button* spcBtn = nullptr;
	
	gui::Label* secLab = nullptr;
	gui::Button* secBtn = nullptr;

public:
	void Initialize() override {
		using namespace gui;

		auto w = new Window(GetScreenWidth(), GetScreenHeight());
		SetCurrentWindow(w);

		w->SetScheme(WindowScheme::Default());

		tf = new TextField({ 18, 1, 30, 28 });
		w->AddElement(tf);

		numLab = new gui::Label({ 11, 1, 5, 3 });
		numLab->SetText(L"1");
		w->AddElement(numLab);

		numBtn = new Button({ 1, 1, 10, 3 });
		numBtn->SetText(L"NUMERIC");
		numBtn->AddEventHandler(new EventHandler(
			[this](auto w, auto i) { 
				int m = tf->GetMode();
				m ^= TEXT_MODE_NUMBERS;
				tf->SetMode(m);
				mNum = !mNum;
				numLab->SetText(std::wstring(1, (WCHAR)(L'0' + mNum)));
		}));
		w->AddElement(numBtn, false);

		alpLab = new gui::Label({ 11, 5, 5, 3 });
		alpLab->SetText(L"1");
		w->AddElement(alpLab);

		alpBtn = new Button({ 1, 5, 10, 3 });
		alpBtn->SetText(L"ALPHABET");
		alpBtn->AddEventHandler(new EventHandler(
			[this](auto w, auto i) {
				int m = tf->GetMode();
				m ^= TEXT_MODE_ALPHABET;
				tf->SetMode(m);
				mAlp = !mAlp;
				alpLab->SetText(std::wstring(1, (WCHAR)(L'0' + mAlp)));
			}));
		w->AddElement(alpBtn);

		spcLab = new gui::Label({ 11, 9, 5, 3 });
		spcLab->SetText(L"1");
		w->AddElement(spcLab);

		spcBtn = new Button({ 1, 9, 10, 3 });
		spcBtn->SetText(L"SPECIAL");
		spcBtn->AddEventHandler(new EventHandler(
			[this](auto w, auto i) {
				int m = tf->GetMode();
				m ^= TEXT_MODE_SPECIAL;
				tf->SetMode(m);
				mSpc = !mSpc;
				spcLab->SetText(std::wstring(1, (WCHAR)(L'0' + mSpc)));
			}));
		w->AddElement(spcBtn);

		secLab = new gui::Label({ 11, 13, 5, 3 });
		secLab->SetText(L"0");
		w->AddElement(secLab);

		secBtn = new Button({ 1, 13, 10, 3 });
		secBtn->SetText(L"SECURE");
		secBtn->AddEventHandler(new EventHandler(
			[this](auto w, auto i) {
				int m = tf->GetMode();
				m ^= TEXT_MODE_SECURE;
				tf->SetMode(m);
				mSec = !mSec;
				secLab->SetText(std::wstring(1, (WCHAR)(L'0' + mSec)));
			}));
		w->AddElement(secBtn);
		
	}
};

int main() {
	Program p;
	p.Create(50, 30, 8, 16);
	p.Run();
	return 0;
}