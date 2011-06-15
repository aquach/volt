#pragma once

#include "Core/Core.h"
#include "Assets/AssetManager.h"

namespace Volt {

class GpuProgram {
public:
    GpuProgram ();
    ~GpuProgram ();

    void Attach (ShaderAssetRef shader);

    bool Link ();

    int handle () const { return m_handle; }

private:
    vector<ShaderAssetRef> m_shaders;
    int m_handle;
};

}
