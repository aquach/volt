#pragma once

#include "Core/Core.h"

namespace Volt {

class AssetManager;
class DataItem;
class DataSource;

/* A resource to be used in the game. */
class Asset {
public:
    Asset () : m_refCount(0) { }
    virtual ~Asset () { }

    void AddReference () { m_refCount++; }
    void RemoveReference () { m_refCount--; }
    int referenceCount () const { return m_refCount; }

    string path () const { return m_path; }

    virtual void Reload () = 0;
    virtual void Unload () = 0;

    AssetManager* manager () { return m_manager; }

protected:
    string m_path;
    AssetManager* m_manager;

private:
    friend class AssetManager;

    int m_refCount;

    DISALLOW_COPY_AND_ASSIGN(Asset);
};

}

