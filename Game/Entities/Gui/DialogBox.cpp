#include "Game/Entities/Gui/DialogBox.h"

int StringReplaceAll (string& subject, const string& pattern,
                       const string& replace) {
    int numReplaces = 0;
    size_t pos = -1;
    while (true) {
        pos = subject.find(pattern, pos + 1);
        if (pos == string::npos)
            break;
        subject.replace(pos, pattern.size(), replace);
        numReplaces++;
    }
    return numReplaces;
}

void DialogBox::ProcessText (string& text, Volt::FontAssetRef font,
                             float maxWidth) {
    // Convert newlines to spaces.
    StringReplaceAll(text, "\n", " ");
    // Collapse multiple spaces.
    while (StringReplaceAll(text, "  ", " ") > 0);

    // Word wrap.

    // Keeps track of the start index of the line. Updated when a newline
    // is added.
    size_t startPos = 0;

    // Keeps track of the next found space.
    size_t pos = -1;

    // Keeps track of the space found before. When we detect that a string
    // has overflowed, we add a space there and backtrack.
    size_t oldPos;
    while (true) {
        oldPos = pos;
        pos = text.find(" ", pos + 1);
        if (pos == string::npos)
            break;
        string substring = text.substr(startPos, pos - startPos);
        if (font->GetTextWidth(substring) > maxWidth) {
            // Replace space with newline at oldPos.
            text.replace(oldPos, 1, "\n");
            startPos = oldPos + 1;
            pos = oldPos;
        }
    }

    // Handle last word.
    string substring = text.substr(startPos);
    if (font->GetTextWidth(substring) > maxWidth) {
        // Replace space with newline at oldPos.
        text.replace(oldPos, 1, "\n");
    }
}
