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

	Color mDefaultForeground = Color::White();
	Color mDefaultBackground = Color::Black();

	Color mFocusedForeground = Color::White();
	Color mFocusedBackground = Color::Black();

	Color mDisabledForeground  = Color::White();
	Color mDisabledBackground  = Color::Black();

	bool mBorders = false;

	std::vector<EventHandler*> mEventHandlers;
public:

	Element(Rect b) : mBounds(b) {}
	Element(const Element& e) : 
		mBounds(e.mBounds), mBorders(e.mBorders),
		mDefaultForeground(e.mDefaultForeground), mDefaultBackground(e.mDefaultBackground), 
		mFocusedForeground(e.mFocusedForeground), mFocusedBackground(e.mFocusedBackground), 
		mDisabledForeground(e.mDisabledForeground), mDisabledBackground(e.mDisabledBackground) 
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

	const Color& GetDefaultForeground() const { return mDefaultForeground; }
	void SetDefaultForeground(Color c) { mDefaultForeground = c; }
	const Color& GetDefaultBackground() const { return mDefaultBackground; }
	void SetDefaultBackground(Color c) { mDefaultBackground = c; }

	const Color& GetFocusedForeground() const { return mFocusedForeground; }
	void SetFocusedForeground(Color c) { mFocusedForeground = c; }
	const Color& GetFocusedBackground() const { return mFocusedBackground; }
	void SetFocusedBackground(Color c) { mFocusedBackground = c; }

	const Color& GetDisabledForeground() const { return mDisabledBackground; }
	void SetDisabledForeground(Color c) { mDisabledBackground = c; }
	const Color& GetDisabledBackground() const { return mDisabledBackground; }
	void SetDisabledBackground(Color c) { mDisabledBackground = c; }

	void AddEventHandler(EventHandler* e) { mEventHandlers.push_back(e); }
	void RemoveEventHandler(EventHandler* e) { mEventHandlers.erase(std::remove(mEventHandlers.begin(), mEventHandlers.end(), e), mEventHandlers.end()); }

	const bool& GetBorders() const { return mBorders; }
	void SetBorders(bool b) { mBorders = b; }

	Color GetCurrentForeground() const;
	Color GetCurrentBackground() const;
	Rect GetInnerBounds() const;

	virtual void Draw(Window* w);
};

}