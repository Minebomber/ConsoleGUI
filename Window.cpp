#include "Window.h"

namespace gui {

Window::Window(int w, int h, Style* defStyle) : bounds(0, 0, w, h),
mBuffer(new CHAR_INFO[w * h]), mStyleMap({ {std::type_index(typeid(Element)), defStyle} }) {}


void Window::SetChar(int x, int y, WCHAR chr, WORD clr) {
	int i = y * bounds.width + x;
	if (i < bounds.width * bounds.height) {
		mBuffer[i].Char.UnicodeChar = chr;
		mBuffer[i].Attributes = clr;
	}
}

void Window::FillScreen(WCHAR chr, WORD clr) {
	for (int i = 0; i < bounds.width * bounds.height; i++) {
		mBuffer[i].Char.UnicodeChar = chr;
		mBuffer[i].Attributes = clr;
	}
}

void Window::DrawRect(Rect r, WCHAR chr, WORD clr, bool fill) {
	for (int i = 0; i < r.height; i++) {
		for (int j = 0; j < r.width; j++) {
			if (fill) SetChar(j + r.x, i + r.y, chr, clr);
			else if (i == 0 || i == r.height - 1 || j == 0 || j == r.width - 1) 
				SetChar(j + r.x, i + r.y, chr, clr);
		}
	}
}

void Window::WriteString(int x, int y, const std::wstring& str, WORD clr) {
	for (int i = 0; i < str.size(); i++) {
		int idx = y * bounds.width + x + i;
		if (idx < bounds.width * bounds.height) {
			mBuffer[y * bounds.width + x + i].Char.UnicodeChar = std::max(str[i], L' ');
			mBuffer[y * bounds.width + x + i].Attributes = clr;
		}
	}
}

void Window::WriteString(int x, int y, const std::wstring& str, WORD clr, int st, int w) {
	for (int i = 0; i < w; i++) {
		int idx = y * bounds.width + x + i;
		if (idx < bounds.width * bounds.height) {
			mBuffer[y * bounds.width + x + i].Char.UnicodeChar = std::max(str[st + i], L' ');
			mBuffer[y * bounds.width + x + i].Attributes = clr;
		}
	}
}

void Window::RenderText(Rect r, const std::wstring& txt, WORD clr, int alignH, int alignV, int wrap) {
	int textWidth = r.width;
	int textHeight = r.height;

	struct LineInfo {
		int dX, sI, lW;
	};
	LineInfo* lines = new LineInfo[textHeight];
	int currentLine = 0;

	int lineBeginIdx = 0;
	int currentIdx = 0;
	while (currentIdx < txt.length() && currentLine < textHeight) {
		int lineWidth = currentIdx - lineBeginIdx + 1;
		// Adjust line spacing x
		int xOffset = 0;
		if (alignH == TEXT_ALIGN_MID) xOffset = (textWidth - lineWidth) / 2;
		else if (alignH == TEXT_ALIGN_MAX)  xOffset = textWidth - lineWidth;

		if (lineWidth >= textWidth) {
			// line too long
			if (wrap == TEXT_WRAP_WORD) {
				// find space
				auto iStart = txt.rbegin() + (txt.length() - currentIdx - 1);
				auto iEnd = txt.rbegin() + (txt.length() - lineBeginIdx);
				auto iSpace = std::find(iStart, iEnd, L' ');

				if (iSpace != iEnd) {
					// if end of string, print line as is
					// else print until space, newline there
					if (currentIdx == txt.length() - 1) {
						lines[currentLine] = { xOffset, lineBeginIdx, lineWidth }; currentLine++;
					} else {
						int spaceIdx = (int)std::distance(txt.begin(), iSpace.base()) - 1;
						lineWidth = spaceIdx - lineBeginIdx;
						if (alignH == TEXT_ALIGN_MID) xOffset = (textWidth - lineWidth) / 2;
						else if (alignH == TEXT_ALIGN_MAX)  xOffset = textWidth - lineWidth;

						lines[currentLine] = { xOffset, lineBeginIdx, lineWidth }; currentLine++;
						lineBeginIdx = spaceIdx + 1;
					}
				} else {
					// char wrap
					// display line from linebegin->current
					// move to next line
					lines[currentLine] = { xOffset, lineBeginIdx, lineWidth }; currentLine++;
					lineBeginIdx = currentIdx + 1;
				}
			} else {
				// char wrap
					// display line from linebegin->current
				// move to next line
				lines[currentLine] = { xOffset, lineBeginIdx, lineWidth }; currentLine++;
				lineBeginIdx = currentIdx + 1;
			}
		} else if (txt[currentIdx] == L'\n') {
			// break on newline
			// dont want to include \n, print linewidth - 1
			lineWidth--;
			if (alignH == TEXT_ALIGN_MID) xOffset = (textWidth - lineWidth) / 2;
			else if (alignH == TEXT_ALIGN_MAX)  xOffset = textWidth - lineWidth;

			lines[currentLine] = { xOffset, lineBeginIdx, lineWidth }; currentLine++;
			// move to next line
			lineBeginIdx = currentIdx + 1;
		} else if (currentIdx == txt.length() - 1) {
			// display to end
			// same line, no wrapping
			lines[currentLine] = { xOffset, lineBeginIdx, lineWidth }; currentLine++;
		}
		currentIdx++;
	}

	int yOffset = 0;
	if (alignV == TEXT_ALIGN_MID) yOffset = (textHeight - currentLine) / 2;
	else if (alignV == TEXT_ALIGN_MAX) yOffset = textHeight - currentLine;
	for (int i = 0; i < currentLine; i++)
		WriteString(r.x + lines[i].dX, r.y + yOffset + i, txt, clr, lines[i].sI, lines[i].lW);

	delete[] lines;
}

void Window::ApplyStyle(Element* e) {
	if (Style* s = GetStyle(*e)) { e->style = *s; }
}

void Window::AddElement(Element* e, bool applyStyle, bool postAutosize) {
	mElements.push_back(e);
	if (applyStyle) { 
		ApplyStyle(e);
		if (postAutosize) e->Autosize();
	}
}

void Window::AddElements(std::initializer_list<Element*> es, bool applyStyle, bool postAutosize) {
	for (Element* e : es) AddElement(e, applyStyle, postAutosize);
}

void Window::RemoveElement(Element* e) { 
	if (focusedElement == e) focusedElement = nullptr; 
	mElements.erase(std::remove(mElements.begin(), mElements.end(), e), mElements.end()); 
}

Element* Window::GetElementAtPoint(const Point& p) {
	Element* r = nullptr;
	for (Element* e : mElements) if (e->bounds.Contains(p)) r = e;
	return r;
}

void Window::Display() {
	FillScreen(baseChar, baseColor);
	for (Element* e : mElements) {
		e->Draw(this);
	}
}

Window::~Window() {
	for (Element* e : mElements) delete e;
	for (auto p : mStyleMap) delete p.second;
	delete[] mBuffer;
}

}