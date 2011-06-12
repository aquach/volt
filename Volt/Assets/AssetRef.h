#pragma once

#include "Core/Core.h"

namespace Volt {

template <class T> class AssetRef {
public:
    AssetRef () : m_asset(NULL) { }
    AssetRef (T* asset) : m_asset(asset) {
        if (m_asset)
            m_asset->AddReference();
    }
    AssetRef (const AssetRef& other) {
        m_asset = other.m_asset;
        if (m_asset)
            m_asset->AddReference();
    }

    ~AssetRef () { Free(); }

    T* operator->() { return m_asset; }

    bool HasAsset () const { return m_asset != NULL; }

    AssetRef& operator=(const AssetRef& other) {
        if (this != &other) {
            Free();
            m_asset = other.m_asset;
            if (m_asset)
                m_asset->AddReference();
        }
        return *this;
    }

private:
    void Free () {
        if (m_asset) {
            m_asset->RemoveReference();
            m_asset = NULL;
        }
    }

    T* m_asset;
};

}
