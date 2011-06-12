#pragma once

#include "Core/Core.h"
#include "AssetRef.h"
#include "TextureAsset.h"

namespace Volt {

class DataSource;

/* Manages all assets, including asset loading/reloading. */
class AssetManager {
public:
    AssetManager (const DataSource* source);
    ~AssetManager ();

    AssetRef<TextureAsset> RequestTexture (
        const string& path,
        TextureAsset::FilterType type = TextureAsset::e_filterTypeLinear,
        bool repeatX = true, bool repeatY = true);

    void GarbageCollect ();

    void ReloadAll ();
    void ReloadPath (const Asset* asset, DataItem* item);

private:
    const DataSource* m_dataSource;
    typedef map<string, Asset*> Assets;
    Assets m_assets;

    DISALLOW_COPY_AND_ASSIGN(AssetManager);
};

}
