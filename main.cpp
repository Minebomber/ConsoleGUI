#include "Console.h"

void SetupGui(gui::Console& c) {
	using namespace gui;

	
}

int main() {
	using namespace gui;

	Console console;
	console.Create(30, 30, 8, 16);

	auto w = new Window(console.GetScreenWidth(), console.GetScreenHeight());
	console.SetCurrentWindow(w);

	w->SetScheme(WindowScheme::Default());

	auto tf = new TextField({ 1, 1, 28, 15 });
	tf->SetMode(TEXT_MODE_ALPHABET);
	w->AddElement(tf);

	console.Run();
	return 0;
}