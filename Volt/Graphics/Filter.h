#pragma once

#include "Volt/Core/Core.h"
#include "Volt/Assets/AssetManager.h"

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

    void AddMap (string name, TextureAssetRef texture);

protected:
    GpuProgram* m_program;
    int m_colorMap;

private:
    friend class Scene;

    int m_layer;
    typedef map<string, TextureAssetRef> Maps;
    Maps m_maps;

    DISALLOW_COPY_AND_ASSIGN(Filter);
};

}
