#pragma once

#include <functional>
#include <unordered_map>

namespace gui {

class Window;
typedef std::function<void(Window*, int)> EventAction;

class T {
	void f() {
	}
};

enum EventType {
	GUI_MOUSE_DOWN,
	GUI_MOUSE_UP,
	GUI_MOUSE_DRAG,
	GUI_MOUSE_WHEELDOWN,
	GUI_MOUSE_WHEELUP,
	GUI_KEY_DOWN,
	GUI_KEY_UP
};

class EventHandler {
protected:
	std::unordered_map<int, EventAction> mActionMap;
public:
	static EventHandler* New() { return new EventHandler(); }

	EventHandler() : mActionMap() {}
	EventHandler(std::initializer_list<std::pair<const int, EventAction>> m) : mActionMap(m) {}

	bool ActionExists(int e) { return (bool)mActionMap[e]; }
	void InvokeAction(int e, Window* w, int i) { mActionMap[e](w, i); }
	EventHandler* SetAction(int e, EventAction f) { mActionMap[e] = f; return this; }
};

}