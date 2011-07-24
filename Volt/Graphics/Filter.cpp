#include "Volt/Graphics/Filter.h"
#include "Volt/Graphics/OpenGL.h"
#include "Volt/Graphics/Graphics.h"
#include "Volt/Graphics/GpuProgram.h"
#include "Volt/Graphics/RenderSurface.h"
#include "Volt/Graphics/Viewport.h"

namespace Volt {

Filter::Filter (GpuProgram* program) :
    m_layer(0),
    m_colorMap(0),
    m_program(program) {

    glGenTextures(1, (GLuint*)&m_colorMap);
    glBindTexture(GL_TEXTURE_2D, m_colorMap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Filter::~Filter () {
    glDeleteTextures(1, (GLuint*)&m_colorMap);
    delete m_program;
}

void Filter::SetUniforms () {
    glBindTexture(GL_TEXTURE_2D, m_colorMap);
    Graphics::SetShaderValue("colorMap", 0);
    int count = 1;
    FOR_(Maps::iterator, i, m_maps) {
        glActiveTexture(GL_TEXTURE0 + count);
        glBindTexture(GL_TEXTURE_2D, i->second->glID());
        Graphics::SetShaderValue(i->first.c_str(), count);
        count++;
    }
    glActiveTexture(GL_TEXTURE0);
}

void Filter::AddMap (string name, TextureAssetRef texture) {
    m_maps[name] = texture;
}

void Filter::Render () {
    glBindTexture(GL_TEXTURE_2D, m_colorMap);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, G_Viewport->width(),
                     G_Viewport->height(), 0);

    Graphics::Clear();
    Graphics::BindShader(m_program);
    SetUniforms();

    RenderSurface::RenderPass();

    Graphics::BindShader(NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
}

}
