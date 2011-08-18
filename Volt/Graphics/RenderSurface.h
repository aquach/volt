#pragma once

#include "Volt/Core/Core.h"
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

    bool IsBound () const;

    int width () const { return m_width; }
    int height () const { return m_height; }

    static void RenderPass ();
    static void CheckStatus ();
    static GLint boundFBO ();

private:
    GLuint m_fbo;
    GLuint m_depthBuffer;
    GLuint m_texture;
    int m_width;
    int m_height;

    DISALLOW_COPY_AND_ASSIGN(RenderSurface);
};

}
