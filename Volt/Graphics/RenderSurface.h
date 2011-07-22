#pragma once

#include "Volt/Core/Core.h"
#include "Volt/Assets/AssetManager.h"
#include "Volt/Graphics/OpenGL.h"

namespace Volt {

class RenderSurface {
public:
    RenderSurface (int width, int height, bool depth, bool floatingPoint,
                   bool linearFilter);
    ~RenderSurface ();

    GLuint texture () const { return m_texture; }

    void Bind ();
    void Unbind ();

    static void RenderPass ();
    static void CheckStatus ();

private:
    GLuint m_fbo;
    GLuint m_depthBuffer;
    GLuint m_texture;

    DISALLOW_COPY_AND_ASSIGN(RenderSurface);
};

}
