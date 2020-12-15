#include "Console.h"

void SetupGui(gui::Console& c) {
	using namespace gui;

	
}

int main() {
	using namespace gui;

	Console console;
	console.Create(31, 30, 8, 16);

	auto w = new Window(console.GetScreenWidth(), console.GetScreenHeight());
	console.SetCurrentWindow(w);

	Timer t(std::chrono::milliseconds(50));

	w->SetScheme(WindowScheme::Default());

	auto progress = new ProgressBar({ 0, 1, 31, 3 }, 0.5);
	w->AddElement(progress);
	
	auto start = new Button({ 1, 10, 9, 3 });
	start->SetText(L"Start");
	start->AddEventHandler(new EventHandler(
		[&t](auto w, auto m) { t.Start(); }
	));
	w->AddElement(start);

	auto stop = new Button({ 21, 10, 9, 3 });
	stop->SetText(L"Stop");
	stop->AddEventHandler(new EventHandler(
		[&t](auto w, auto m) { t.Stop(); }
	));
	w->AddElement(stop);

	t.SetAction([progress]() {
		progress->IncrementProgress(0.01);
	});

	console.Run();
	return 0;
}