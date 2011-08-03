#pragma once

#include "Game/Core/Core.h"
#include "Volt/Graphics/Filter.h"

namespace Volt {
    class GpuProgram;
    class RenderSurface;
}

class EdgeFilter : public Volt::Filter {
public:
    EdgeFilter (int bottomLayer, int topLayer);
    virtual ~EdgeFilter ();

    virtual void OnBottomLayer ();
    virtual void OnTopLayer ();
private:
    Volt::RenderSurface* m_buffer;
    Volt::GpuProgram* m_shader;
};