#pragma once

#include <functional>
#include <unordered_map>

namespace gui {

class Window;
typedef std::function<void(Window*, int)> EventAction;

enum class EventType {
	MouseDown,
	MouseUp,
	MouseDrag,
	MouseWheelDown,
	MouseWheelUp,
	KeyDown,
	KeyUp
};

class EventHandler {
protected:
	struct EventTypeHash { 
		template <typename T> 
		std::size_t operator()(T t) const { return static_cast<std::size_t>(t); }
	};
	std::unordered_map<EventType, EventAction, EventTypeHash> mActionMap;
public:
	static EventHandler* New() { return new EventHandler(); }

	EventHandler() : mActionMap() {}
	EventHandler(std::initializer_list<std::pair<const EventType, EventAction>> m) : mActionMap(m) {}

	bool ActionExists(EventType e) { return (bool)mActionMap[e]; }
	void InvokeAction(EventType e, Window* w, int i) { mActionMap[e](w, i); }
	EventHandler* SetAction(EventType e, EventAction f) { mActionMap[e] = f; return this; }
};

}