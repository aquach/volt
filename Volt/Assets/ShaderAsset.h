#pragma once

#include "Core/Core.h"
#include "Asset.h"

namespace Volt {

class DataItem;
class Shader;

/* A shader file. */
class ShaderAsset : public Asset {
public:
    enum ShaderType {
        SHADER_VERTEX,
        SHADER_FRAGMENT
    };

    ShaderAsset () : m_shader(NULL) { }
    ~ShaderAsset ();

    bool Load (const DataItem& item, ShaderType type);

    virtual void Reload ();
    virtual void Unload ();

    int handle () const;

private:
    Shader* m_shader;
    ShaderType m_type;
};

}
