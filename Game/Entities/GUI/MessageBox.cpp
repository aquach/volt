#include "MessageBox.h"
#include "Graphics/Graphics.h"
#include "Volt/Game/Game.h"
#include "Volt/Graphics/Viewport.h"

const float HEIGHT = 250;
const float MARGIN = 40;
const float TEXT_MARGIN = 50;
const float CHARACTERS_PER_SECOND = 40;
const float SECONDS_PER_CHARACTER = 1 / CHARACTERS_PER_SECOND;

MessageBox::MessageBox (const MessageBoxDef& def)
    : m_def(def),
      m_nextCharTimer(0),
      m_currentCharacter(0),
      m_finished(false) {
};

void MessageBox::Skip () {
    while (HasCharactersRemaining()) {
        m_textStream << m_def.text[m_currentCharacter++];
    }
}
    
void MessageBox::Update () {
    if (HasCharactersRemaining()) {
        m_nextCharTimer -= Volt::G_Game->dt();
        if (m_nextCharTimer <= 0) {
            m_nextCharTimer = SECONDS_PER_CHARACTER;
            m_textStream << m_def.text[m_currentCharacter++];
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
    Graphics::SetBlend(Graphics::BLEND_ALPHA);
    Graphics::SetColor(Volt::Color::RGBA(150, 150, 200, 70));
    Graphics::Translate(box.center());
    Graphics::RenderQuad(box.extents().x, box.extents().y);
    glLoadIdentity();
    
    Volt::BBox textBox = box.Expand(-TEXT_MARGIN);
    Graphics::SetColor(Volt::Color::RGB(0, 0, 0));
    Graphics::RenderText(m_font, m_textStream.str(), textBox.min.x, textBox.min.y);

    glPopMatrix();
}
