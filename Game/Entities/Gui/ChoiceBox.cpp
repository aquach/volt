#include "Game/Entities/Gui/ChoiceBox.h"
#include "Game/Graphics/Graphics.h"
#include "Volt/Game/AppTime.h"
#include "Volt/Graphics/Viewport.h"

const float HEIGHT = 270;
const float MARGIN = 40;
const float TEXT_MARGIN = 50;
const float CHARACTERS_PER_SECOND = 40;
const float SECONDS_PER_CHARACTER = 1 / CHARACTERS_PER_SECOND;
const float CURSOR_SPACE = 20;
const float CHOICE_SPACE = 45;

ChoiceBox::ChoiceBox (const ChoiceBoxDef& def)
    : m_def(def),
      m_nextCharTimer(0),
      m_currentCharacter(0),
      m_choice(-1) {
    CHECK_GT(def.choices.size(), 0);
};

void ChoiceBox::Skip () {
    while (HasCharactersRemaining()) {
        m_textStream << m_def.text[m_currentCharacter++];
    }
}

void ChoiceBox::OnAdded () {
    DialogBox::ProcessText(m_def.text, m_font, textLineMaxWidth());
}

void ChoiceBox::Update () {
    if (HasCharactersRemaining()) {
        m_nextCharTimer -= G_Time->dt();
        if (m_nextCharTimer <= 0) {
            m_nextCharTimer = SECONDS_PER_CHARACTER;
            m_textStream << m_def.text[m_currentCharacter++];
        }
    } else if (m_choice == -1) {
        // Choices are shown, select 0 if none was selected before.
        m_choice = 0;
    }
}

bool ChoiceBox::HasCharactersRemaining () const {
    return m_currentCharacter < (int)m_def.text.size();
}

void ChoiceBox::OnKeyEvent (SDL_KeyboardEvent event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.keysym.sym) {
            case SDLK_z:
                if (HasCharactersRemaining())
                    Skip();
                else
                    OnFinished();
            break;
            case SDLK_LEFT:
                if (m_choice > 0)
                    m_choice--;
            break;
            case SDLK_RIGHT:
                if (m_choice < (int)m_def.choices.size() - 1)
                    m_choice++;
            break;
            default:
            break;
        }
    }
}

int ChoiceBox::textLineMaxWidth () {
    return G_Viewport->width() - MARGIN * 2 - TEXT_MARGIN * 2;
}

void ChoiceBox::Render () {
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

    if (!HasCharactersRemaining()) {
        float x = textBox.min.x + CURSOR_SPACE;
        float y = textBox.max.y - 20;

        for (uint i = 0; i < m_def.choices.size(); i++) {
            Graphics::SetColor(Volt::Color::RGB(120, 0, 0));
            const string& text = m_def.choices[i];
            Graphics::RenderText(m_font, text, x, y);

            if ((int)i == m_choice) {
                // Draw cursor pointing at this choice, to the left and
                // vertically centered on the text.
                glPushMatrix();
                Vector2 pos(x - CURSOR_SPACE,
                            y - m_font->GetTextHeight(text) / 2);
                Graphics::Translate(pos);
                Graphics::SetColor(Volt::Color::RGB(20, 20, 175));
                glBegin(GL_TRIANGLES);
                glVertex2f(0, -15);
                glVertex2f(15, 0);
                glVertex2f(0, 15);
                glEnd();
                glPopMatrix();
            }

            x += m_font->GetTextWidth(text) + CURSOR_SPACE + CHOICE_SPACE;
        }

    }

    glPopMatrix();
}
