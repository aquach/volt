#include "Game/Entities/GUI/MessageBox.h"
#include "Game/Graphics/Graphics.h"
#include "Volt/Game/AppTime.h"
#include "Volt/Graphics/Viewport.h"

const float HEIGHT = 270;
const float MARGIN = 40;
const float TEXT_MARGIN = 50;
const float CHARACTERS_PER_SECOND = 40;
const float SECONDS_PER_CHARACTER = 1 / CHARACTERS_PER_SECOND;

MessageBox::MessageBox (const MessageBoxDef& def)
    : m_def(def),
      m_nextCharTimer(0),
      m_currentCharacter(0),
      m_finished(false),
      m_nextCursorTimer(false),
      m_pauseTimer(-1) {
};

void MessageBox::Skip () {
    while (HasCharactersRemaining()) {
        m_textStream << m_def.text[m_currentCharacter++];
    }
}

void MessageBox::Update () {
    if (HasCharactersRemaining()) {
        m_nextCharTimer -= G_Time->dt();
        if (m_nextCharTimer <= 0) {
            m_nextCharTimer = SECONDS_PER_CHARACTER;
            m_textStream << m_def.text[m_currentCharacter++];
        }
    } else {
        m_nextCursorTimer += G_Time->dt();
        if (!m_def.canSkip) {
            // Pause timer only applies to unskippables.
            if (m_pauseTimer == -1) {
                // Start pause timer.
                m_pauseTimer = m_def.pauseDuration;
            } else {
                m_pauseTimer -= G_Time->dt();
                if (m_pauseTimer <= 0) {
                    m_finished = true;
                }
            }
        }
    }
}

bool MessageBox::HasCharactersRemaining () const {
    return m_currentCharacter < (int)m_def.text.size();
}

bool MessageBox::IsFinished () const {
    return m_finished;
}

void MessageBox::OnKeyEvent (SDL_KeyboardEvent event) {
    if (m_def.canSkip) {
        if (event.keysym.sym == SDLK_z && event.type == SDL_KEYDOWN) {
            if (HasCharactersRemaining())
                Skip();
            else
                m_finished = true;
        }
    }
}

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

void MessageBox::ProcessText () {
    string& str = m_def.text;

    // Convert newlines to spaces.
    StringReplaceAll(str, "\n", " ");
    // Collapse multiple spaces.
    while (StringReplaceAll(str, "  ", " ") > 0);

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
        pos = str.find(" ", pos + 1);
        if (pos == string::npos)
            break;
        string substring = str.substr(startPos, pos - startPos);
        if (m_font->GetTextWidth(substring) > textLineMaxWidth()) {
            // Replace space with newline at oldPos.
            str.replace(oldPos, 1, "\n");
            startPos = oldPos + 1;
            pos = oldPos;
        }
    }

    // Handle last word.
    string substring = str.substr(startPos);
    if (m_font->GetTextWidth(substring) > textLineMaxWidth()) {
        // Replace space with newline at oldPos.
        str.replace(oldPos, 1, "\n");
    }
}

int MessageBox::textLineMaxWidth () {
    return G_Viewport->width() - MARGIN * 2 - TEXT_MARGIN * 2;
}

void MessageBox::Render () {
    glPushMatrix();
    glLoadIdentity();

    float windowWidth = G_Viewport->width();
    float windowHeight = G_Viewport->height();
    Volt::BBox box(
        Vector2(0, windowHeight - HEIGHT),
        Vector2(windowWidth, windowHeight)
    );
    box = box.Expand(-MARGIN);
    Vector2 lowerRight = box.max;
    Graphics::SetBlend(Graphics::BLEND_ALPHA);
    Graphics::SetColor(Volt::Color::RGBA(200, 200, 255, 85));
    Graphics::Translate(box.center());
    Graphics::RenderQuad(box.extents().x, box.extents().y);
    glLoadIdentity();

    Volt::BBox textBox = box.Expand(-TEXT_MARGIN);
    Graphics::SetColor(Volt::Color::RGB(0, 0, 0));
    Graphics::RenderText(m_font, m_textStream.str(), textBox.min.x,
                         textBox.min.y);

    if (!HasCharactersRemaining() && m_def.canSkip) {
        float blinkCursor = fmodf(m_nextCursorTimer, 1.0f);
        if (blinkCursor <= 0.5f) {
            Graphics::SetColor(Volt::Color::RGB(210, 210, 210));
            Vector2 center = lowerRight - Vector2(35, 35);
            glPushMatrix();
            Graphics::Translate(center);
            Graphics::RenderQuad(20, 30);
            glPopMatrix();
        }
    }

    glPopMatrix();
}
