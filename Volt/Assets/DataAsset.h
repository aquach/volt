#pragma once

#include "Core/Core.h"
#include "Asset.h"

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

    static void Save (const Json::Value& root, string filename);

private:
    Json::Value m_root;
};

}