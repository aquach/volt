#include "Filter.h"
#include "OpenGL.h"
#include "Graphics.h"
#include "GpuProgram.h"
#include "Window.h"

namespace Volt {

Filter::Filter (GpuProgram* program) :
    m_layer(0),
    m_colorMap(0),
    m_program(program) {

    glGenTextures(1, (GLuint*)&m_colorMap);
    glBindTexture(GL_TEXTURE_2D, m_colorMap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Filter::~Filter () {
    glDeleteTextures(1, (GLuint*)&m_colorMap);
    delete m_program;
}

void Filter::SetUniforms () {
    Graphics::SetShaderValue("colorMap", 0);
}

void Filter::Render () {
    glBindTexture(GL_TEXTURE_2D, m_colorMap);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, G_Window->width(),
                     G_Window->height(), 0);

    Graphics::Clear();
    Graphics::BindShader(m_program);
    glBindTexture(GL_TEXTURE_2D, m_colorMap);
    SetUniforms();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1, 0, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    Graphics::SetColor(Color::white);
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0);
    glVertex2i(0, 0);
    glTexCoord2i(1, 0);
    glVertex2i(1, 0);
    glTexCoord2i(1, 1);
    glVertex2i(1, 1);
    glTexCoord2i(0, 1);
    glVertex2i(0, 1);
    glEnd();

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    Graphics::BindShader(NULL);
}

}
