#pragma once

#include <functional>
#include <unordered_map>

namespace gui {

class Window;
typedef std::function<void(Window*, int)> EventAction;

enum EventType {
	EVENT_MOUSE_DOWN,
	EVENT_MOUSE_UP,
	EVENT_MOUSE_DRAG,
	EVENT_MOUSE_WHEELDOWN,
	EVENT_MOUSE_WHEELUP,
	EVENT_KEY_DOWN,
	EVENT_KEY_UP
};

class EventHandler {
protected:
	std::unordered_map<int, EventAction> mActions;
public:
	static EventHandler* New() { return new EventHandler(); }

	bool ActionExists(int e) { return (bool)mActions[e]; }
	void InvokeAction(int e, Window* w, int i) { mActions[e](w, i); }
	EventHandler* SetAction(int e, EventAction f) { mActions[e] = f; return this; }
};

}