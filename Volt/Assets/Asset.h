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

    /* Do not call these manually! Use AssetRef to keep reference-counted
     * pointers! */
    void AddReference () { m_refCount++; }
    void RemoveReference () { m_refCount--; CHECK_GE(m_refCount, 0); }

    int referenceCount () const { return m_refCount; }

    string path () const { return m_path; }

    virtual void Reload () = 0;
    virtual void Unload () = 0;

    /** Key that AssetManager uses to differentiate assets. Defaults to just
     *  the path, but child classes can override to add other keys like font
     *  size. */
    virtual string assetKey () const { return m_path; }

protected:
    string m_path;
    AssetManager* m_manager;

private:
    friend class AssetManager;

    int m_refCount;

    DISALLOW_COPY_AND_ASSIGN(Asset);
};

}

