#pragma once

#include "Volt/Core/Core.h"
#include "Volt/Assets/Asset.h"

namespace Volt {

class DataItem;

/* A Python script. */
class ScriptAsset : public Asset {
public:
    ScriptAsset ();
    ~ScriptAsset ();

    bool Load (const DataItem& item);

    virtual void Reload ();
    virtual void Unload ();

    const string& script () const { return m_script; }

private:
    string m_script;
};

}
