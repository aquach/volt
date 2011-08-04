#pragma once

#include "Game/Core/Core.h"
#include "Volt/Graphics/Filter.h"

namespace Volt {
    class GpuProgram;
    class RenderSurface;
}

// TODO: Maybe make SimpleFilter class that just takes a shader.
// Also maybe make filter that doesn't require bottom/top layer specification.
class BlendFilter : public Volt::Filter {
public:
    BlendFilter (const string& name, int bottomLayer, int topLayer);
    virtual ~BlendFilter ();

    virtual void OnAdded ();

    virtual void OnBottomLayer ();
    virtual void OnTopLayer ();

    void setBlendAmount (float amount) { m_amount = amount; }
    float amount () const { return m_amount; }
    void setBlendColor (Volt::Color color) { m_blendColor = color; }
    Volt::Color blendColor () const { return m_blendColor; }

private:
    Volt::RenderSurface* m_buffer;
    Volt::GpuProgram* m_shader;
    float m_amount;
    Volt::Color m_blendColor;
};
