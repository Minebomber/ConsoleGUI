#pragma once

#include <functional>

namespace gui {

class Window;
typedef std::function<void(Window*, int)> EventAction;

class EventHandler {
protected:
	EventAction mMouseDownAction;
	EventAction mMouseUpAction;
	
	EventAction mMouseDragAction;

	EventAction mMouseWheelDownAction;
	EventAction mMouseWheelUpAction;

	EventAction mKeyDownAction;
	EventAction mKeyUpAction;

public:
	EventHandler(
		EventAction mDown = nullptr,
		EventAction mUp = nullptr,
		EventAction mDrag = nullptr,
		EventAction wDown = nullptr,
		EventAction wUp = nullptr,
		EventAction kDown = nullptr,
		EventAction kUp = nullptr
	) : mMouseDownAction(mDown),
		mMouseUpAction(mUp),
		mMouseDragAction(mDrag),
		mMouseWheelDownAction(wDown),
		mMouseWheelUpAction(wUp),
		mKeyDownAction(kDown),
		mKeyUpAction(kUp) {}

	bool MouseDownActionExists() const { return (bool)mMouseDownAction; }
	void InvokeMouseDownAction(Window* w, int i) { mMouseDownAction(w, i); }
	void SetMouseDownAction(EventAction f) { mMouseDownAction = f; }

	bool MouseUpActionExists() const { return (bool)mMouseUpAction; }
	void InvokeMouseUpAction(Window* w, int i) { mMouseUpAction(w, i); }
	void SetMouseUpAction(EventAction f) { mMouseUpAction = f; }

	bool MouseDragActionExists() const { return (bool)mMouseDragAction; }
	void InvokeMouseDragAction(Window* w, int i) { mMouseDragAction(w, i); }
	void SetMouseDragAction(EventAction f) { mMouseDragAction = f; }

	bool MouseWheelDownActionExists() const { return (bool)mMouseWheelDownAction; }
	void InvokeMouseWheelDownAction(Window* w, int i) { mMouseWheelDownAction(w, i); }
	void SetMouseWheelDownAction(EventAction f) { mMouseWheelDownAction = f; }

	bool MouseWheelUpActionExists() const { return (bool)mMouseWheelUpAction; }
	void InvokeMouseWheelUpAction(Window* w, int i) { mMouseWheelUpAction(w, i); }
	void SetMouseWheelUpAction(EventAction f) { mMouseWheelUpAction = f; }

	bool KeyDownActionExists() const { return (bool)mKeyDownAction; }
	void InvokeKeyDownAction(Window* w, int i) { mKeyDownAction(w, i); }
	void SetKeyDownAction(EventAction f) { mKeyDownAction = f; }
	
	bool KeyUpActionExists() const { return (bool)mKeyUpAction; }
	void InvokeKeyUpAction(Window* w, int i) { mKeyUpAction(w, i); }
	void SetKeyUpAction(EventAction f) { mKeyUpAction = f; }
};

}