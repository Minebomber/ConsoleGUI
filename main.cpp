#include <math.h>
#include "Console.h"
#include "Styles.h"
class Program : public gui::Console {
private:
	bool mNum = true;
	bool mAlp = true;
	bool mSpc = true;
	bool mSec = false;

	gui::TextField* msTf = nullptr;
	gui::TextField* stTf = nullptr;
	gui::ProgressBar* pgBr = nullptr;

	gui::Timer* pgTm = nullptr;

	int pgMs = 100;
	int pgSt = 1;

public:
	void Initialize() override {
		using namespace gui;
		auto w = new Window(GetScreenWidth(), GetScreenHeight(), ElementStyle::Green(false));
		SetCurrentWindow(w);

		auto msLab = new Label({ 1, 1, 13, 1 });
		w->AddElement(msLab);
		msLab->SetText(L"Interval (ms)");
		msLab->SetBorders(false);

		msTf = new TextField({ 15, 1, 5, 1 });
		msTf->SetText(std::to_wstring(pgMs));
		msTf->AddEventHandler(new EventHandler(nullptr, nullptr, nullptr, nullptr, nullptr, [this](auto w, auto i) { if (i == VK_RETURN) { pgMs = std::stoi(msTf->GetText()); pgTm->SetInterval(std::chrono::milliseconds(pgMs)); } }));
		w->AddElement(msTf);
		msTf->SetMode(TEXT_MODE_NUMBERS);
		msTf->SetBorders(false);
		msTf->SetDefaultBackgroundColor(BG_DARK_GREEN);
		msTf->SetDefaultForegroundColor(FG_BLACK | BG_DARK_GREEN);
		msTf->SetFocusedBackgroundColor(BG_GREEN);
		msTf->SetFocusedForegroundColor(FG_BLACK | BG_GREEN);

		auto stLab = new Label({ 6, 3, 8, 1 });
		w->AddElement(stLab);
		stLab->SetText(L"Step (%)");
		stLab->SetBorders(false);

		stTf = new TextField(*msTf);
		stTf->SetText(std::to_wstring(pgSt));
		stTf->SetBounds({ 15, 3, 5, 1 });
		stTf->AddEventHandler(new EventHandler(nullptr, nullptr, nullptr, nullptr, nullptr, [this](auto w, auto i) { if (i == VK_RETURN) pgSt = std::stoi(stTf->GetText()); }));
		w->AddElement(stTf, false);

		pgBr = new ProgressBar({ 5, 5, 40, 1 });
		w->AddElement(pgBr);
		pgBr->SetBorders(false);

		pgTm = new Timer(std::chrono::milliseconds(pgMs), [this]() { 
			pgBr->IncrementProgress(pgSt * 0.01); 
		});

		auto startBtn = new Button({ 24, 1, 7, 3 });
		startBtn->SetText(L"Start");
		startBtn->AddEventHandler(new EventHandler([this](auto w, auto i) { pgBr->SetProgress(0); pgTm->Start(); }));
		w->AddElement(startBtn);

		auto stopBtn = new Button({ 34, 1, 8, 3 });
		stopBtn->SetText(L"Stop");
		stopBtn->AddEventHandler(new EventHandler([this](auto w, auto i) { pgTm->Stop(); }));
		w->AddElement(stopBtn);

	}
};

int main() {
	Program p;
	p.Create(50, 20, 8, 16);
	p.Run();
	return 0;
}