#include "Game/Python/ScriptConsole.h"
#include "Volt/Game/Scene.h"
#include "Game/Graphics/Graphics.h"
#include "Volt/Graphics/OpenGL.h"
#include "Volt/Graphics/Viewport.h"
#include "Volt/GUI/Label.h"
#include <Python.h>

const uint MAX_HISTORY = 20;

ScriptConsole::ScriptConsole (Volt::Scene* scene)
    : m_visible(false),
      m_scene(scene),
      m_historyLocation(0) {
    m_label = new Volt::Label(
                    G_AssetManager->GetFont("Fonts/Inconsolata.ttf", 24),
                    10, G_Viewport->height() - 10);
    m_label->setVisible(false);
    m_label->SetAnchorY(Volt::Label::ANCHOR_BOTTOM);
    m_label->SetColor(Volt::Color::white);
}

ScriptConsole::~ScriptConsole () {
    delete m_label;
}

void ScriptConsole::Render () {
    if (!m_visible)
        return;

    if (m_label->scene() == NULL)
        m_scene->Add(m_label, -50);

    glPushMatrix();
    glLoadIdentity();
    Graphics::Translate(Vector2(G_Viewport->width() / 2, G_Viewport->height() - 20));
    Graphics::SetColor(Volt::Color::RGB(100, 0, 0));
    Graphics::RenderQuad(G_Viewport->width(), 40);
    glPopMatrix();
}

bool ScriptConsole::OnKeyEvent (SDL_KeyboardEvent event) {
    if (event.type != SDL_KEYDOWN)
        return false;

    switch (event.keysym.sym) {
        case SDLK_BACKQUOTE:
            m_visible = !m_visible;
            m_label->setVisible(m_visible);
            // Enable unicode to receive ASCII values.
            SDL_EnableUNICODE(m_visible);
            if (m_visible) {
                SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,
                                    SDL_DEFAULT_REPEAT_INTERVAL);
            } else {
                SDL_EnableKeyRepeat(0, 0);
            }
            return true;
        break;
        case SDLK_RETURN:
            if (m_visible) {
                // Execute code.
                if (m_history.size() > MAX_HISTORY)
                    m_history.pop_front();
                string text = m_label->text();
                m_history.push_back(text);

                replace(text.begin(), text.end(), ';', '\n');
                PyRun_SimpleString(text.c_str());
                m_label->SetText("");
                m_historyLocation = 0;
                return true;
            }
        break;
        case SDLK_UP:
            if (m_visible && m_history.size() > 0) {
                m_label->SetText(
                    m_history[m_history.size() - 1 - m_historyLocation]);
                if (m_historyLocation < (int)m_history.size() - 1)
                    m_historyLocation++;
                return true;
            }
        break;
        case SDLK_DOWN:
            if (m_visible && m_history.size() > 0) {
                m_label->SetText(
                    m_history[m_history.size() - 1 - m_historyLocation]);
                if (m_historyLocation > 0)
                    m_historyLocation--;
                return true;
            }
        break;
        case SDLK_BACKSPACE: {
            if (m_visible) {
                string text = m_label->text();
                if (text.size() > 0) {
                    text = text.substr(0, text.size() - 1);
                    m_label->SetText(text);
                }
                return true;
            }
        }
        break;
        default: {
            if (m_visible) {
                if ((event.keysym.unicode & 0xFF80) == 0) {
                    char ch = event.keysym.unicode & 0x7F;
                    if (ch != 0) {
                        m_label->SetText(m_label->text() + ch);
                    }
                }
                return true;
            }
        }
        break;
    }
    return false;
}


