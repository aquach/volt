#pragma once

#include "Volt/Core/Core.h"
#include "Volt/Assets/Asset.h"

namespace Json {
    class Value;
}

namespace Volt {

class DataItem;

/* A JSON data file. */
class DataAsset : public Asset {
public:
    DataAsset ();
    ~DataAsset ();

    bool Load (const DataItem& item);

    virtual void Reload ();
    virtual void Unload ();

    const Json::Value& data () const { return m_root; }

    static void Save (const Json::Value& root, const string& filename);

private:
    Json::Value m_root;
};

}
