#include "Game/Python/ScriptConsole.h"
#include <Python.h>
#include "Volt/Game/AppTime.h"
#include "Volt/Game/Scene.h"
#include "Volt/Graphics/OpenGL.h"
#include "Volt/Graphics/Viewport.h"
#include "Volt/GUI/Label.h"
#include "Game/Graphics/Graphics.h"

const uint MAX_HISTORY = 20;
const float BLINK_CYCLE = 1.0f;

ScriptConsole::ScriptConsole ()
    : m_historyLocation(0),
      m_cursor(0),
      m_blinkTimer(0),
      m_label(NULL) {
    setVisible(false);
}

ScriptConsole::~ScriptConsole () {
}

void ScriptConsole::OnAdded () {
    m_label = new Volt::Label(
                    G_AssetManager->GetFont("Fonts/Inconsolata.ttf", 24),
                    10, G_Viewport->height() - 10);
    m_label->setVisible(false);
    m_label->SetAnchorY(Volt::Label::ANCHOR_BOTTOM);
    m_label->SetColor(Volt::Color::white);
    scene()->Add(m_label, -51);
}

void ScriptConsole::OnRemoved () {
    scene()->Remove(m_label);
}

void ScriptConsole::Update () {
    m_blinkTimer += G_Time->dt();
    if (m_blinkTimer >= BLINK_CYCLE)
        m_blinkTimer = 0.0f;
}

void ScriptConsole::Render () {
    glPushMatrix();
    glLoadIdentity();
    Graphics::Translate(Vector2(G_Viewport->width() / 2,
                                G_Viewport->height() - 20));
    Graphics::SetColor(Volt::Color::RGB(100, 0, 0));
    Graphics::RenderQuad(G_Viewport->width(), 40);

    if (m_blinkTimer <= BLINK_CYCLE * 0.5f) {
        glLoadIdentity();
        Graphics::SetColor(Volt::Color::white);
        glLineWidth(1.0f);
        string before = m_label->text().substr(0, m_cursor);
        float x = m_label->position().x + m_label->font()->GetTextWidth(before)
            + 2;
        float y = m_label->position().y;
        Graphics::RenderLine(Vector2(x, y - 20), Vector2(x, y + 5));
    }
    glPopMatrix();
}

bool ScriptConsole::OnKeyEvent (SDL_KeyboardEvent event) {
    if (event.type != SDL_KEYDOWN)
        return false;
    if (m_label == NULL)
        return false;

    if (event.keysym.sym == SDLK_BACKQUOTE) {
        setVisible(!visible());
        m_label->setVisible(visible());
        // Enable unicode to receive ASCII values.
        SDL_EnableUNICODE(visible());
        if (visible()) {
            SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,
                                SDL_DEFAULT_REPEAT_INTERVAL);
        } else {
            SDL_EnableKeyRepeat(0, 0);
        }
        return true;
    }

    if (!visible())
        return false;

    m_blinkTimer = 0.0f;

    switch (event.keysym.sym) {
        case SDLK_RETURN: {
            // Execute code.
            if (m_history.size() > MAX_HISTORY)
                m_history.pop_front();
            string text = m_label->text();
            m_history.push_back(text);

            LOG(INFO) << ">>> " << text;

            replace(text.begin(), text.end(), ';', '\n');
            PyRun_SimpleString(text.c_str());
            m_label->SetText("");
            m_historyLocation = 0;
            m_cursor = 0;
            return true;
        }
        break;
        case SDLK_UP:
            if (m_history.size() > 0) {
                m_label->SetText(
                    m_history[m_history.size() - 1 - m_historyLocation]);
                if (m_historyLocation < (int)m_history.size() - 1)
                    m_historyLocation++;
                m_cursor = m_label->text().size();
                return true;
            }
        break;
        case SDLK_LEFT:
            if (m_cursor > 0)
                m_cursor--;
            return true;
        break;
        case SDLK_RIGHT:
            if (m_cursor < (int)m_label->text().size())
                m_cursor++;
            return true;
        break;
        case SDLK_DOWN:
            if (m_history.size() > 0) {
                m_label->SetText(
                    m_history[m_history.size() - 1 - m_historyLocation]);
                m_cursor = m_label->text().size();
                if (m_historyLocation > 0)
                    m_historyLocation--;
                return true;
            }
        break;
        case SDLK_BACKSPACE: {
            string text = m_label->text();
            if (text.size() > 0 && m_cursor > 0) {
                text = text.substr(0, m_cursor - 1) +
                       text.substr(m_cursor, text.size() - 1);
                m_cursor--;
                m_label->SetText(text);
            }
            return true;
        }
        break;
        case SDLK_HOME:
            m_cursor = 0;
        break;
        case SDLK_END:
            m_cursor = m_label->text().size();
        break;
        default: {
            if ((event.keysym.unicode & 0xFF80) == 0) {
                char ch = event.keysym.unicode & 0x7F;
                if (ch != 0 && ch != 127) {
                    string text = m_label->text();
                    text = text.substr(0, m_cursor) + ch +
                           text.substr(m_cursor, text.size() - 1);
                    m_label->SetText(text);
                    m_cursor++;
                }
            }
            return true;
        }
        break;
    }
    return false;
}


