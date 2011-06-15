#pragma once

#include "Core/Core.h"

namespace Volt {

class GpuProgram;

/** A filter, in the form of a shader, to be applied to the screen. Filters
 *  allow people to add multi-pass full-screen effects using shaders. Filters
 *  specify a layer they should be applied at and a shader that should be
 *  applied, and the engine handles the multiple passes. */
class Filter {
public:
    Filter (GpuProgram* program);
    virtual ~Filter ();

    void Render ();

    virtual void SetUniforms ();

    int layer () const { return m_layer; }

protected:
    GpuProgram* m_program;
    int m_colorMap;

private:
    friend class Scene;

    int m_layer;

    DISALLOW_COPY_AND_ASSIGN(Filter);
};

}
