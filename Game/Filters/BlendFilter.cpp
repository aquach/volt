#include "Game/Filters/BlendFilter.h"
#include "Volt/Graphics/GpuProgram.h"
#include "Volt/Graphics/RenderSurface.h"
#include "Volt/Graphics/Viewport.h"
#include "Game/Graphics/Graphics.h"

BlendFilter::BlendFilter (const string& name, int bottomLayer, int topLayer)
    : Volt::Filter(name, bottomLayer, topLayer),
      m_shader(NULL),
      m_buffer(NULL),
      m_amount(0),
      m_blendColor(Volt::Color::white) {
}

void BlendFilter::OnAdded () {
    m_buffer = new Volt::RenderSurface(G_Viewport->width(),
                                       G_Viewport->height(), false,
                                       false, true);
    m_shader = new Volt::GpuProgram;
    m_shader->Attach(
        G_AssetManager->GetShader("Shaders/standard.vert",
                                        Volt::ShaderAsset::SHADER_VERTEX));
    m_shader->Attach(
        G_AssetManager->GetShader("Shaders/blend.frag",
                                        Volt::ShaderAsset::SHADER_FRAGMENT));
}

BlendFilter::~BlendFilter () {
    delete m_buffer;
    delete m_shader;
}

void BlendFilter::OnBottomLayer () {
    m_buffer->Bind();
    Graphics::Clear();
}

void BlendFilter::OnTopLayer () {
    CHECK(m_buffer->IsBound()) << "FBO was changed during filter execution.";
    m_buffer->Unbind();

    glBindTexture(GL_TEXTURE_2D, m_buffer->texture());
    Graphics::BindShader(m_shader);
    Graphics::SetShaderValue("blendColor", m_blendColor);
    Graphics::SetShaderValue("amount", m_amount);
    Volt::RenderSurface::RenderPass();
    Graphics::BindShader(NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
}
