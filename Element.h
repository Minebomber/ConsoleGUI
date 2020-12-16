#pragma once

#define NOMINMAX
#include <Windows.h>
#include <future>
#include <thread>
#include <vector>

#include "Colors.h"
#include "Rect.h"
#include "Events.h"
#include "Window.h"

namespace gui {

class Window;

enum ElementState {
	ELEMENT_DEFAULT,
	ELEMENT_FOCUSED,
	ELEMENT_DISABLED,
};

class Element {
	friend class Window;
	friend class Console;
protected:
	Rect mBounds = { 0, 0, 0, 0 };

	int mState = ELEMENT_DEFAULT;

	WORD mDefaultForegroundColor = FG_WHITE;
	WORD mDefaultBackgroundColor = BG_BLACK;

	WORD mFocusedForegroundColor = FG_WHITE;
	WORD mFocusedBackgroundColor = FG_WHITE;

	WORD mDisabledForegroundColor = FG_WHITE;
	WORD mDisabledBackgroundColor = FG_WHITE;

	bool mBorders = true;

	std::vector<EventHandler*> mEventHandlers;
public:

	Element(Rect b) : mBounds(b) {}
	Element(const Element& e) : 
		mBounds(e.mBounds), mBorders(e.mBorders),
		mDefaultForegroundColor(e.mDefaultForegroundColor), mDefaultBackgroundColor(e.mDefaultBackgroundColor), 
		mFocusedForegroundColor(e.mFocusedForegroundColor), mFocusedBackgroundColor(e.mFocusedBackgroundColor), 
		mDisabledForegroundColor(e.mDisabledForegroundColor), mDisabledBackgroundColor(e.mDisabledBackgroundColor) 
	{}

	virtual ~Element() {}

	void HandleMouseDownEvent(Window* w, int i) {
		for (EventHandler* h : mEventHandlers)
			if (h->MouseDownActionExists()) h->InvokeMouseDownAction(w, i);
	}

	void HandleMouseUpEvent(Window* w, int i) {
		for (EventHandler* h : mEventHandlers)
			if (h->MouseUpActionExists()) h->InvokeMouseUpAction(w, i);
	}

	void HandleMouseDragEvent(Window* w, int i) {
		for (EventHandler* h : mEventHandlers)
			if (h->MouseDragActionExists()) h->InvokeMouseDragAction(w, i);
	}

	void HandleMouseWheelDownEvent(Window* w, int i) {
		for (EventHandler* h : mEventHandlers)
			if (h->MouseWheelDownActionExists()) h->InvokeMouseWheelDownAction(w, i);
	}

	void HandleMouseWheelUpEvent(Window* w, int i) {
		for (EventHandler* h : mEventHandlers)
			if (h->MouseWheelUpActionExists()) h->InvokeMouseWheelUpAction(w, i);
	}

	void HandleKeyDownEvent(Window* w, int i) {
		for (EventHandler* h : mEventHandlers)
			if (h->KeyDownActionExists()) h->InvokeKeyDownAction(w, i);
	}

	void HandleKeyUpEvent(Window* w, int i) {
		for (EventHandler* h : mEventHandlers)
			if (h->KeyUpActionExists()) h->InvokeKeyUpAction(w, i);
	}

	const Rect& GetBounds() const { return mBounds; }
	virtual void SetBounds(Rect b) { mBounds = b; }

	const WORD& GetDefaultForegroundColor() const { return mDefaultForegroundColor; }
	void SetDefaultForegroundColor(WORD c) { mDefaultForegroundColor = c; }
	const WORD& GetDefaultBackgroundColor() const { return mDefaultBackgroundColor; }
	void SetDefaultBackgroundColor(WORD c) { mDefaultBackgroundColor = c; }

	const WORD& GetFocusedForegroundColor() const { return mFocusedForegroundColor; }
	void SetFocusedForegroundColor(WORD c) { mFocusedForegroundColor = c; }
	const WORD& GetFocusedBackgroundColor() const { return mFocusedBackgroundColor; }
	void SetFocusedBackgroundColor(WORD c) { mFocusedBackgroundColor = c; }

	const WORD& GetDisabledForegroundColor() const { return mDisabledBackgroundColor; }
	void SetDisabledForegroundColor(WORD c) { mDisabledBackgroundColor = c; }
	const WORD& GetDisabledBackgroundColor() const { return mDisabledBackgroundColor; }
	void SetDisabledBackgroundColor(WORD c) { mDisabledBackgroundColor = c; }

	void AddEventHandler(EventHandler* e) { mEventHandlers.push_back(e); }
	void RemoveEventHandler(EventHandler* e) { mEventHandlers.erase(std::remove(mEventHandlers.begin(), mEventHandlers.end(), e), mEventHandlers.end()); }

	const bool& GetBorders() const { return mBorders; }
	void SetBorders(bool b) { mBorders = b; }

	WORD GetCurrentForegroundColor() const;
	WORD GetCurrentBackgroundColor() const;
	Rect GetInnerBounds() const;

	virtual void Draw(Window* w);
};

}