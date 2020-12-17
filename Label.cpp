#include "Label.h"

namespace gui {

Label::Label(Rect b) : Element(b) {}

Label::Label(Rect b, std::wstring t, bool fit) : Element(b), mText(t) {
	if (fit) Autosize();
}

Label::Label(const Label& e) : Element(e), mText(e.mText), mAlignH(e.mAlignH), mAlignV(e.mAlignV), mTextWrap(e.mTextWrap) {}

void Label::Autosize() {
	int w = 0, h = 0, l = mText.length();
	for (int i = 0, nl = 0; i < l; i++) {
		if (i == l - 1 || mText[i] == L'\n') {
			if (i - nl + 1 > w) { w = i - nl + 1; nl = i; }
			h++;
		}
	}
	mBounds.SetWidth(w + 2 * mBorders);
	mBounds.SetHeight(h + 2 * mBorders);
}

void Label::RenderText(Window* c, Rect r, const std::wstring& s, WORD cl) {
	int textWidth = r.GetWidth();
	int textHeight = r.GetHeight();

	struct LineInfo {
		int dX, sI, lW;
	};
	LineInfo* lines = new LineInfo[textHeight];
	int currentLine = 0;

	int lineBeginIdx = 0;
	int currentIdx = 0;
	while (currentIdx < s.length() && currentLine < textHeight) {
		int lineWidth = currentIdx - lineBeginIdx + 1;
		// Adjust line spacing x
		int xOffset = 0;
		if (mAlignH == TEXT_ALIGN_MID) xOffset = (textWidth - lineWidth) / 2;
		else if (mAlignH == TEXT_ALIGN_MAX)  xOffset = textWidth - lineWidth;

		if (lineWidth >= textWidth) {
			// line too long
			if (mTextWrap == TEXT_WRAP_WORD) {
				// find space
				auto iStart = s.rbegin() + (s.length() - currentIdx - 1);
				auto iEnd = s.rbegin() + (s.length() - lineBeginIdx);
				auto iSpace = std::find(iStart, iEnd, L' ');

				if (iSpace != iEnd) {
					// if end of string, print line as is
					// else print until space, newline there
					if (currentIdx == s.length() - 1) {
						lines[currentLine] = { xOffset, lineBeginIdx, lineWidth }; currentLine++;
					} else {
						int spaceIdx = (int)std::distance(s.begin(), iSpace.base()) - 1;
						lineWidth = spaceIdx - lineBeginIdx;
						if (mAlignH == TEXT_ALIGN_MID) xOffset = (textWidth - lineWidth) / 2;
						else if (mAlignH == TEXT_ALIGN_MAX)  xOffset = textWidth - lineWidth;

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
		} else if (s[currentIdx] == L'\n') {
			// break on newline
			// dont want to include \n, print linewidth - 1
			lineWidth--;
			if (mAlignH == TEXT_ALIGN_MID) xOffset = (textWidth - lineWidth) / 2;
			else if (mAlignH == TEXT_ALIGN_MAX)  xOffset = textWidth - lineWidth;

			lines[currentLine] = { xOffset, lineBeginIdx, lineWidth }; currentLine++;
			// move to next line
			lineBeginIdx = currentIdx + 1;
		} else if (currentIdx == s.length() - 1) {
			// display to end
			// same line, no wrapping
			lines[currentLine] = { xOffset, lineBeginIdx, lineWidth }; currentLine++;
		}
		currentIdx++;
	}

	int yOffset = 0;
	if (mAlignV == TEXT_ALIGN_MID) yOffset = (textHeight - currentLine) / 2;
	else if (mAlignV == TEXT_ALIGN_MAX) yOffset = textHeight - currentLine;
	for (int i = 0; i < currentLine; i++)
		c->WriteString(r.GetX() + lines[i].dX, r.GetY() + yOffset + i, s, cl, lines[i].sI, lines[i].lW);

	delete[] lines;
}

void Label::Draw(Window* w) {
	Element::Draw(w);
	Rect r = GetInnerBounds();
	RenderText(w, r, mText, GetCurrentForeground().Foreground() | GetCurrentBackground().Background());
}

}