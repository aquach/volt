#include "Volt/Graphics/Graphics.h"
#include "Volt/Assets/AssetManager.h"
#include "Volt/Graphics/GpuProgram.h"
#include "Volt/Graphics/OpenGL.h"
#include "Volt/Graphics/SDLWindow.h"
#include "Volt/Graphics/Viewport.h"

namespace Volt {

Graphics* Graphics::instance = NULL;

Graphics::Graphics (Viewport* viewport)
    : m_program(NULL),
      m_viewport(viewport),
      m_initialized(false) {
    instance = this;
    currentBlend = BLEND_NONE;
}

void Graphics::Clear () {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::Init () {
    LOG(INFO) << "Initializing graphics...";
    if (GLEW_OK != glewInit()) {
        LOG(ERROR) << "Failed to initialize glewInit!";
    }

    glEnable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glCullFace(GL_BACK);
    //glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_initialized = true;

    ShowBuffer();
}

void Graphics::SetBackgroundColor (const Color& color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

Color Graphics::GetBackgroundColor () {
    GLfloat *clearColor = new GLfloat[4];
    glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
    Color color(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    delete[] clearColor;
    return color;
}

void Graphics::BindShader (GpuProgram* program) {
    if (program != NULL) {
        if (program != instance->m_program) {
            if (!program->IsLinked())
                program->Link();
            glUseProgramObjectARB(program->handle());
            instance->m_program = program;
        }
    } else {
        glUseProgramObjectARB(0);
        instance->m_program = NULL;
    }
}

int Graphics::GetUniformLocation (const char* s) {
    if (instance->m_program != NULL)
        return glGetUniformLocationARB(instance->m_program->handle(), s);
    return 0;
}

void Graphics::SetShaderValue (const char* valueName, Color value) {
    if (instance->m_program != NULL) {
        GLuint location = GetUniformLocation(valueName);
        if ((int)location == -1) {
            LOG(WARNING) << "Could not get location of uniform "
                         << valueName;
        }
        glUniform4fARB(location, value.r, value.g, value.b, value.a);
    }
}

void Graphics::SetShaderValue (const char* valueName, Vector2 value) {
    if (instance->m_program != NULL) {
        GLuint location = GetUniformLocation(valueName);
        if ((int)location == -1) {
            LOG(WARNING) << "Could not get location of uniform "
                         << valueName;
        }
        glUniform2fARB(location, value.x, value.y);
    }
}

void Graphics::SetShaderValue (const char* valueName, float value) {
    if (instance->m_program != NULL) {
        GLuint location = GetUniformLocation(valueName);
        if ((int)location == -1) {
            LOG(WARNING) << "Could not get location of uniform "
                         << valueName;
        }
        glUniform1fARB(location, value);
    }
}

void Graphics::SetShaderValue (const char* valueName, int value) {
    if (instance->m_program != NULL) {
        GLuint location = GetUniformLocation(valueName);
        if ((int)location == -1) {
            LOG(WARNING) << "Could not get location of uniform "
                         << valueName;
        }
        glUniform1iARB(location, value);
    }
}

void Graphics::SetBlend (BlendType blend)
{
    if (blend != instance->currentBlend) {
        switch (blend) {
            case BLEND_NONE:
                glBlendFunc(GL_ONE, GL_ZERO);
                break;
            case BLEND_ALPHA:
                glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
                break;
            case BLEND_ADDITIVE:
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                break;
            case BLEND_MULTIPLY:
                glBlendFunc(GL_ZERO, GL_SRC_COLOR);
                break;
        }
        instance->currentBlend = blend;
    }
}

void Graphics::Set2D (int virtualWidth, int virtualHeight) {
    instance->virtualWidth = virtualWidth;
    instance->virtualHeight = virtualHeight;

    glViewport(0, 0, virtualWidth, virtualHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0f, instance->m_viewport->width(), instance->m_viewport->height(),
            0.0f, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    instance->resolutionScale = Vector2(
        float(instance->m_viewport->width()) / virtualWidth,
        float(instance->m_viewport->width()) / virtualWidth);

    instance->screenCenter = Vector2(virtualWidth / 2, virtualHeight / 2);
}

const Vector2& Graphics::GetResolutionScale () {
    return instance->resolutionScale;
}

void Graphics::Translate (const Vector2& pos) {
    glTranslatef(pos.x, pos.y, 0.0f);
}

void Graphics::Scale (const Vector2& scale) {
    glScalef(scale.x, scale.y, 1.0f);
}

void Graphics::TransformMatrix (const Transform& transform) {
    glTranslatef(transform.position.x, transform.position.y, 0.0f);
    glRotatef(transform.rotation, 0.0f, 0.0f, 1.0f);
    glScalef(transform.scale.x, transform.scale.y, 1.0f);
}

void Graphics::SetColor (const Color& color) {
    glColor4f(color.r, color.g, color.b, color.a);
}

/*
void Graphics::RenderQuad (float w, float h) {
    float hw = w * 0.5f;
    float hh = h * 0.5f;

    glBegin(GL_QUADS);
        glVertex3f(-w, -h, 0.0f);
        glTexCoord2f(1, 1);
        glVertex3f(w, -h, 0.0f);
        glTexCoord2f(1, 0);
        glVertex3f(w, h, 0.0f);
        glTexCoord2f(0, 0);
        glVertex3f(-w, h, 0.0f);
        glTexCoord2f(0, 1);
    glEnd();
}
*/
void Graphics::RenderLine (const Vector2& pos1, const Vector2& pos2) {
    glBegin(GL_LINES);
        glVertex3f(pos1.x, pos1.y, 0.0f);
        glVertex3f(pos2.x, pos2.y, 0.0f);
    glEnd();
}

void Graphics::RenderLineRect (float x, float y, float w, float h) {
    float hw = w*0.5f;
    float hh = h*0.5f;

    glBegin(GL_LINES);
        glVertex3f(x-hw, y-hh, 0.0f);
        glVertex3f(x+hw, y-hh, 0.0f);

        glVertex3f(x+hw, y-hh, 0.0f);
        glVertex3f(x+hw, y+hh, 0.0f);

        glVertex3f(x+hw, y+hh, 0.0f);
        glVertex3f(x-hw, y+hh, 0.0f);

        glVertex3f(x-hw, y+hh, 0.0f);
        glVertex3f(x-hw, y-hh, 0.0f);
    glEnd();
}

void Graphics::RenderLineRect (Vector2 pos1, Vector2 pos2) {
    BBox b(pos1, pos2);
    RenderLineRect(b);
}

void Graphics::RenderLineRect (const BBox& box) {
    RenderLineRect(box.center().x, box.center().y,
                   box.max.x - box.min.x, box.max.y - box.min.y);
}

Vector2 Graphics::GetScreenCenter() {
    return instance->screenCenter;
}

/*
void Graphics::RenderQuadCustom(const Vector2 &ul, const Vector2 &ur, const Vector2 &lr, const Vector2 &ll, const Vector2 &textureOffset, const Vector2 &textureScale) {
    glBegin(GL_QUADS);
        // UL
        glTexCoord2f(textureOffset.x, textureOffset.y);
        glVertex3f(ul.x, ul.y, 0.0f);

        //UR
        glTexCoord2f(textureOffset.x + textureScale.x, textureOffset.y);
        glVertex3f(ur.x, ur.y, 0.0f);

        //LR
        glTexCoord2f(textureOffset.x + textureScale.x, textureOffset.y + textureScale.y);
        glVertex3f(lr.x, lr.y, 0.0f);

        //LL
        glTexCoord2f(textureOffset.x, textureOffset.y + textureScale.y);
        glVertex3f(ll.x, ll.y, 0.0f);
    glEnd();
}
*/

void Graphics::RenderTriangle (float width, float height,
                               const Vector2& textureOffset,
                               const Vector2& textureScale,
                               const Vector2& position) {
    glBegin(GL_TRIANGLES);
        glTexCoord2f(textureOffset.x, textureOffset.y);
        glVertex3f(position.x, position.y, 0.0f);

        glTexCoord2f(textureOffset.x + textureScale.x, textureOffset.y);
        glVertex3f(position.x + width, position.y, 0.0f);

        glTexCoord2f(textureOffset.x, textureOffset.y + textureScale.y);
        glVertex3f(position.x, position.y + height, 0.0f);
    glEnd();
}

void Graphics::RenderQuad (float width, float height,
                           const Vector2 &textureOffset,
                           const Vector2 &textureScale,
                           const Vector2 &position) {

    float halfWidth = width * 0.5f;
    float halfHeight = height * 0.5f;

    glBegin(GL_QUADS);
        // UL
        glTexCoord2f(textureOffset.x, textureOffset.y);
        glVertex3f(-halfWidth + position.x, -halfHeight + position.y, 0.0f);

        //UR
        glTexCoord2f(textureOffset.x + textureScale.x, textureOffset.y);
        glVertex3f(halfWidth + position.x, -halfHeight + position.y, 0.0f);

        //LR
        glTexCoord2f(textureOffset.x + textureScale.x, textureOffset.y + textureScale.y);
        glVertex3f(halfWidth + position.x, halfHeight + position.y, 0.0f);

        //LL
        glTexCoord2f(textureOffset.x, textureOffset.y + textureScale.y);
        glVertex3f(-halfWidth + position.x, halfHeight + position.y, 0.0f);
    glEnd();
}

void Graphics::RenderText (FontAssetRef font, const string& text,
                           float x, float y) {
    glEnable(GL_BLEND);
    BindFont(font);
    BBox verts, texCoords;
    glBegin(GL_QUADS);
    for (uint i = 0; i < text.size(); i++)
    {
        char c = text[i];
        if ((c >= 32) && (c < 128))
        {
            font->GetGlyphData(c, &x, &y, &verts, &texCoords);

            glTexCoord2f(texCoords.min.x, texCoords.min.y);
            glVertex2f(verts.min.x, verts.min.y);

            glTexCoord2f(texCoords.max.x, texCoords.min.y);
            glVertex2f(verts.max.x, verts.min.y);

            glTexCoord2f(texCoords.max.x, texCoords.max.y);
            glVertex2f(verts.max.x, verts.max.y);

            glTexCoord2f(texCoords.min.x, texCoords.max.y);
            glVertex2f(verts.min.x, verts.max.y);
        }
    }
    glEnd();
    BindFont(NULL);
}

void Graphics::RenderAxes () {
    glLineWidth(4.0);
    Graphics::SetColor(Color::red);
    Graphics::RenderLine(Vector2(0, 0), Vector2(1, 0));
    Graphics::SetColor(Color::green);
    Graphics::RenderLine(Vector2(0, 0), Vector2(0, 1));
}

void Graphics::DefaultMatrix () {
    glLoadIdentity();
    glScalef(instance->resolutionScale.x, instance->resolutionScale.y, 0.0f);
    glTranslatef(instance->screenCenter.x, instance->screenCenter.y, 0.0f);
}

void Graphics::ResolutionMatrix () {
    glLoadIdentity();
    glScalef(instance->resolutionScale.x, instance->resolutionScale.y, 0.0f);
}

void Graphics::IdentityMatrix () {
    glLoadIdentity();
}

void Graphics::ShowBuffer () {
    instance->m_viewport->SwapBuffers();
}

void Graphics::BindTexture (TextureAssetRef textureAsset) {
    if (textureAsset.HasAsset()) {
        glBindTexture(GL_TEXTURE_2D, textureAsset->glID());
    }
    else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Graphics::BindFont (FontAssetRef fontAsset) {
    if (fontAsset.HasAsset()) {
        glBindTexture(GL_TEXTURE_2D, fontAsset->glID());
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

Vector2 Graphics::GetMatrixPosition () {
    float m[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    float x = m[12];
    float y = m[13];
    return Vector2(x, y);
}

void Graphics::CheckErrors () {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
        LOG(ERROR) << "OpenGL Error: " << gluErrorString(error);
}

/*
void Graphics::BeginLine() {
    glBegin(GL_LINES);
}

void Graphics::BeginLineStrip()
{
    glBegin(GL_LINE_STRIP);
}

void Graphics::Vertex(Vector2 vertex)
{
    glVertex2f(vertex.x, vertex.y);
}

void Graphics::EndLine()
{
    glEnd();
}
*/

void Graphics::RenderCircle (float radius, int segments) {
	float increment = 2.0f * Volt::PI / segments;
	float theta = 0.0f;
	glBegin(GL_TRIANGLE_FAN);
	for (int32 i = 0; i < segments; ++i)
	{
		Vector2 v = radius * Vector2(cosf(theta), sinf(theta));
		glVertex2f(v.x, v.y);
		theta += increment;
	}
	glEnd();
}

void Graphics::RenderLineCircle (float radius, int segments) {
	float increment = 2.0f * Volt::PI / segments;
	float theta = 0.0f;
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < segments; ++i)
	{
		Vector2 v = radius * Vector2(cosf(theta), sinf(theta));
		glVertex2f(v.x, v.y);
		theta += increment;
	}
	glEnd();
}

bool Graphics::initialized () {
    return instance != NULL && instance->m_initialized;
}

void Graphics::SaveTextureToFile (int glId, string filename) {
    int width;
    int height;
    glBindTexture(GL_TEXTURE_2D, glId);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    if (width == 0 && height == 0) {
        LOG(WARNING) << "Texture to be saved doesn't exist or is 0 by 0.";
        return;
    }


    SDL_Surface* temp = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 24,
        #if SDL_BYTEORDER == SDL_LIL_ENDIAN
            0x000000FF, 0x0000FF00, 0x00FF0000, 0
        #else
            0x00FF0000, 0x0000FF00, 0x000000FF, 0
        #endif
    );

    unsigned char* pixels = (unsigned char*)malloc(3 * width * height);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    for (int i = 0; i < height; i++)
        memcpy(((char *)temp->pixels) + temp->pitch * i,
               pixels + 3 * width * (height - i - 1),
               width * 3);
    free(pixels);

    SDL_SaveBMP(temp, filename.c_str());
    SDL_FreeSurface(temp);
}

}
