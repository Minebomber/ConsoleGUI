#include "Console.h"

int main() {
	using namespace gui;

	Console console;
	console.Create(30, 30, 8, 16);

	auto w = new Window(console.GetScreenWidth(), console.GetScreenHeight());
	console.SetCurrentWindow(w);

	w->SetScheme(WindowScheme::Red());

	auto e1 = new Element({ 5, 5, 10, 10 });
	w->AddElement(e1);

	console.Run();
	return 0;
}