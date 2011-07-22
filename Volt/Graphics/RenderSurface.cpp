#include "Volt/Graphics/RenderSurface.h"
#include "Volt/Graphics/Graphics.h"

namespace Volt {

static char FRAMEBUFFER_STATUS_ENUMS[][20] = {
    /* All framebuffer attachment points are 'framebuffer attachment
     * complete'. */
    "attachment",
    // There is at least one image attached to the framebuffer.
    "missing attachment",
    ""
    // All attached images have the same width and height.
    "dimensions",
    /* All images attached to the attachment points COLOR_ATTACHMENT0_EXT
       through COLOR_ATTACHMENTn_EXT must have the same internal format. */
    "formats",
    /* The value of FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT must not be NONE
     * for any color attachment point(s) named by DRAW_BUFFERi. */
    "draw buffer",
    /* If READ_BUFFER is not NONE, then the value of
     * FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT must not be NONE for the
     * color attachment point named by READ_BUFFER. */
    "read buffer",
    /* The combination of internal formats of the attached images does not
     * violate an implementation-dependent set of restrictions. */
    "unsupported format"
};

void RenderSurface::CheckStatus () {
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        int index = status - GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT;
        LOG(ERROR) << "Frame buffer was not fully configured because of "
            << FRAMEBUFFER_STATUS_ENUMS[index] << ".";
    }
}

RenderSurface::RenderSurface (int width, int height, bool depth,
                              bool floatingPoint, bool linearFilter)
    : m_fbo(0),
      m_depthBuffer(0),
      m_texture(0),
      m_width(width),
      m_height(height) {

    GLenum internalFormat = floatingPoint ? GL_RGBA16F_ARB : GL_RGBA;
    GLenum type = floatingPoint ? GL_HALF_FLOAT_ARB : GL_UNSIGNED_BYTE;
    GLenum filter = linearFilter ? GL_LINEAR : GL_NEAREST;

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA,
                 type, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (depth) {
        glGenRenderbuffers(1, &m_depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width,
                              height);
    }

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           m_texture, 0);
    if (depth) {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                  GL_RENDERBUFFER, m_depthBuffer);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    CheckStatus();

}

RenderSurface::~RenderSurface () {
    glDeleteTextures(1, &m_texture);
    glDeleteFramebuffers(1, &m_fbo);
}

void RenderSurface::Bind () {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void RenderSurface::Unbind () {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Renders a quad across the entire screen.
void RenderSurface::RenderPass () {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1, 0, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    Graphics::SetColor(Volt::Color::white);
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

    glMatrixMode(GL_MODELVIEW);
}

}
