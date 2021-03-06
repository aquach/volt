#include "Game/Filters/EdgeFilter.h"
#include "Volt/Graphics/GpuProgram.h"
#include "Volt/Graphics/RenderSurface.h"
#include "Volt/Graphics/Viewport.h"
#include "Game/Graphics/Graphics.h"

EdgeFilter::EdgeFilter (const string& name, int bottomLayer, int topLayer)
    : Volt::Filter(name, bottomLayer, topLayer),
      m_shader(NULL),
      m_buffer(NULL) {
}

void EdgeFilter::OnAdded () {
    m_buffer = new Volt::RenderSurface(G_Viewport->width(),
                                       G_Viewport->height(), false,
                                       false, true);
    m_shader = new Volt::GpuProgram;
    m_shader->Attach(
        G_AssetManager->GetShader("Shaders/standard.vert",
                                        Volt::ShaderAsset::SHADER_VERTEX));
    m_shader->Attach(
        G_AssetManager->GetShader("Shaders/celshade.frag",
                                        Volt::ShaderAsset::SHADER_FRAGMENT));
}

EdgeFilter::~EdgeFilter () {
    delete m_buffer;
    delete m_shader;
}

void EdgeFilter::OnBottomLayer () {
    m_buffer->Bind();
    Graphics::Clear();
}

void EdgeFilter::OnTopLayer () {
    CHECK(m_buffer->IsBound()) << "FBO was changed during filter execution.";
    m_buffer->Unbind();

    glBindTexture(GL_TEXTURE_2D, m_buffer->texture());
    Graphics::BindShader(m_shader);
    Vector2 pixelSize(1.0f / G_Viewport->width(), 1.0f / G_Viewport->height());
    Graphics::SetShaderValue("pixelSize", pixelSize);
    Volt::RenderSurface::RenderPass();
    Graphics::BindShader(NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
}
