#include "Window.h"

namespace gui {

void Window::AddElement(Element* e) {
	if (e->GetId() != -1) return;
	for (size_t i = 0; i < mElements.size(); i++) {
		if (mElements.at(i) == nullptr) {
			e->mId = i;
			mElements.at(i) = e;
			return;
		}
	}
	mElements.push_back(e);
	e->mId = mElements.size() - 1;

	if (e->mMouseHandler) AddMouseHandler(e->mMouseHandler);
}

Element* Window::GetElement(int i) {
	return mElements.at(i);
}

Element* Window::GetElement(Element* e) {
	return GetElement(e->mId);
}

void Window::RemoveElement(int i) {
	if (mElements.at(i)->mMouseHandler) RemoveMouseHandler(mElements.at(i)->mMouseHandler);

	mElements.at(i)->mId = -1;
	mElements.at(i) = nullptr;
}

void Window::RemoveElement(Element* e) {
	RemoveElement(e->mId);
}

void Window::AddMouseHandler(MouseHandler* h) {
	if (h->GetId() != -1) return;
	for (size_t i = 0; i < mMouseHandlers.size(); i++) {
		if (mMouseHandlers.at(i) == nullptr) {
			h->mId = i;
			mMouseHandlers.at(i) = h;
			return;
		}
	}
	mMouseHandlers.push_back(h);
	h->mId = mMouseHandlers.size() - 1;
}

MouseHandler* Window::GetMouseHandler(int i) { return mMouseHandlers.at(i); }

MouseHandler* Window::GetMouseHandler(MouseHandler* h) { return GetMouseHandler(h->mId); }

void Window::RemoveMouseHandler(int i) {
	mMouseHandlers.at(i)->mId = -1;
	mMouseHandlers.at(i) = nullptr;
}

void Window::RemoveMouseHandler(MouseHandler* h) { RemoveMouseHandler(h->mId); }

}