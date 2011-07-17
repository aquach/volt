#pragma once

#include "Volt/Core/Core.h"
#include "Volt/Assets/AssetManager.h"
#include <set>

namespace Volt {

class GpuProgram {
public:
    GpuProgram ();
    ~GpuProgram ();

    void Attach (ShaderAssetRef shader);

    bool Link ();

    int handle () const { return m_handle; }

    bool IsLinked () const { return m_linked; }

    /* Unload shaders, but do not clear the shaders list. */
    void UnloadShaders ();
    void ReloadShaders ();

    static void UnloadAllShaders ();
    static void ReloadAllShaders ();
private:
    static set<GpuProgram*> s_programs;

    vector<ShaderAssetRef> m_shaders;
    int m_handle;
    bool m_linked;

    DISALLOW_COPY_AND_ASSIGN(GpuProgram);
};

}
