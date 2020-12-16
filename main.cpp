#include "Console.h"

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
		// Window setup
		auto w = new Window(
			GetScreenWidth(), 
			GetScreenHeight(), 
			ElementStyle::Green(false)
		);
		SetCurrentWindow(w);

		w->SetStyle<TextField>(new ElementStyle(
			Color::Black(), Color::DarkGreen(),
			Color::Black(), Color::Green(),
			Color::Black(), Color::DarkGreen(),
			false
		));

		w->SetStyle<Button>(ElementStyle::Green(true));

		w->SetStyle<ProgressBar>(new ElementStyle(
			Color::Green(), Color::DarkGreen(),
			Color::Black(), Color::Black(),
			Color::Black(), Color::Black(),
			false
		));

		auto msLab = new Label({ 1, 1, 13, 1 });
		msLab->SetText(L"Interval (ms)");
		w->AddElement(msLab);

		msTf = new TextField({ 15, 1, 5, 1 });
		msTf->SetText(std::to_wstring(pgMs));
		msTf->AddEventHandler(new EventHandler(nullptr, nullptr, nullptr, nullptr, nullptr, [this](auto w, auto i) { if (i == VK_RETURN) { pgMs = std::stoi(msTf->GetText()); pgTm->SetInterval(std::chrono::milliseconds(pgMs)); } }));
		msTf->SetMode(TEXT_MODE_NUMBERS);
		w->AddElement(msTf);
		
		auto stLab = new Label({ 6, 3, 8, 1 });
		stLab->SetText(L"Step (%)");
		w->AddElement(stLab);

		stTf = new TextField({ 15, 3, 5, 1 });
		stTf->SetText(std::to_wstring(pgSt));
		stTf->AddEventHandler(new EventHandler(nullptr, nullptr, nullptr, nullptr, nullptr, [this](auto w, auto i) { if (i == VK_RETURN) pgSt = std::stoi(stTf->GetText()); }));
		w->AddElement(stTf);

		pgBr = new ProgressBar({ 5, 5, 40, 1 });
		w->AddElement(pgBr);

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
	using namespace gui;

	Program p;
	p.Create(50, 20, 8, 16);
	p.Run();
	return 0;
}