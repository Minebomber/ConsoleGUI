#pragma once

#include <functional>

namespace gui {

class Window;
typedef std::function<void(Window*, int)> EventAction;

class EventHandler {
protected:
	EventAction mMouseDownAction = nullptr;
	EventAction mMouseUpAction = nullptr;
	
	EventAction mMouseDragAction = nullptr;

	EventAction mMouseWheelDownAction = nullptr;
	EventAction mMouseWheelUpAction = nullptr;

	EventAction mKeyDownAction = nullptr;
	EventAction mKeyUpAction = nullptr;

public:
	static EventHandler* New() { return new EventHandler(); }

	bool MouseDownActionExists() const { return (bool)mMouseDownAction; }
	void InvokeMouseDownAction(Window* w, int i) { mMouseDownAction(w, i); }
	EventHandler* SetMouseDownAction(EventAction f) { mMouseDownAction = f; return this; }

	bool MouseUpActionExists() const { return (bool)mMouseUpAction; }
	void InvokeMouseUpAction(Window* w, int i) { mMouseUpAction(w, i); }
	EventHandler* SetMouseUpAction(EventAction f) { mMouseUpAction = f; return this; }

	bool MouseDragActionExists() const { return (bool)mMouseDragAction; }
	void InvokeMouseDragAction(Window* w, int i) { mMouseDragAction(w, i); }
	EventHandler* SetMouseDragAction(EventAction f) { mMouseDragAction = f; return this; }

	bool MouseWheelDownActionExists() const { return (bool)mMouseWheelDownAction; }
	void InvokeMouseWheelDownAction(Window* w, int i) { mMouseWheelDownAction(w, i); }
	EventHandler* SetMouseWheelDownAction(EventAction f) { mMouseWheelDownAction = f; return this; }

	bool MouseWheelUpActionExists() const { return (bool)mMouseWheelUpAction; }
	void InvokeMouseWheelUpAction(Window* w, int i) { mMouseWheelUpAction(w, i); }
	EventHandler* SetMouseWheelUpAction(EventAction f) { mMouseWheelUpAction = f; return this; }

	bool KeyDownActionExists() const { return (bool)mKeyDownAction; }
	void InvokeKeyDownAction(Window* w, int i) { mKeyDownAction(w, i); }
	EventHandler* SetKeyDownAction(EventAction f) { mKeyDownAction = f; return this; }
	
	bool KeyUpActionExists() const { return (bool)mKeyUpAction; }
	void InvokeKeyUpAction(Window* w, int i) { mKeyUpAction(w, i); }
	EventHandler* SetKeyUpAction(EventAction f) { mKeyUpAction = f; return this; }
};

}